// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/status.hpp>

#include <tm/ability.hpp>
#include <tm/status.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
namespace {
using std::to_string;

struct InvalidSleepProbability : std::runtime_error {
	InvalidSleepProbability(double const expected, double const calculated):
		std::runtime_error("Expected: " + to_string(expected) + " but got " + to_string(calculated))
		{
	}
};

void awakening_probability_tests() {
	auto const ability = Ability::Pressure;
	auto status = Status(Statuses::sleep);
	for (auto const expected : { 0.0, 1.0 / 4.0, 1.0 / 3.0, 1.0 / 2.0, 1.0 / 1.0 }) {
		auto const calculated = status.probability_of_clearing(Generation::four, ability);
		if (expected != calculated) {
			throw InvalidSleepProbability(expected, calculated);
		}
		constexpr auto clear_status = false;
		status.advance_from_move(ability, clear_status);
	}
}

}	// namespace

void status_tests () {
	std::cout << "Running status tests.\n";
	std::cout << "\tRunning awakening probability tests.\n";
	awakening_probability_tests();
	std::cout << "Status tests passed.\n\n";
}

}	// namespace technicalmachine
