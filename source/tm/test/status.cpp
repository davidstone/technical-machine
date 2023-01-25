// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.status.status;
import tm.status.status_name;

import tm.ability;
import tm.generation;

namespace technicalmachine {
namespace {

TEST_CASE("Awakening probability", "[Awakening probability]") {
	auto const ability = Ability::Pressure;
	auto status = Status(StatusName::sleep);
	for (auto const expected : {0.0, 1.0 / 4.0, 1.0 / 3.0, 1.0 / 2.0, 1.0 / 1.0}) {
		auto const calculated = status.probability_of_clearing(Generation::four, ability);
		CHECK(expected == calculated);
		constexpr auto clear_status = false;
		status.advance_from_move(ability, clear_status);
	}
}

} // namespace
} // namespace technicalmachine
