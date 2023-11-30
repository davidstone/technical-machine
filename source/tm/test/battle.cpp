// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.move.actual_damage;
import tm.move.move;
import tm.move.move_name;
import tm.move.no_effect_function;
import tm.move.side_effects;
import tm.move.used_move;

import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.nickname;
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

TEST_CASE("Perish Song", "[Battle]") {
	constexpr auto generation = Generation::four;
	constexpr auto damage = ActualDamage::Known(0_bi);
	auto battle = Battle<generation>(
		make_known_team<generation>({
			{
				.species = Species::Misdreavus,
				.gender = Gender::female,
				.ability = Ability::Levitate,
				.moves = {{
					MoveName::Perish_Song,
				}}
			},
			{
				.species = Species::Regice,
				.ability = Ability::Clear_Body,
				.moves = {{
					MoveName::Explosion,
				}}
			},
		}),
		SeenTeam<generation>(2_bi)
	);
	battle.first_turn(true);
	battle.find_or_add_foe_pokemon(Species::Starmie, Nickname(), Level(100_bi), Gender::genderless);
	battle.add_move(false, MoveName::Recover);

	auto const recover_side_effects = possible_side_effects(
		MoveName::Recover,
		battle.foe().pokemon(),
		battle.ai(),
		battle.environment()
	);
	CHECK(containers::size(recover_side_effects) == 1_bi);

	auto call_recover = [&] {
		battle.handle_use_move(
			UsedMove<SeenTeam<generation>>(
				MoveName::Recover,
				containers::front(recover_side_effects).function
			),
			false,
			false,
			damage
		);
	};

	auto const perish_song_side_effects = possible_side_effects(
		MoveName::Perish_Song,
		battle.ai().pokemon(),
		battle.foe(),
		battle.environment()
	);
	CHECK(containers::size(perish_song_side_effects) == 1_bi);

	auto call_perish_song = [&] {
		battle.handle_use_move(
			UsedMove<KnownTeam<generation>>(
				MoveName::Perish_Song,
				containers::front(perish_song_side_effects).function
			),
			false,
			false,
			damage
		);
	};

	for (auto const turn : containers::integer_range(4_bi)) {
		INFO(turn);

		CHECK(battle.ai().pokemon().hp().current() == battle.ai().pokemon().hp().max());
		CHECK(battle.foe().pokemon().hp().current() == battle.foe().pokemon().hp().max());

		call_recover();
		call_perish_song();

		CHECK(battle.ai().pokemon().hp().current() == battle.ai().pokemon().hp().max());
		CHECK(battle.foe().pokemon().hp().current() == battle.foe().pokemon().hp().max());

		battle.handle_end_turn(false, end_of_turn_flags, end_of_turn_flags);
	}

	CHECK(battle.ai().pokemon().hp().current() == 0_bi);
	CHECK(battle.foe().pokemon().hp().current() == 0_bi);
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
	battle.first_turn(true);

	battle.handle_use_move(
		UsedMove<KnownTeam<generation>>(MoveName::Tackle, no_effect_function),
		false,
		false,
		ActualDamage::Known(3_bi)
	);

	battle.correct_hp(
		false,
		VisibleHP(
			CurrentVisibleHP(99_bi),
			MaxVisibleHP(100_bi)
		)
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
