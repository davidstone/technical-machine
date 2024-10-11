// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <operators/forward.hpp>

export module tm.team_predictor.server.make_dictionary;

import tm.stat.nature;

import tm.string_conversions.ability;
import tm.string_conversions.generation;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.species;

import tm.team_predictor.server.style;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.usage_stats;
import tm.team_predictor.usage_stats_probabilities;

import tm.generation;
import tm.item;
import tm.nlohmann_json;

import containers;
import std_module;

namespace technicalmachine {
namespace {

auto json_from_range(auto && range) -> nlohmann::json {
	auto result = nlohmann::json();
	for (auto && value : range) {
		result.update(OPERATORS_FORWARD(value));
	}
	return result;
}

auto json_array_from_range(auto && range) -> nlohmann::json {
	return nlohmann::json(nlohmann::json::array_t(
		containers::make_legacy_iterator(containers::begin(range)),
		containers::make_legacy_iterator(containers::end(range))
	));
}

auto json_from_enum_range(auto && range) -> nlohmann::json {
	return json_array_from_range(containers::transform(
		range,
		[](auto const & value) { return to_string(value); }
	));
}

auto pokemon_dictionary(Generation const generation, UsageStatsProbabilities::Inner const & probabilities) -> nlohmann::json {
	auto json = nlohmann::json();
	json["moves"] = json_from_enum_range(containers::transform(
		probabilities.moves.map(),
		containers::get_key
	));
	if (generation >= Generation::three) {
		json["abilities"] = json_from_enum_range(containers::transform(
			probabilities.abilities.map(),
			containers::get_key
		));
	}
	return json;
}

auto item_dictionary(UsageStatsProbabilities::Map const & probabilities) -> nlohmann::json {
	auto used_items = std::unordered_set<Item>();
	for (auto const & entry : probabilities) {
		for (auto const value : entry.mapped.items.map()) {
			used_items.insert(value.key);
		}
	}
	return json_from_enum_range(used_items);
}

auto generation_dictionary(Generation const generation, UsageStats const & usage_stats) -> nlohmann::json {
	auto json = nlohmann::json();
	auto const & probabilities = usage_stats.assuming().map();
	json["pokemon"] = json_from_range(containers::transform(
		probabilities,
		[&](auto const & entry) {
			return nlohmann::json({{
				to_string(entry.key),
				pokemon_dictionary(generation, entry.mapped)
			}});
		}
	));
	if (generation >= Generation::two) {
		json["items"] = item_dictionary(probabilities);
	}
	if (generation >= Generation::three) {
		json["natures"] = json_from_enum_range(containers::enum_range<Nature>());
	}
	return json;
}

} // namespace

export auto make_dictionary(AllUsageStats const & all_usage_stats) -> nlohmann::json {
	return nlohmann::json({{
		"generations",
		json_from_range(containers::transform(
			containers::enum_range<Generation>(),
			[&](Generation const generation) {
				return nlohmann::json({{
					to_string(generation),
					generation_dictionary(generation, all_usage_stats[generation])
				}});
			}
		))
	}});
}

} // namespace technicalmachine
