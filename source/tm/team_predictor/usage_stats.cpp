// Load stats such as Pokemon usage stats
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/usage_stats.hpp>

#include <tm/invalid_settings_file.hpp>
#include <tm/buffer_view.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>

#include <bounded/to_integer.hpp>

#include <containers/algorithms/ska_sort.hpp>
#include <containers/front_back.hpp>
#include <containers/push_back.hpp>
#include <containers/take.hpp>
#include <containers/vector.hpp>

#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <cmath>
#include <fstream>
#include <string>

namespace technicalmachine {

namespace {

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
	} else if constexpr (std::is_same_v<T, CombinedStats<EV>>) {
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
		return CombinedStats<EV>{nature, hp, atk, def, spa, spd, spe};
	}
}

template<typename T>
auto per_pokemon_data(boost::property_tree::ptree const & pt, auto const max) {
	auto full = containers::vector<std::pair<std::string_view, double>>();
	for (auto const & p : pt) {
		auto datum_str = std::string_view(p.first);
		if (datum_str == "") {
			continue;
		}
		containers::push_back(full, std::pair(datum_str, p.second.template get<double>("")));
	}
	containers::ska_sort(full, [](auto const & value) { return -value.second; });
	return containers::static_vector<T, max.value()>(containers::transform(
		containers::take(full, max),
		[](auto const & pair) { return from_statistics_string<T>(pair.first); }
	));
}

template<typename T>
auto per_pokemon_datum(boost::property_tree::ptree const & pt) {
	auto all = per_pokemon_data<T>(pt, 1_bi);
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

void turn_teammates_into_multiplier(containers::array<UsageStats::PerPokemon, number_of_species> & all_per_pokemon) {
	constexpr auto all_species = containers::enum_range<Species>();
	for (auto const species_outer : all_species) {
		auto & per_pokemon = all_per_pokemon[bounded::integer(species_outer)];
		for (auto const species_inner : all_species) {
			auto & multiplier = per_pokemon.teammates[bounded::integer(species_inner)];
			if (is_alternate_form(species_outer, species_inner)) {
				multiplier = 0.0F;
				continue;
			}
			auto const base_value = all_per_pokemon[bounded::integer(species_inner)].weighted_usage;
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
	auto pt = boost::property_tree::ptree();
	read_json(path.string(), pt);

	auto const & data = pt.get_child("data");
	for (auto const & pokemon : data) {
		auto const species = from_string<Species>(pokemon.first);
		auto & per_pokemon = m_all_per_pokemon[bounded::integer(species)];

		per_pokemon.weighted_usage = pokemon.second.get<float>("usage");
		check_finite(per_pokemon.weighted_usage);
		check_non_negative(per_pokemon.weighted_usage);
		m_total_weighted_usage += per_pokemon.weighted_usage;

		for (auto const & teammate : pokemon.second.get_child("Teammates")) {
			if (teammate.first == "empty") {
				continue;
			}
			auto const value = std::stof(teammate.second.get<std::string>(""));
			check_finite(value);
			per_pokemon.teammates[bounded::integer(from_string<Species>(teammate.first))] = value;
		}
		per_pokemon.moves = per_pokemon_data<Moves>(pokemon.second.get_child("Moves"), max_moves_per_pokemon);
		per_pokemon.ability = per_pokemon_datum<Ability>(pokemon.second.get_child("Abilities"));
		per_pokemon.item = per_pokemon_datum<Item>(pokemon.second.get_child("Items"));
		per_pokemon.stats = per_pokemon_datum<CombinedStats<EV>>(pokemon.second.get_child("Spreads"));
	}

	check_finite(m_total_weighted_usage);
	check_non_negative(m_total_weighted_usage);
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
		stats_for_generation(Generation::seven)
		// stats_for_generation(Generation::eight)
	}
{
	static_assert(bounded::max_value<Generation> == Generation::eight);
}

} // namespace technicalmachine
