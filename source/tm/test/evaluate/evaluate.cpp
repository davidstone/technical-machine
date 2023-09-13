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

import tm.stat.combined_stats;

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
	constexpr auto pokemon = Pokemon<generation>(
		Species::Blissey,
		Level(100_bi),
		Gender::female,
		Item::Leftovers,
		Ability::Natural_Cure,
		default_combined_stats<generation>,
		RegularMoves({Move(generation, MoveName::Tackle)})
	);

	auto environment = Environment();

	auto team1 = Team<generation>({pokemon});
	team1.pokemon().switch_in(environment);

	auto team2 = Team<generation>({pokemon});
	team2.pokemon().switch_in(environment);
	change_hp(team1.pokemon(), environment, -50_bi);

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
