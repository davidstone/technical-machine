// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.move.actual_damage;
import tm.move.move;
import tm.move.move_name;
import tm.move.move_result;
import tm.move.used_move;

import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.default_evs;

import tm.test.pokemon_init;

import tm.ability;
import tm.battle;
import tm.end_of_turn_flags;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;
import tm.visible_hp;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false, false);

constexpr auto visible_hp(auto const min, auto const max) -> VisibleHP {
	return VisibleHP(CurrentVisibleHP(min), MaxVisibleHP(max));
}

constexpr auto damaging_move(MoveName const move, VisibleHP const remaining) -> Used {
	auto result = Used(move);
	result.damage = remaining;
	return result;
}

TEST_CASE("Perish Song", "[Battle]") {
	constexpr auto generation = Generation::four;
	auto battle = Battle<generation>(
		make_known_team<generation>({
			{
				.species = Species::Misdreavus,
				.moves = {{
					MoveName::Perish_Song,
				}}
			},
			{
				.species = Species::Regice,
			},
		}),
		make_seen_team<generation>({
			{.species = Species::Starmie},
		})
	);

	auto const ai_pokemon = battle.ai().pokemon();
	auto const foe_pokemon = battle.foe().pokemon();
	for (auto const turn : containers::integer_range(4_bi)) {
		INFO(turn);

		CHECK(ai_pokemon.hp().current() == ai_pokemon.hp().max());
		CHECK(foe_pokemon.hp().current() == foe_pokemon.hp().max());

		battle.use_move(
			false,
			Used(MoveName::Recover),
			false
		);
		battle.use_move(
			true,
			Used(MoveName::Perish_Song),
			false
		);

		CHECK(ai_pokemon.hp().current() == ai_pokemon.hp().max());
		CHECK(foe_pokemon.hp().current() == foe_pokemon.hp().max());

		battle.end_turn(false, end_of_turn_flags, end_of_turn_flags);
	}

	CHECK(ai_pokemon.hp().current() == 0_bi);
	CHECK(foe_pokemon.hp().current() == 0_bi);
}

TEST_CASE("Accurate HP after move", "[Battle]") {
	constexpr auto generation = Generation::one;
	auto battle = Battle<generation>(
		make_known_team<generation>({
			{
				.species = Species::Bulbasaur,
				.moves = {{
					MoveName::Tackle,
				}}
			},
		}),
		make_seen_team<generation>({
			{
				.species = Species::Bulbasaur,
			},
		})
	);

	battle.use_move(
		true,
		damaging_move(MoveName::Tackle, visible_hp(99_bi, 100_bi)),
		false
	);

	battle.correct_hp(
		false,
		visible_hp(99_bi, 100_bi)
	);

	auto const visible_hp = battle.foe().all_pokemon()().visible_hp();
	CHECK(visible_hp.max.value() == 100_bi);
	CHECK(visible_hp.current.value() == 99_bi);

	auto const hp = battle.foe().pokemon().hp();
	CHECK(hp.max() == 293_bi);
	CHECK(hp.current() == 290_bi);
}

} // namespace
} // namespace technicalmachine
