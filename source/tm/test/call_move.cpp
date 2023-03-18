// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.max_moves_per_pokemon;
import tm.move.move;
import tm.move.move_name;
import tm.move.no_effect_function;
import tm.move.other_move;
import tm.move.regular_moves;
import tm.move.side_effects;
import tm.move.used_move;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.status.status_name;

import tm.ability;
import tm.block;
import tm.contact_ability_effect;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;
import tm.switch_decision_required;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;
constexpr auto damage = ActualDamage::Unknown{};

constexpr auto regular_moves(auto... moves) {
	return RegularMoves{Move(generation, moves)...};
}

TEST_CASE("Baton Pass", "[call_move]") {
	auto environment = Environment();

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Smeargle,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Own_Tempo,
			default_combined_stats<generation>,
			regular_moves(MoveName::Baton_Pass, MoveName::Belly_Drum)
		),
		Pokemon<generation>(
			Species::Alakazam,
			Level(100_bi),
			Gender::male,
			Item::Lum_Berry,
			Ability::Synchronize,
			CombinedStats<generation>{
				Nature::Hardy,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(0_bi),
					EV(252_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi)
				)
			},
			regular_moves(MoveName::Psycho_Cut, MoveName::Recover)
		)
	});
	attacker.pokemon().switch_in(environment);

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Gengar,
			Level(100_bi),
			Gender::male,
			Item::Choice_Specs,
			Ability::Levitate,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(252_bi),
					EV(0_bi),
					EV(0_bi)
				)
			},
			regular_moves(MoveName::Shadow_Ball)
		),
		Pokemon<generation>(
			Species::Misdreavus,
			Level(100_bi),
			Gender::female,
			Item::Choice_Specs,
			Ability::Levitate,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(252_bi),
					EV(0_bi),
					EV(0_bi)
				)
			},
			regular_moves(MoveName::Shadow_Ball)
		)
	});
	defender.pokemon().switch_in(environment);

	attacker.reset_start_of_turn();

	CHECK(
		legal_selections(attacker, defender, environment) ==
		LegalSelections({MoveName::Baton_Pass, MoveName::Belly_Drum, MoveName::Switch1})
	);

	{
		auto const side_effects = possible_side_effects(MoveName::Belly_Drum, attacker.pokemon().as_const(), defender, environment);
		CHECK(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		CHECK(side_effect.probability == 1.0);

		call_move(
			attacker,
			UsedMove<Team<generation>>(MoveName::Belly_Drum, side_effect.function),
			defender,
			FutureMove{false},
			environment,
			false,
			damage,
			false
		);
	}
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(!switch_decision_required(attacker));
	CHECK(
		legal_selections(attacker, defender, environment) ==
		LegalSelections({MoveName::Pass})
	);

	attacker.reset_start_of_turn();

	CHECK(
		legal_selections(attacker, defender, environment) ==
		LegalSelections({MoveName::Baton_Pass, MoveName::Belly_Drum, MoveName::Switch1})
	);

	call_move(
		attacker,
		UsedMove<Team<generation>>(MoveName::Baton_Pass, no_effect_function),
		defender,
		FutureMove{false},
		environment,
		false,
		damage,
		false
	);
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(switch_decision_required(attacker));
	CHECK(
		legal_selections(attacker, defender, environment) ==
		LegalSelections({MoveName::Switch1})
	);

	{
		auto const side_effects = possible_side_effects(MoveName::Switch1, attacker.pokemon().as_const(), defender, environment);
		CHECK(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		CHECK(side_effect.probability == 1.0);
		call_move(
			attacker,
			UsedMove<Team<generation>>(MoveName::Switch1, side_effect.function),
			defender,
			FutureMove{false},
			environment,
			false,
			damage,
			false
		);
	}
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(attacker.pokemon().species() == Species::Alakazam);
	CHECK(!switch_decision_required(attacker));
	CHECK(
		legal_selections(attacker, defender, environment) ==
		LegalSelections({MoveName::Pass})
	);
}

TEST_CASE("Wonder Guard", "[call_move]") {
	auto environment = Environment();

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Volt_Absorb,
			default_combined_stats<generation>,
			regular_moves(MoveName::Shadow_Ball, MoveName::Thunderbolt)
		)
	});
	attacker.pokemon().switch_in(environment);

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Shedinja,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Wonder_Guard,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Tackle)})
		)
	});
	auto shedinja = defender.pokemon();
	shedinja.switch_in(environment);

	CHECK(shedinja.hp().current() == 1_bi);

	call_move(
		attacker,
		UsedMove<Team<generation>>(MoveName::Thunderbolt, no_effect_function),
		defender,
		FutureMove{false},
		environment,
		false,
		damage,
		false
	);
	CHECK(shedinja.hp().current() == 1_bi);

	call_move(
		attacker,
		UsedMove<Team<generation>>(MoveName::Shadow_Ball, no_effect_function),
		defender,
		FutureMove{false},
		environment,
		false,
		damage,
		false
	);
	CHECK(shedinja.hp().current() == 0_bi);
}

