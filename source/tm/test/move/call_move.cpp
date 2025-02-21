// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.future_selection;
import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.no_effect_function;
import tm.move.pass;
import tm.move.side_effects;
import tm.move.switch_;
import tm.move.used_move;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.status.status_name;

import tm.ability;
import tm.contact_ability_effect;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.get_legal_selections;
import tm.item;
import tm.probability;
import tm.switch_decision_required;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto damage = ActualDamage::Unknown{};

TEST_CASE("Baton Pass", "[call_move]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Smeargle,
			.moves = {{
				MoveName::Baton_Pass,
				MoveName::Belly_Drum,
			}}
		},
		{
			.species = Species::Alakazam,
			.moves = {{
				MoveName::Psycho_Cut,
				MoveName::Recover,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Gengar,
			.item = Item::Choice_Specs,
			.moves = {{
				MoveName::Shadow_Ball,
			}}
		},
		{
			.species = Species::Misdreavus,
			.item = Item::Choice_Specs,
			.moves = {{
				MoveName::Shadow_Ball,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	CHECK(
		get_legal_selections(attacker, defender, environment) ==
		LegalSelections({MoveName::Baton_Pass, MoveName::Belly_Drum, Switch(1_bi)})
	);

	{
		auto const side_effects = possible_side_effects(MoveName::Belly_Drum, attacker.pokemon().as_const(), defender, environment);
		CHECK(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		CHECK(side_effect.probability == Probability(1.0));

		call_move(
			attacker,
			UsedMove<Team<generation>>(MoveName::Belly_Drum, side_effect.function),
			defender,
			FutureSelection(false),
			environment,
			false,
			damage,
			false
		);
	}
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(!switch_decision_required(attacker));
	CHECK(
		get_legal_selections(attacker, defender, environment) ==
		LegalSelections({pass})
	);

	attacker.reset_end_of_turn();

	CHECK(
		get_legal_selections(attacker, defender, environment) ==
		LegalSelections({MoveName::Baton_Pass, MoveName::Belly_Drum, Switch(1_bi)})
	);

	call_move(
		attacker,
		UsedMove<Team<generation>>(MoveName::Baton_Pass, no_effect_function),
		defender,
		FutureSelection(false),
		environment,
		false,
		damage,
		false
	);
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(switch_decision_required(attacker));
	CHECK(
		get_legal_selections(attacker, defender, environment) ==
		LegalSelections({Switch(1_bi)})
	);

	attacker.switch_pokemon(defender.pokemon(), environment, 1_bi);
	CHECK(attacker.pokemon().stages()[BoostableStat::atk] == 6_bi);
	CHECK(attacker.pokemon().species() == Species::Alakazam);
	CHECK(!switch_decision_required(attacker));
	CHECK(
		get_legal_selections(attacker, defender, environment) ==
		LegalSelections({pass})
	);
}

TEST_CASE("Wonder Guard", "[call_move]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Jolteon,
			.moves = {{
				MoveName::Shadow_Ball,
				MoveName::Thunderbolt,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Shedinja,
			.ability = Ability::Wonder_Guard,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);
	CHECK(defender.pokemon().hp().current() == 1_bi);

	{
		auto attacker_copy = attacker;
		auto defender_copy = defender;
		call_move(
			attacker_copy,
			UsedMove<Team<generation>>(MoveName::Thunderbolt, no_effect_function),
			defender_copy,
			FutureSelection(false),
			environment,
			false,
			damage,
			false
		);
		CHECK(defender_copy.pokemon().hp().current() == 1_bi);
	}
	{
		auto attacker_copy = attacker;
		auto defender_copy = defender;
		call_move(
			attacker_copy,
			UsedMove<Team<generation>>(MoveName::Shadow_Ball, no_effect_function),
			defender_copy,
			FutureSelection(false),
			environment,
			false,
			damage,
			false
		);
		CHECK(defender_copy.pokemon().hp().current() == 0_bi);
	}
}

TEST_CASE("Fire move thaws target", "[call_move]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Charmander,
			.moves = {{
				MoveName::Ember,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Vaporeon,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	auto vaporeon = defender.pokemon();
	vaporeon.switch_in(environment, true);
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
			FutureSelection(false),
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
		FutureSelection(false),
		environment,
		false,
		damage,
		false
	);

	CHECK(vaporeon.status().name() == StatusName::burn);
}

TEST_CASE("Sleep Talk Substitute", "[call_move]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Registeel,
			.moves = {{
				MoveName::Sleep_Talk,
				MoveName::Substitute,
			}}
		},
	}});
	user.pokemon().switch_in(environment, true);
	user.pokemon().set_hp(environment, 5_bi);
	user.pokemon().rest(environment, Ability::Honey_Gather, false);

	auto other = Team<generation>({{
		{
			.species = Species::Bulbasaur,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	other.pokemon().switch_in(environment, true);

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
			false,
			ContactAbilityEffect::nothing,
			containers::front(side_effects).function
		),
		other,
		FutureSelection(false),
		environment,
		false,
		damage,
		false
	);

	CHECK(user.pokemon().substitute().hp() == user.pokemon().hp().max() / 4_bi);
}

TEST_CASE("Static paralyzes", "[call_move]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Sentret,
			.moves = {{
				MoveName::Scratch,
			}}
		},
	}});
	user.pokemon().switch_in(environment, true);

	auto other = Team<generation>({{
		{
			.species = Species::Electabuzz,
			.ability = Ability::Static,
			.moves = {{
				MoveName::Scratch,
			}}
		},
	}});
	other.pokemon().switch_in(environment, true);

	call_move(
		user,
		UsedMove<Team<generation>>(
			MoveName::Scratch,
			MoveName::Scratch,
			false,
			false,
			false,
			ContactAbilityEffect::paralysis,
			no_effect_function
		),
		other,
		FutureSelection(false),
		environment,
		false,
		damage,
		false
	);

	CHECK(user.pokemon().status().name() == StatusName::paralysis);
}

TEST_CASE("Pokemon faints after Explosion against a Substitute in later generations", "[call_move]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Registeel,
			.moves = {{
				MoveName::Explosion,
			}}
		},
		{
			.species = Species::Regice,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	user.pokemon().switch_in(environment, true);

	auto other = Team<generation>({{
		{
			.species = Species::Ninjask,
			.moves = {{
				MoveName::Substitute,
			}}
		},
	}});
	other.pokemon().switch_in(environment, true);

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
			FutureSelection(false),
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
		FutureSelection(false),
		environment,
		false,
		damage,
		false
	);

	CHECK(user.pokemon().hp().current() == 0_bi);
}

