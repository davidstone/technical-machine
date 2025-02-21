// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

export module tm.team_predictor.server.test.make_dictionary;

import tm.team_predictor.server.make_dictionary;

import tm.team_predictor.all_usage_stats;

import tm.test.usage_bytes;

import tm.generation;
import tm.nlohmann_json;

namespace technicalmachine {
namespace {
using namespace nlohmann::literals;

TEST_CASE("make_dictionary", "[make_dictionary]") {
	auto const all_usage_stats = AllUsageStats([](Generation const generation) {
		return bytes_to_usage_stats(smallest_team_bytes(generation));
	});
	auto const json = make_dictionary(all_usage_stats);
	auto const expected = R"({
		"generations": {
			"1": {
				"pokemon": {
					"Mew": {
						"moves": ["Cut"]
					}
				}
			},
			"2": {
				"items": ["None"],
				"pokemon": {
					"Mew": {
						"moves": ["Cut"]
					}
				}
			},
			"3": {
				"items": ["None"],
				"natures": [
					"Hardy",
					"Lonely",
					"Brave",
					"Adamant",
					"Naughty",
					"Bold",
					"Docile",
					"Relaxed",
					"Impish",
					"Lax",
					"Timid",
					"Hasty",
					"Serious",
					"Jolly",
					"Naive",
					"Modest",
					"Mild",
					"Quiet",
					"Bashful",
					"Rash",
					"Calm",
					"Gentle",
					"Sassy",
					"Careful",
					"Quirky"
				],
				"pokemon": {
					"Mew": {
						"abilities": ["Synchronize"],
						"moves": ["Cut"]
					}
				}
			},
			"4": {
				"items": ["None"],
				"natures": [
					"Hardy",
					"Lonely",
					"Brave",
					"Adamant",
					"Naughty",
					"Bold",
					"Docile",
					"Relaxed",
					"Impish",
					"Lax",
					"Timid",
					"Hasty",
					"Serious",
					"Jolly",
					"Naive",
					"Modest",
					"Mild",
					"Quiet",
					"Bashful",
					"Rash",
					"Calm",
					"Gentle",
					"Sassy",
					"Careful",
					"Quirky"
				],
				"pokemon": {
					"Mew": {
						"abilities": ["Synchronize"],
						"moves": ["Cut"]
					}
				}
			},
			"5": {
				"items": ["None"],
				"natures": [
					"Hardy",
					"Lonely",
					"Brave",
					"Adamant",
					"Naughty",
					"Bold",
					"Docile",
					"Relaxed",
					"Impish",
					"Lax",
					"Timid",
					"Hasty",
					"Serious",
					"Jolly",
					"Naive",
					"Modest",
					"Mild",
					"Quiet",
					"Bashful",
					"Rash",
					"Calm",
					"Gentle",
					"Sassy",
					"Careful",
					"Quirky"
				],
				"pokemon": {
					"Mew": {
						"abilities": ["Synchronize"],
						"moves": ["Cut"]
					}
				}
			},
			"6": {
				"items": ["None"],
				"natures": [
					"Hardy",
					"Lonely",
					"Brave",
					"Adamant",
					"Naughty",
					"Bold",
					"Docile",
					"Relaxed",
					"Impish",
					"Lax",
					"Timid",
					"Hasty",
					"Serious",
					"Jolly",
					"Naive",
					"Modest",
					"Mild",
					"Quiet",
					"Bashful",
					"Rash",
					"Calm",
					"Gentle",
					"Sassy",
					"Careful",
					"Quirky"
				],
				"pokemon": {
					"Mew": {
						"abilities": ["Synchronize"],
						"moves": ["Cut"]
					}
				}
			},
			"7": {
				"items": ["None"],
				"natures": [
					"Hardy",
					"Lonely",
					"Brave",
					"Adamant",
					"Naughty",
					"Bold",
					"Docile",
					"Relaxed",
					"Impish",
					"Lax",
					"Timid",
					"Hasty",
					"Serious",
					"Jolly",
					"Naive",
					"Modest",
					"Mild",
					"Quiet",
					"Bashful",
					"Rash",
					"Calm",
					"Gentle",
					"Sassy",
					"Careful",
					"Quirky"
				],
				"pokemon": {
					"Mew": {
						"abilities": ["Synchronize"],
						"moves": ["Cut"]
					}
				}
			},
			"8": {
				"items": ["None"],
				"natures": [
					"Hardy",
					"Lonely",
					"Brave",
					"Adamant",
					"Naughty",
					"Bold",
					"Docile",
					"Relaxed",
					"Impish",
					"Lax",
					"Timid",
					"Hasty",
					"Serious",
					"Jolly",
					"Naive",
					"Modest",
					"Mild",
					"Quiet",
					"Bashful",
					"Rash",
					"Calm",
					"Gentle",
					"Sassy",
					"Careful",
					"Quirky"
				],
				"pokemon": {
					"Mew": {
						"abilities": ["Synchronize"],
						"moves": ["Cut"]
					}
				}
			}
		}
	})"_json;
	CHECK(json == expected);
}

} // namespace
} // namespace technicalmachine
