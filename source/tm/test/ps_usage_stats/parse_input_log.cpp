module;

#include <catch2/catch_test_macros.hpp>

export module tm.test.ps_usage_stats.parse_input_log;

import tm.clients.ps.battle_response_switch;

import tm.clients.party;

import tm.move.move_name;

import tm.ps_usage_stats.parse_input_log;

import tm.nlohmann_json;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;
using namespace ps_usage_stats;

TEST_CASE("Parse input log", "[parse_input_log]") {
	auto const parsed = parse_input_log(nlohmann::json({
		">version 73a206e1a35e3dc83c7c975610dc2697cf7d8a34",
		">start {\"formatid\":\"gen1ou\",\"seed\":[1,2,3,4],\"rated\":\"Rated battle\"}",
		">player p1 {\\\"\\\"name\\\":\\\"Player 1\\\"}",
		">player p2 {\\\"\\\"name\\\":\\\"Player 2\\\"}",
		">p1 move bodyslam",
		">p2 switch 3",
		">p1 move bodyslam",
		">p2 move blizzard",
		">p1 switch 2",
		">p1 move thunder",
		">p2 switch 2",
	}));
	auto const expected = InputLog(
		{{
			MoveName::Body_Slam,
			MoveName::Body_Slam,
			ps::BattleResponseSwitch(2_bi),
			MoveName::Thunder
		}},
		{{
			ps::BattleResponseSwitch(3_bi),
			MoveName::Blizzard,
			ps::BattleResponseSwitch(2_bi)
		}}
	);
	CHECK(parsed == expected);
}

} // namespace technicalmachine