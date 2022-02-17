// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/status.hpp>
#include <tm/ability.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

TEST_CASE("Awakening probability", "[Awakening probability]") {
	auto const ability = Ability::Pressure;
	auto status = Status(Statuses::sleep);
	for (auto const expected : {0.0, 1.0 / 4.0, 1.0 / 3.0, 1.0 / 2.0, 1.0 / 1.0}) {
		auto const calculated = status.probability_of_clearing(Generation::four, ability);
		CHECK(expected == calculated);
		constexpr auto clear_status = false;
		status.advance_from_move(ability, clear_status);
	}
}

} // namespace
} // namespace technicalmachine