TEST_CASE("Fire move thaws target", "[call_move]") {
	auto environment = Environment();

	auto attacker = Team<generation>({
		Pokemon<generation>(
			Species::Charmander,
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Blaze,
			default_combined_stats<generation>,
			regular_moves(MoveName::Ember)
		)
	});
	attacker.pokemon().switch_in(environment);

	auto defender = Team<generation>({
		Pokemon<generation>(
			Species::Vaporeon,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Water_Absorb,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Tackle)})
		)
	});
	auto vaporeon = defender.pokemon();
	vaporeon.switch_in(environment);
	vaporeon.set_status(StatusName::freeze, environment);

	CHECK(vaporeon.status().name() == StatusName::freeze);

	constexpr auto move_name = MoveName::Ember;
	auto const side_effects = possible_side_effects(move_name, attacker.pokemon().as_const(), defender, environment);
	CHECK(containers::size(side_effects) == 2_bi);

	{
		auto attacker_copy = attacker;
		auto defender_copy = defender;

		call_move(
			attacker_copy,
			UsedMove<Team<generation>>(move_name, side_effects[0_bi].function),
			defender_copy,
			FutureMove{false},
			environment,
			false,
			damage,
			false
		);

		CHECK(defender_copy.pokemon().status().name() == StatusName::clear);
	}

	call_move(
		attacker,
		UsedMove<Team<generation>>(move_name, side_effects[1_bi].function),
		defender,
		FutureMove{false},
		environment,
		false,
		damage,
		false
	);

	CHECK(vaporeon.status().name() == StatusName::burn);
}

TEST_CASE("Sleep Talk Substitute", "[call_move]") {
	auto environment = Environment();

	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Registeel,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Clear_Body,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Sleep_Talk), Move(generation, MoveName::Substitute)})
		)
	});
	user.pokemon().switch_in(environment);
	user.pokemon().set_hp(environment, 5_bi);
	user.pokemon().rest(environment, false);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Tyranitar,
			Level(100_bi),
			Gender::male,
			Item::Choice_Band,
			Ability::Sand_Stream,
			default_combined_stats<generation>,
			regular_moves(MoveName::Rock_Slide)
		)
	});
	other.pokemon().switch_in(environment);

	CHECK(user.pokemon().substitute().hp() == 0_bi);

	auto const side_effects = possible_side_effects(MoveName::Substitute, user.pokemon().as_const(), other, environment);
	CHECK(containers::size(side_effects) == 1_bi);
	call_move(
		user,
		UsedMove<Team<generation>>(
			MoveName::Sleep_Talk,
			MoveName::Substitute,
			false,
			false,
			ContactAbilityEffect::nothing,
			containers::front(side_effects).function
		),
		other,
		FutureMove{false},
		environment,
		false,
		damage,
		false
	);

	CHECK(user.pokemon().substitute().hp() == user.pokemon().hp().max() / 4_bi);
}