TEST_CASE("Perish Song", "[call_move]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();

	auto user = Team<generation>({{
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
	}});
	user.pokemon().switch_in(environment, true);

	auto other = Team<generation>({{
		{
			.species = Species::Starmie,
			.moves = {{
				MoveName::Recover,
			}}
		},
		{
			.species = Species::Regice,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});
	other.pokemon().switch_in(environment, true);

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
			FutureSelection(false),
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
			FutureSelection(false),
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

TEST_CASE("Solar Beam", "[call_move]") {
	constexpr auto generation = Generation::four;
	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Meganium,
			.moves = {{
				MoveName::Solar_Beam,
			}}
		},
	}});
	user.pokemon().switch_in(environment, true);

	auto other = Team<generation>({{
		{
			.species = Species::Moltres,
			.moves = {{
				MoveName::Agility,
			}}
		},
	}});
	other.pokemon().switch_in(environment, true);

	auto call_solar_beam = [&] {
		constexpr auto move = MoveName::Solar_Beam;
		auto const side_effects = possible_side_effects(move, user.pokemon().as_const(), other, environment);
		CHECK(containers::size(side_effects) == 1_bi);
		call_move(
			user,
			UsedMove<Team<generation>>(
				move,
				containers::front(side_effects).function
			),
			other,
			FutureSelection(false),
			environment,
			false,
			damage,
			false
		);
	};

	CHECK(!user.pokemon().last_used_move().name());
	CHECK(!user.pokemon().last_used_move().moved_this_turn());
	CHECK(!user.pokemon().last_used_move().locked_in());

	call_solar_beam();

	CHECK(user.pokemon().last_used_move().name() == MoveName::Solar_Beam);
	CHECK(user.pokemon().last_used_move().moved_this_turn());
	CHECK(user.pokemon().last_used_move().locked_in() == MoveName::Solar_Beam);

	auto check_max_hp = [](auto const & team) {
		auto const hp = team.pokemon().hp();
		CHECK(hp.current() == hp.max());
	};
	check_max_hp(user);
	check_max_hp(other);

	user.reset_end_of_turn();

	call_solar_beam();

	CHECK(user.pokemon().last_used_move().name() == MoveName::Solar_Beam);
	CHECK(user.pokemon().last_used_move().moved_this_turn());
	CHECK(!user.pokemon().last_used_move().locked_in());

	user.reset_end_of_turn();

	call_solar_beam();

	CHECK(user.pokemon().last_used_move().name() == MoveName::Solar_Beam);
	CHECK(user.pokemon().last_used_move().moved_this_turn());
	CHECK(user.pokemon().last_used_move().locked_in() == MoveName::Solar_Beam);
}

