// Test status functions
// Copyright (C) 2014 David Stone
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

#include "status.hpp"

#include "../rational.hpp"
#include "../status.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../pokemon/species.hpp"

#include <bounded_integer/array.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
namespace {
class InvalidSleepProbability : public std::runtime_error {
public:
	template<typename N1, typename D1, typename N2, typename D2>
	InvalidSleepProbability(bounded_rational<N1, D1> const expected, bounded_rational<N2, D2> const calculated):
		std::runtime_error("Expected: " + to_string(expected) + " but got " + to_string(calculated))
		{
	}
};

void awakening_probability_tests() {
	Weather weather;
	Team team(max_pokemon_per_team);
	Level const level(100_bi);
	team.add_pokemon(Species::Zapdos, level, Gender());
	auto pokemon = team.pokemon();
	Status::apply<Statuses::sleep>(pokemon, weather);
	for (auto const expected : bounded::make_array(make_rational(0_bi, 1_bi), make_rational(1_bi, 4_bi), make_rational(1_bi, 3_bi), make_rational(1_bi, 2_bi), make_rational(1_bi, 1_bi))) {
		auto const calculated = awaken_probability(pokemon);
		if (expected != calculated) {
			throw InvalidSleepProbability(expected, calculated);
		}
		pokemon.increase_sleep_counter();
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
