// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>

import tm.move.actual_damage;
import tm.move.move_name;
import tm.move.move_result;
import tm.move.switch_;
import tm.move.used_move;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.test.make_seen_team;

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

constexpr auto damaging_move(MoveName const move, VisibleHP const remaining) -> VisibleMove {
	auto result = VisibleMove(move);
	result.damage = remaining;
	return result;
}

TEST_CASE("Perish Song", "[Battle]") {
	constexpr auto generation = Generation::four;
	auto battle = Battle<generation>(
		KnownTeam<generation>({{
			{
				.species = Species::Misdreavus,
				.moves = {{
					MoveName::Perish_Song,
				}}
			},
			{
				.species = Species::Regice,
				.moves = {{
					MoveName::Tackle,
				}}
			},
		}}),
		make_seen_team<generation>({
			.team_size = 1_bi,
			.species = Species::Starmie,
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
			VisibleMove(MoveName::Recover),
			false
		);
		battle.use_move(
			true,
			VisibleMove(MoveName::Perish_Song),
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
		KnownTeam<generation>({{
			{
				.species = Species::Bulbasaur,
				.moves = {{
					MoveName::Tackle,
				}}
			},
		}}),
		make_seen_team<generation>({
			.team_size = 1_bi,
			.species = Species::Bulbasaur,
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

TEST_CASE("Handle Toxic in generation 1", "[Battle]") {
	constexpr auto generation = Generation::one;
	auto battle = Battle<generation>(
		KnownTeam<generation>({{
			{
				.species = Species::Magikarp,
				.moves = {{
					MoveName::Splash,
				}}
			},
			{
				.species = Species::Gyarados,
				.moves = {{
					MoveName::Splash,
				}}
			},
		}}),
		make_seen_team<generation>({
			.team_size = 1_bi,
			.species = Species::Jolteon,
		})
	);

	battle.use_move(
		false,
		VisibleMove(MoveName::Toxic),
		false
	);

	battle.use_move(
		true,
		VisibleMove(MoveName::Splash),
		false
	);

	{
		auto const hp = battle.ai().pokemon().hp();
		CHECK(hp.current() == 228_bi);
		CHECK(hp.max() == 243_bi);
	}

	battle.end_turn(false, end_of_turn_flags, end_of_turn_flags);

	{
		auto const hp = battle.ai().pokemon().hp();
		CHECK(hp.current() == 228_bi);
		CHECK(hp.max() == 243_bi);
	}

	battle.use_switch(true, Switch(1_bi));

	battle.use_move(
		false,
		damaging_move(MoveName::Thunderbolt, visible_hp(0_bi, 393_bi)),
		false
	);

	battle.use_switch(true, Switch(0_bi));

	{
		auto const hp = battle.ai().pokemon().hp();
		CHECK(hp.current() == 228_bi);
		CHECK(hp.max() == 243_bi);
	}
}

TEST_CASE("Handle hitting self in confusion", "[Battle]") {
	constexpr auto generation = Generation::one;
	auto battle = Battle<generation>(
		KnownTeam<generation>({{
			{
				.species = Species::Exeggutor,
				.moves = {{
					MoveName::Psychic,
				}}
			},
		}}),
		make_seen_team<generation>({
			.team_size = 1_bi,
			.species = Species::Gengar,
		})
	);

	battle.use_move(
		false,
		VisibleMove(MoveName::Confuse_Ray),
		false
	);

	battle.hit_self_in_confusion(
		true,
		visible_hp(355_bi, 393_bi)
	);

	auto const hp = battle.ai().pokemon().hp();
	CHECK(hp.current() == 355_bi);
	CHECK(hp.max() == 393_bi);

	
}

} // namespace
} // namespace technicalmachine