TEST_CASE("Generation 1 Hyper Beam KO", "[call_move]") {
	constexpr auto generation = Generation::one;
	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Snorlax,
			.moves = {{
				MoveName::Hyper_Beam,
			}}
		},
	}});
	user.pokemon().switch_in(environment, true);

	auto other = Team<generation>({{
		{
			.species = Species::Chansey,
			.level = Level(40_bi),
			.moves = {{
				MoveName::Ice_Beam,
			}}
		},
		{
			.species = Species::Slowbro,
			.moves = {{
				MoveName::Ice_Beam,
			}}
		},
	}});
	other.pokemon().switch_in(environment, true);

	constexpr auto move = MoveName::Hyper_Beam;
	auto const side_effects = possible_side_effects(
		move,
		user.pokemon().as_const(),
		other,
		environment
	);
	CHECK(containers::size(side_effects) == 1_bi);
	call_move(
		user,
		UsedMove<Team<generation>>(
			move,
			containers::front(side_effects).function
		),
		other,
		FutureSelection(false),
		environment,
		false,
		damage,
		false
	);

	CHECK(other.pokemon().hp().current() == 0_bi);

	CHECK(!user.pokemon().last_used_move().locked_in());
	CHECK(!user.pokemon().recharge());
}

TEST_CASE("Generation 1 Mirror Move Hyper Beam KO", "[call_move]") {
	constexpr auto generation = Generation::one;
	auto environment = Environment();

	auto user = Team<generation>({{
		{
			.species = Species::Pidgeot,
			.moves = {{
				MoveName::Mirror_Move,
			}}
		},
	}});
	user.pokemon().switch_in(environment, true);

	auto other = Team<generation>({{
		{
			.species = Species::Chansey,
			.level = Level(40_bi),
			.moves = {{
				MoveName::Hyper_Beam,
			}}
		},
		{
			.species = Species::Slowbro,
			.moves = {{
				MoveName::Ice_Beam,
			}}
		},
	}});
	other.pokemon().switch_in(environment, true);

	constexpr auto selected = MoveName::Mirror_Move;
	constexpr auto executed = MoveName::Hyper_Beam;
	auto const side_effects = possible_side_effects(
		executed,
		user.pokemon().as_const(),
		other,
		environment
	);
	CHECK(containers::size(side_effects) == 1_bi);
	call_move(
		user,
		UsedMove<Team<generation>>(
			selected,
			executed,
			false,
			false,
			false,
			ContactAbilityEffect::nothing,
			containers::front(side_effects).function
		),
		other,
		FutureSelection(false),
		environment,
		false,
		damage,
		false
	);

	CHECK(other.pokemon().hp().current() == 0_bi);

	CHECK(!user.pokemon().last_used_move().locked_in());
	CHECK(!user.pokemon().recharge());
}

} // namespace
} // namespace technicalmachine
