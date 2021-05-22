// Test damage-related functions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/evaluate/evaluate.hpp>

#include <tm/evaluate/evaluate.hpp>

#include <tm/pokemon/species.hpp>

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/team.hpp>
#include <tm/weather.hpp>

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
		default_combined_stats<generation>,
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