TEST_CASE("Static paralyzes", "[call_move]") {
	auto environment = Environment();

	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Sentret,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Run_Away,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Scratch)})
		)
	});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Electabuzz,
			Level(100_bi),
			Gender::male,
			Item::Choice_Band,
			Ability::Static,
			default_combined_stats<generation>,
			regular_moves(MoveName::Barrier)
		)
	});
	other.pokemon().switch_in(environment);

	call_move(
		user,
		UsedMove<Team<generation>>(
			MoveName::Scratch,
			MoveName::Scratch,
			false,
			false,
			ContactAbilityEffect::paralysis,
			no_effect_function
		),
		other,
		FutureMove{false},
		environment,
		false,
		damage,
		false
	);

	CHECK(user.pokemon().status().name() == StatusName::paralysis);
}

TEST_CASE("Pokemon faints after Explosion against a Substitute in later generations", "[call_move]") {
	auto environment = Environment();

	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Registeel,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Clear_Body,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Explosion)})
		),
		Pokemon<generation>(
			Species::Regice,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Clear_Body,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Explosion)})
		)
	});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Ninjask,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Speed_Boost,
			default_combined_stats<generation>,
			regular_moves(MoveName::Substitute)
		)
	});
	other.pokemon().switch_in(environment);

	{
		auto const side_effects = possible_side_effects(MoveName::Substitute, other.pokemon().as_const(), other, environment);
		CHECK(containers::size(side_effects) == 1_bi);
		call_move(
			other,
			UsedMove<Team<generation>>(
				MoveName::Substitute,
				containers::front(side_effects).function
			),
			user,
			FutureMove{false},
			environment,
			false,
			damage,
			false
		);

		CHECK(other.pokemon().substitute().hp() == other.pokemon().hp().max() / 4_bi);
	}

	auto const side_effects = possible_side_effects(MoveName::Explosion, user.pokemon().as_const(), other, environment);
	CHECK(containers::size(side_effects) == 1_bi);
	call_move(
		user,
		UsedMove<Team<generation>>(
			MoveName::Explosion,
			containers::front(side_effects).function
		),
		other,
		FutureMove{false},
		environment,
		false,
		damage,
		false
	);

	CHECK(user.pokemon().hp().current() == 0_bi);
}

TEST_CASE("Perish Song", "[call_move]") {
	auto environment = Environment();

	auto user = Team<generation>({
		Pokemon<generation>(
			Species::Misdreavus,
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Levitate,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Perish_Song)})
		),
		Pokemon<generation>(
			Species::Regice,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Clear_Body,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Explosion)})
		)
	});
	user.pokemon().switch_in(environment);

	auto other = Team<generation>({
		Pokemon<generation>(
			Species::Starmie,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Illuminate,
			default_combined_stats<generation>,
			regular_moves(MoveName::Recover)
		),
		Pokemon<generation>(
			Species::Regice,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Clear_Body,
			default_combined_stats<generation>,
			RegularMoves({Move(generation, MoveName::Explosion)})
		)
	});
	other.pokemon().switch_in(environment);

	auto call_perish_song = [&] {
		auto const side_effects = possible_side_effects(MoveName::Perish_Song, user.pokemon().as_const(), other, environment);
		CHECK(containers::size(side_effects) == 1_bi);
		call_move(
			user,
			UsedMove<Team<generation>>(
				MoveName::Perish_Song,
				containers::front(side_effects).function
			),
			other,
			FutureMove{false},
			environment,
			false,
			damage,
			false
		);
	};
	auto call_recover = [&] {
		auto const side_effects = possible_side_effects(MoveName::Recover, other.pokemon().as_const(), user, environment);
		CHECK(containers::size(side_effects) == 1_bi);
		call_move(
			other,
			UsedMove<Team<generation>>(
				MoveName::Recover,
				containers::front(side_effects).function
			),
			user,
			FutureMove{false},
			environment,
			false,
			damage,
			false
		);
	};

	call_perish_song();
	call_recover();

	auto check_max_hp = [](auto const & team) {
		CHECK(team.pokemon().hp().current() == team.pokemon().hp().max());
	};
	check_max_hp(user);
	check_max_hp(other);
}

} // namespace
} // namespace technicalmachine
