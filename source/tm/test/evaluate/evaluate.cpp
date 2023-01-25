// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.evaluate.evaluate;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.change_hp;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;
import tm.weather;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

TEST_CASE("Evaluate", "[Evaluate]") {
	auto pokemon = Pokemon<generation>(
		Species::Blissey,
		Level(100_bi),
		Gender::female,
		Item::Leftovers,
		Ability::Natural_Cure,
		default_combined_stats<generation>,
		RegularMoves({Move(generation, MoveName::Tackle)})
	);

	auto weather = Weather();

	auto team1 = Team<generation>({pokemon});
	team1.pokemon().switch_in(weather);

	auto team2 = Team<generation>({pokemon});
	team2.pokemon().switch_in(weather);
	change_hp(team1.pokemon(), weather, -50_bi);

	auto const evaluate = Evaluate<generation>();
	CHECK(evaluate(team1, team2) < 0_bi);
}

} // namespace
} // namespace technicalmachine
