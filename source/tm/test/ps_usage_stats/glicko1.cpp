// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/glicko1.hpp>

#include <cmath>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

TEST_CASE("Glicko-1", "[Glicko-1]") {
	auto glicko1 = ps_usage_stats::Glicko1();
	glicko1.add_result(1_bi, 2_bi, ps_usage_stats::BattleResult::Winner::side1);
	glicko1.finalize();
	auto print = [&](auto const index, ps_usage_stats::Rating const expected) {
		auto rating = glicko1.get(index);
		CHECK(std::round(rating.value) == expected.value);
		CHECK(std::round(rating.deviation) == expected.deviation);
	};
	print(1_bi, ps_usage_stats::Rating(1662.0, 290.0));
	print(2_bi, ps_usage_stats::Rating(1338.0, 290.0));
}

} // namespace
} // namespace technicalmachine
