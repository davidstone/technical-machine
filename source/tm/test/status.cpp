// Copyright (C) 2016 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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
