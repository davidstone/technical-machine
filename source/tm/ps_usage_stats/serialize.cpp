// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/serialize.hpp>

#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/species.hpp>

#include <containers/algorithms/filter_iterator.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/integer_range.hpp>
#include <containers/size.hpp>
#include <containers/static_vector.hpp>

#include <cstdint>
#include <string>

namespace technicalmachine::ps_usage_stats {
namespace {

template<typename T>
constexpr auto get_used(auto const get) {
	struct Result {
		T name;
		double value;
	};
	return containers::static_vector<Result, static_cast<std::size_t>(number_of<T>)>(
		containers::filter(
			containers::transform(containers::enum_range<T>(), [&](T const name) {
				return Result{name, get(name)};
			}),
			[](Result const x) { return x.value != 0.0; }
		)
	);
}

template<typename T>
auto serialize(UsageStats const & usage_stats, Species const species) {
	auto result = nlohmann::json();
	auto const used = get_used<T>([&](T const value) { return usage_stats.get(species, value); });
	for (auto const value : used) {
		result[std::string(to_string(value.name))] = value.value;
	}
	return result;
}

auto serialize_speed(UsageStats const & usage_stats, Species const species) {
	auto result = nlohmann::json();
	auto const & speed_distribution = usage_stats.speed_distribution(species);
	for (auto const index : containers::integer_range(containers::size(speed_distribution))) {
		auto const speed = speed_distribution[index];
		if (speed == 0.0) {
			continue;
		}
		result[std::string(to_string(index))] = speed;
	}
	return result;
};

auto serialize_correlations(Generation const generation, Correlations::TopMoves const & top_moves) -> nlohmann::json {
	auto result = nlohmann::json();
	for (auto const & top_move : top_moves) {
		auto & per_move = result[std::string(to_string(top_move.key))];

		auto & teammates = per_move["Teammates"];
		for (auto const related : top_move.mapped.moves_and_species) {
			teammates[std::string(to_string(related.key.other_species))][std::string(to_string(related.key.other_move))] = related.mapped;
		}
		if (generation >= Generation::two) {
			auto & items = per_move["Items"];
			for (auto const related : top_move.mapped.items) {
				items[std::string(to_string(related.key))] = related.mapped;
			}
		}
		if (generation >= Generation::three) {
			auto & abilities = per_move["Abilities"];
			for (auto const related : top_move.mapped.abilities) {
				abilities[std::string(to_string(related.key))] = related.mapped;
			}
		}
	}
	return result;
}

} // namespace

auto serialize(Generation const generation, UsageStats const & usage_stats, Correlations const & correlations) -> nlohmann::json {
	auto output = nlohmann::json();
	auto & all_pokemon = output["Pokemon"];
	for (auto const species : get_used<Species>([&](Species const species) { return usage_stats.get_total(species); })) {
		auto & species_data = all_pokemon[std::string(to_string(species.name))];
		species_data = {
			{"Count", species.value},
			{"Moves", serialize<Moves>(usage_stats, species.name)},
			{"Speed", serialize_speed(usage_stats, species.name)},
			{"Correlations", serialize_correlations(generation, correlations.top_moves(species.name))}
		};
		if (generation >= Generation::two) {
			species_data["Items"] = serialize<Item>(usage_stats, species.name);
		}
		if (generation >= Generation::three) {
			species_data["Abilities"] = serialize<Ability>(usage_stats, species.name);
		}
	}
	output["Number of teams"] = usage_stats.number_of_teams().value();

	return output;
}

} // namespace technicalmachine::ps_usage_stats
