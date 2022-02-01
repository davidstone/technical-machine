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

}	// namespace

UsageStats::UsageStats(std::filesystem::path const & usage_stats_directory) {
	auto const path = usage_stats_directory / "usage.json";
	auto const json = load_json_from_file(path);

	auto const & data = json.at("data");
	for (auto const & pokemon : data.items()) {
		auto const species = from_string<Species>(pokemon.key());
		auto & per_pokemon = m_all_per_pokemon[bounded::integer(species)];

		per_pokemon.weight = pokemon.value().at("usage").get<float>();
		check_finite(per_pokemon.weight);
		check_non_negative(per_pokemon.weight);

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
	}
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
