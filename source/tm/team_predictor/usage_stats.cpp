// Load stats such as Pokemon usage stats
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/usage_stats.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <tm/buffer_view.hpp>
#include <tm/load_json_from_file.hpp>
#include <tm/invalid_settings_file.hpp>

#include <bounded/to_integer.hpp>

#include <containers/algorithms/ska_sort.hpp>
#include <containers/front_back.hpp>
#include <containers/push_back.hpp>
#include <containers/take.hpp>
#include <containers/vector.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <nlohmann/json.hpp>

#include <cmath>
#include <fstream>
#include <string>

namespace technicalmachine {

namespace {

struct NatureAndEVs {
	Nature nature;
	EVs evs;
};

template<typename T>
auto from_statistics_string(std::string_view const str) {
	if constexpr (std::is_same_v<T, Moves>) {
		return from_string<Moves>(str);
	} else if constexpr (std::is_same_v<T, Item>) {
		return
			str == "nothing" ? Item::None :
			from_string<Item>(str);
	} else if constexpr (std::is_same_v<T, Ability>) {
		return
			str == "noability" ? Ability::Honey_Gather :
			from_string<Ability>(str);
	} else if constexpr (std::is_same_v<T, NatureAndEVs>) {
		auto buffer = DelimitedBufferView(str, '/');
		auto const nature = from_string<Nature>(buffer.pop(':'));
		auto get_ev = [&]{
			return EV(bounded::to_integer<EV::value_type>(buffer.pop()));
		};
		auto const hp = get_ev();
		auto const atk = get_ev();
		auto const def = get_ev();
		auto const spa = get_ev();
		auto const spd = get_ev();
		auto const spe = get_ev();
		return NatureAndEVs{nature, EVs(hp, atk, def, spa, spd, spe)};
	}
}

template<typename T>
auto per_pokemon_data(nlohmann::json const & json, auto const max) {
	// TODO: Use push_heap and pop_heap?
	auto full = containers::vector<nlohmann::json::const_iterator>();
	for (auto it = json.begin(); it != json.end(); ++it) {
		if (it.key() == "") {
			continue;
		}
		containers::push_back(full, it);
	}
	containers::ska_sort(full, [](nlohmann::json::const_iterator const & it) { return -it.value().get<double>(); });
	return containers::static_vector<T, max>(containers::transform(
		containers::take(full, max),
		[](nlohmann::json::const_iterator const & it) { return from_statistics_string<T>(it.key()); }
	));
}

template<typename T>
auto per_pokemon_datum(nlohmann::json const & json) {
	auto all = per_pokemon_data<T>(json, 1_bi);
	if (containers::is_empty(all)) {
		throw std::runtime_error("Bad statistics file");
	}
	return containers::front(all);
}

bool is_deoxys(Species const species) {
	switch (species) {
		case Species::Deoxys_Attack:
		case Species::Deoxys_Defense:
		case Species::Deoxys_Normal:
		case Species::Deoxys_Speed:
			return true;
		default:
			return false;
	}
}

bool is_giratina(Species const species) {
	switch (species) {
		case Species::Giratina_Altered:
		case Species::Giratina_Origin:
			return true;
		default:
			return false;
	}
}

bool is_rotom(Species const species) {
	switch (species) {
		case Species::Rotom:
		case Species::Rotom_Mow:
		case Species::Rotom_Frost:
		case Species::Rotom_Heat:
		case Species::Rotom_Fan:
		case Species::Rotom_Wash:
			return true;
		default:
			return false;
	}
}

bool is_shaymin(Species const species) {
	switch (species) {
		case Species::Shaymin_Land:
		case Species::Shaymin_Sky:
			return true;
		default:
			return false;
	}
}

bool is_wormadam(Species const species) {
	switch (species) {
		case Species::Wormadam_Plant:
		case Species::Wormadam_Sandy:
		case Species::Wormadam_Trash:
			return true;
		default:
			return false;
	}
}

bool is_alternate_form(Species const first, Species const second) {
	return
		first == second or
		(is_deoxys(first) and is_deoxys(second)) or
		(is_giratina(first) and is_giratina(second)) or
		(is_rotom(first) and is_rotom(second)) or
		(is_shaymin(first) and is_shaymin(second)) or
		(is_wormadam(first) and is_wormadam(second));
}

void check_finite(float const value) {
	if (!std::isfinite(value)) {
		throw std::runtime_error("Non-finite value found in configuration");
	}
}

void check_non_negative(float const value) {
	if (value < 0.0F) {
		throw std::runtime_error("Negative value found where a positive was expected");
	}
}

void turn_teammates_into_multiplier(containers::array<UsageStats::PerPokemon, number_of<Species>> & all_per_pokemon) {
	constexpr auto all_species = containers::enum_range<Species>();
	for (auto const species_outer : all_species) {
		auto & per_pokemon = all_per_pokemon[bounded::integer(species_outer)];
		for (auto const species_inner : all_species) {
			auto & multiplier = per_pokemon.teammates[bounded::integer(species_inner)];
			if (is_alternate_form(species_outer, species_inner)) {
				multiplier = 0.0F;
				continue;
			}
			auto const base_value = all_per_pokemon[bounded::integer(species_inner)].usage;
			if (base_value == 0.0F) {
				multiplier = 1.0F;
			} else {
#if 0
				multiplier = (base_value + multiplier) / base_value;
				check_finite(multiplier);
				check_non_negative(multiplier);
#else
				multiplier = 1.0F;
#endif
			}
		}
	}
}

}	// namespace

UsageStats::UsageStats(std::filesystem::path const & usage_stats_directory) {
	auto const path = usage_stats_directory / "usage.json";
	auto const json = load_json_from_file(path);

	auto const & data = json.at("data");
	for (auto const & pokemon : data.items()) {
		auto const species = from_string<Species>(pokemon.key());
		auto & per_pokemon = m_all_per_pokemon[bounded::integer(species)];

		per_pokemon.usage = pokemon.value().at("usage").get<float>();
		check_finite(per_pokemon.usage);
		check_non_negative(per_pokemon.usage);
		m_total_usage += per_pokemon.usage;

		auto const & teammates = pokemon.value().at("Teammates");
		for (auto teammate = teammates.begin(); teammate != teammates.end(); ++teammate) {
			if (teammate.key() == "empty") {
				continue;
			}
			auto const value = teammate.value().get<float>();
			check_finite(value);
			per_pokemon.teammates[bounded::integer(from_string<Species>(teammate.key()))] = value;
		}
		per_pokemon.moves = per_pokemon_data<Moves>(pokemon.value().at("Moves"), max_moves_per_pokemon);
		per_pokemon.ability = per_pokemon_datum<Ability>(pokemon.value().at("Abilities"));
		per_pokemon.item = per_pokemon_datum<Item>(pokemon.value().at("Items"));
		auto const stats = per_pokemon_datum<NatureAndEVs>(pokemon.value().at("Spreads"));
		per_pokemon.nature = stats.nature;
		per_pokemon.evs = stats.evs;
	}

	check_finite(m_total_usage);
	check_non_negative(m_total_usage);
	turn_teammates_into_multiplier(m_all_per_pokemon);
}

namespace {

auto stats_for_generation(Generation const generation) {
	return UsageStats(std::filesystem::path("settings") / bounded::to_string(bounded::integer(generation)) / "OU");
}

} // namespace

AllUsageStats::AllUsageStats():
	m_all_stats{
		stats_for_generation(Generation::one),
		stats_for_generation(Generation::two),
		stats_for_generation(Generation::three),
		stats_for_generation(Generation::four),
		stats_for_generation(Generation::five),
		stats_for_generation(Generation::six),
		stats_for_generation(Generation::seven),
		stats_for_generation(Generation::eight)
	}
{
	static_assert(numeric_traits::max_value<Generation> == Generation::eight);
}

} // namespace technicalmachine
