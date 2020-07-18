// Test damage-related functions
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

#include <tm/test/evaluate/evaluate.hpp>

#include <tm/evaluate/evaluate.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/pokemon/species.hpp>

#include <bounded/integer.hpp>

#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

void assert_lower_score(Evaluate<generation> const & evaluate, Team<generation> const & lesser, Team<generation> const & greater) {
	BOUNDED_ASSERT(evaluate(lesser, greater) < 0_bi);
}

}	// namespace

void evaluate_tests() {
	std::cout << "Running evaluate tests.\n";

	auto pokemon = Pokemon<generation>(
		Species::Blissey,
		Level(100_bi),
		Gender::female,
		Item::Leftovers,
		Ability::Natural_Cure,
		CombinedStats<IVAndEV>{
			Nature::Hardy,
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
			{IV(31_bi), EV(0_bi)},
		},
		RegularMoves({Move(generation, Moves::Tackle)})
	);

	auto weather = Weather();

	auto team1 = Team<generation>(1_bi);
	team1.add_pokemon(pokemon);
	team1.pokemon().switch_in(weather);

	auto team2 = Team<generation>(1_bi);
	team2.add_pokemon(pokemon);
	team2.pokemon().switch_in(weather);
	change_hp(team1.pokemon(), weather, -50_bi);

	auto const evaluate = Evaluate<generation>();
	assert_lower_score(evaluate, team1, team2);

	std::cout << "Evaluate tests passed.\n\n";
}

}	// namespace technicalmachine
