// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

export module tm.test.ps_usage_stats.glicko1;

import tm.ps_usage_stats.battle_result;
import tm.ps_usage_stats.glicko1;
import tm.ps_usage_stats.rating;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

TEST_CASE("Glicko-1", "[Glicko-1]") {
	auto glicko1 = ps_usage_stats::Glicko1();
	glicko1.add_result(1_bi, 2_bi, ps_usage_stats::BattleResult::Winner::side1);
	glicko1.finalize();
	auto check = [&](auto const index, ps_usage_stats::Rating const expected) {
		auto rating = glicko1.get(index);
		CHECK(std::round(rating.value) == expected.value);
		CHECK(std::round(rating.deviation) == expected.deviation);
	};
	check(1_bi, ps_usage_stats::Rating(1540.0, 123.0));
	check(2_bi, ps_usage_stats::Rating(1460.0, 123.0));
}

} // namespace technicalmachine
