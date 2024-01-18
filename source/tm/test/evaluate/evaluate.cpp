// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

import tm.evaluate.evaluate;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.change_hp;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.test.pokemon_init;

import tm.ability;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

static_assert([] {
	auto environment = Environment();

	auto team1 = Team<generation>({{
		{
			.species = Species::Blissey,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	team1.pokemon().switch_in(environment);
	change_hp(team1.pokemon(), environment, -50_bi);

	auto team2 = Team<generation>({{
		{
			.species = Species::Blissey,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	team2.pokemon().switch_in(environment);

	constexpr auto evaluate = Evaluate<generation>({
		.hp = 1000_bi,
		.hidden = 0_bi,
		.spikes = 0_bi,
		.stealth_rock = 0_bi,
		.toxic_spikes = 0_bi
	});
	return evaluate(team1, team2) < 0_bi;
}());

} // namespace
} // namespace technicalmachine
