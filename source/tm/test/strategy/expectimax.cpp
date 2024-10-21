// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.evaluate.all_evaluate;
import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.scored_selection;
import tm.evaluate.state;
import tm.evaluate.victory;

import tm.move.actual_damage;
import tm.move.call_move;
import tm.move.future_selection;
import tm.move.move_name;
import tm.move.no_effect_function;
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

import tm.status.status_name;

import tm.strategy.expectimax;
import tm.strategy.random_selection;
import tm.strategy.strategy;
import tm.strategy.weighted_selection;

import tm.ability;
import tm.end_of_turn;
import tm.end_of_turn_flags;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.get_legal_selections;
import tm.item;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto make_strategy(DepthInt const depth) -> Strategy {
	return make_expectimax(
		AllEvaluate(),
		Depth(depth, 0_bi),
		make_random_selection()
	);
}

template<Generation generation>
auto determine_best_selection(Strategy const & strategy, Team<generation> const & ai, Team<generation> const & foe, Environment const environment) -> WeightedSelection {
	auto const moves = strategy(
		ai,
		get_legal_selections(ai, foe, environment),
		foe,
		get_legal_selections(foe, ai, environment),
		environment
	).user;
	return *containers::max_element(moves, [](WeightedSelection const lhs, WeightedSelection const rhs) {
		return lhs.weight > rhs.weight;
	});
}

TEST_CASE("expectimax OHKO", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(1_bi);
	auto const environment = Environment();

	auto team1 = Team<generation>({{
		{
			.species = Species::Jolteon,
			.item = Item::Leftovers,
			.ability = Ability::Volt_Absorb,
			.moves = {{
				MoveName::Thunderbolt,
				MoveName::Charm,
				MoveName::Thunder,
				MoveName::Shadow_Ball,
			}}
		},
	}});
	team1.pokemon().switch_in(environment, true);

	auto team2 = Team<generation>({{
		{
			.species = Species::Gyarados,
			.item = Item::Leftovers,
			.ability = Ability::Intimidate,
			.moves = {{
				MoveName::Dragon_Dance,
				MoveName::Waterfall,
				MoveName::Stone_Edge,
				MoveName::Taunt,
			}}
		},
	}});
	team2.pokemon().switch_in(environment, true);

	{
		auto const best = determine_best_selection(strategy, team1, team2, environment);
		CHECK(best.selection == MoveName::Thunderbolt);
		CHECK(best.weight == 1.0);
	}
	
	auto team3 = Team<generation>({{
		{
			.species = Species::Shedinja,
			.item = Item::Lum_Berry,
			.ability = Ability::Wonder_Guard,
			.moves = {{
				MoveName::Swords_Dance,
				MoveName::X_Scissor,
				MoveName::Shadow_Sneak,
				MoveName::Will_O_Wisp,
			}}
		},
	}});
	team3.pokemon().switch_in(environment, true);
	
	{
		auto const best = determine_best_selection(strategy, team1, team3, environment);
		CHECK(best.selection == MoveName::Shadow_Ball);
		CHECK(best.weight == 1.0);
	}
}

TEST_CASE("expectimax one-turn damage", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(1_bi);
	auto const environment = Environment();
	
	auto attacker = Team<generation>({{
		{
			.species = Species::Jolteon,
			.item = Item::Leftovers,
			.ability = Ability::Volt_Absorb,
			.moves = {{
				MoveName::Thunderbolt,
				MoveName::Charm,
				MoveName::Thunder,
				MoveName::Shadow_Ball,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Swampert,
			.item = Item::Leftovers,
			.ability = Ability::Torrent,
			.moves = {{
				MoveName::Surf,
				MoveName::Ice_Beam,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	auto const best = determine_best_selection(strategy, attacker, defender, environment);
	CHECK(best.selection == MoveName::Shadow_Ball);
}

TEST_CASE("expectimax BellyZard", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(2_bi);
	auto const environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Charizard,
			.item = Item::Salac_Berry,
			.ability = Ability::Blaze,
			.moves = {{
				MoveName::Fire_Punch,
				MoveName::Belly_Drum,
				MoveName::Earthquake,
				MoveName::Double_Edge,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Mew,
			.item = Item::Leftovers,
			.ability = Ability::Synchronize,
			.moves = {{
				MoveName::Soft_Boiled,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	auto const best = determine_best_selection(strategy, attacker, defender, environment);
	CHECK(best.selection == MoveName::Belly_Drum);
	CHECK(best.weight == 1.0);
}

TEST_CASE("expectimax Hippopotas vs Wobbuffet", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(11_bi);
	auto const environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Hippopotas,
			.item = Item::Leftovers,
			.ability = Ability::Sand_Stream,
			.stats = {
				.nature = Nature::Adamant,
				.evs = EVs(
					EV(0_bi),
					EV(252_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi)
				),
			},
			.moves = {{
				MoveName::Curse,
				MoveName::Crunch,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	// TODO: Implement Encore's effect ending when PP runs out, then Wobbuffet
	// can have Encore

	auto defender = Team<generation>({{
		{
			.species = Species::Wobbuffet,
			.item = Item::Leftovers,
			.ability = Ability::Shadow_Tag,
			.stats = {
				.nature = Nature::Hardy,
				.evs = EVs(
					EV(0_bi),
					EV(0_bi),
					EV(252_bi),
					EV(0_bi),
					EV(0_bi),
					EV(0_bi)
				),
			},
			.moves = {{
				MoveName::Counter,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	auto const best = determine_best_selection(strategy, attacker, defender, environment);
	CHECK(best.selection == MoveName::Curse);
	CHECK(best.weight == 1.0);
}


TEST_CASE("expectimax Baton Pass middle of turn other has moved", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(1_bi);
	auto environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Smeargle,
			.item = Item::Leftovers,
			.ability = Ability::Own_Tempo,
			.moves = {{
				MoveName::Baton_Pass,
				MoveName::Belly_Drum,
			}}
		},
		{
			.species = Species::Alakazam,
			.item = Item::Lum_Berry,
			.ability = Ability::Synchronize,
			.moves = {{
				MoveName::Psycho_Cut,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Misdreavus,
			.item = Item::Choice_Specs,
			.ability = Ability::Levitate,
			.moves = {{
				MoveName::Shadow_Ball,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	{
		constexpr auto move_name = MoveName::Shadow_Ball;
		auto const side_effects = possible_side_effects(move_name, defender.pokemon().as_const(), attacker, environment);
		REQUIRE(!containers::is_empty(side_effects));
		call_move(
			defender,
			UsedMove<Team<generation>>(
				move_name,
				containers::front(side_effects).function
			),
			attacker,
			FutureSelection(false),
			environment,
			false,
			ActualDamage::Unknown{},
			false
		);
	}
	{
		constexpr auto move_name = MoveName::Baton_Pass;
		auto const side_effects = possible_side_effects(move_name, attacker.pokemon().as_const(), defender, environment);
		REQUIRE(containers::size(side_effects) == 1_bi);
		call_move(
			attacker,
			UsedMove<Team<generation>>(
				move_name,
				containers::front(side_effects).function
			),
			defender,
			FutureSelection(false),
			environment,
			false,
			ActualDamage::Unknown{},
			false
		);
	}

	auto const best = determine_best_selection(strategy, attacker, defender, environment);
	CHECK(best.selection == Switch(1_bi));
}

TEST_CASE("expectimax Baton Pass middle of turn other has not moved", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(1_bi);
	auto environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Smeargle,
			.item = Item::Leftovers,
			.ability = Ability::Own_Tempo,
			.moves = {{
				MoveName::Baton_Pass,
				MoveName::Belly_Drum,
			}}
		},
		{
			.species = Species::Alakazam,
			.item = Item::Lum_Berry,
			.ability = Ability::Synchronize,
			.moves = {{
				MoveName::Psycho_Cut,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Misdreavus,
			.item = Item::Iron_Ball,
			.ability = Ability::Levitate,
			.moves = {{
				MoveName::Shadow_Ball,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	{
		constexpr auto move_name = MoveName::Baton_Pass;
		auto const side_effects = possible_side_effects(move_name, attacker.pokemon().as_const(), defender, environment);
		REQUIRE(containers::size(side_effects) == 1_bi);
		call_move(
			attacker,
			UsedMove<Team<generation>>(
				move_name,
				containers::front(side_effects).function
			),
			defender,
			FutureSelection(false),
			environment,
			false,
			ActualDamage::Unknown{},
			false
		);
	}

	auto const best = determine_best_selection(strategy, attacker, defender, environment);
	CHECK(best.selection == Switch(1_bi));
}


TEST_CASE("expectimax Baton Pass start of turn", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(3_bi);
	auto const environment = Environment();

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
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Misdreavus,
			.item = Item::Choice_Specs,
			.moves = {{
				MoveName::Shadow_Ball,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	auto const best = determine_best_selection(strategy, attacker, defender, environment);
	CHECK(best.selection == MoveName::Belly_Drum);
	CHECK(best.weight == 1.0);
}


TEST_CASE("expectimax replace fainted", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(2_bi);
	auto environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Magikarp,
			.level = Level(5_bi),
			.moves = {{
				MoveName::Tackle,
			}}
		},
		{
			.species = Species::Slugma,
			.moves = {{
				MoveName::Flamethrower,
				MoveName::Earth_Power,
			}}
		},
		{
			.species = Species::Zapdos,
			.item = Item::Choice_Specs,
			.stats = {
				.nature = Nature::Modest,
			},
			.moves = {{
				MoveName::Thunderbolt,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Suicune,
			.item = Item::Leftovers,
			.ability = Ability::Pressure,
			.moves = {{
				MoveName::Calm_Mind,
				MoveName::Surf,
				MoveName::Ice_Beam,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	{
		constexpr auto move_name = MoveName::Surf;
		auto const side_effects = possible_side_effects(move_name, defender.pokemon().as_const(), attacker, environment);
		REQUIRE(containers::size(side_effects) == 1_bi);
		auto const & side_effect = containers::front(side_effects);
		call_move(
			defender,
			UsedMove<Team<generation>>(
				move_name,
				side_effect.function
			),
			attacker,
			FutureSelection(false),
			environment,
			false,
			ActualDamage::Unknown{},
			false
		);
	}

	attacker.reset_end_of_turn();
	defender.reset_end_of_turn();

	auto const best = determine_best_selection(strategy, attacker, defender, environment);
	CHECK(best.selection == Switch(2_bi));
}


TEST_CASE("expectimax Latias vs Suicune", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(3_bi);
	auto const environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Latias,
			.item = Item::Leftovers,
			.ability = Ability::Levitate,
			.stats = {
				.nature = Nature::Calm,
				.evs = EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(120_bi),
					EV(136_bi),
					EV(0_bi)
				),
			},
			.moves = {{
				MoveName::Calm_Mind,
				MoveName::Dragon_Pulse,
				MoveName::Recover,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Suicune,
			.item = Item::Leftovers,
			.ability = Ability::Pressure,
			.stats = {
				.nature = Nature::Calm,
				.evs = EVs(
					EV(0_bi),
					EV(0_bi),
					EV(0_bi),
					EV(120_bi),
					EV(136_bi),
					EV(0_bi)
				),
			},
			.moves = {{
				MoveName::Ice_Beam,
				MoveName::Rest,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	auto const best = determine_best_selection(strategy, attacker, defender, environment);
	CHECK(best.selection == MoveName::Calm_Mind);
}

TEST_CASE("expectimax Sleep Talk", "[expectimax]") {
	constexpr auto generation = Generation::four;
	auto const strategy = make_strategy(1_bi);
	auto environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Jolteon,
			.item = Item::Leftovers,
			.ability = Ability::Volt_Absorb,
			.moves = {{
				MoveName::Sleep_Talk,
				MoveName::Thunderbolt,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Gyarados,
			.item = Item::Life_Orb,
			.ability = Ability::Intimidate,
			.moves = {{
				MoveName::Earthquake,
			}}
		},
	}});
	defender.pokemon().switch_in(environment, true);

	constexpr auto keep_status = false;
	constexpr auto unknown_damage = ActualDamage::Unknown{};
	constexpr auto sleep_talk = UsedMove<Team<generation>>(
		MoveName::Sleep_Talk,
		no_effect_function
	);
	constexpr auto thunderbolt = UsedMove<Team<generation>>(
		MoveName::Thunderbolt,
		no_effect_function
	);
	constexpr auto other_action = FutureSelection(false);

	auto next_turn = [&] {
		constexpr auto end_of_turn_flags = EndOfTurnFlags(false, false, false);
		end_of_turn(attacker, end_of_turn_flags, defender, end_of_turn_flags, environment);
	};

	auto jolteon = attacker.pokemon();

	// TODO: Validate score, too

	CHECK(jolteon.status().name() == StatusName::clear);
	CHECK(determine_best_selection(strategy, attacker, defender, environment).selection == MoveName::Thunderbolt);

	call_move(
		attacker,
		sleep_talk,
		defender,
		other_action,
		environment,
		keep_status,
		unknown_damage,
		false
	);
	jolteon.set_status(StatusName::sleep, environment);
	next_turn();
	CHECK(jolteon.status().name() == StatusName::sleep);
	CHECK(determine_best_selection(strategy, attacker, defender, environment).selection == MoveName::Sleep_Talk);

	call_move(
		attacker,
		thunderbolt,
		defender,
		other_action,
		environment,
		keep_status,
		unknown_damage,
		false
	);
	next_turn();
	CHECK(jolteon.status().name() == StatusName::sleep);
	CHECK(determine_best_selection(strategy, attacker, defender, environment).selection == MoveName::Sleep_Talk);

	call_move(
		attacker,
		thunderbolt,
		defender,
		other_action,
		environment,
		keep_status,
		unknown_damage,
		false
	);
	next_turn();
	CHECK(jolteon.status().name() == StatusName::sleep);
	CHECK(determine_best_selection(strategy, attacker, defender, environment).selection == MoveName::Sleep_Talk);

	#if 0
		// Same probability of either move
		call_move(
			attacker,
			thunderbolt,
			defender,
			other_action,
			environment,
			keep_status,
			unknown_damage,
			false
		);
		next_turn();
		CHECK(jolteon.status().name() == StatusName::sleep);
		CHECK(determine_best_selection(strategy, attacker, defender, environment).selection == ?);
	#endif
}

TEST_CASE("Generation 1 frozen last Pokemon", "[expectimax]") {
	constexpr auto generation = Generation::one;
	auto environment = Environment();

	auto attacker = Team<generation>({{
		{
			.species = Species::Alakazam,
			.moves = {{
				MoveName::Toxic,
				MoveName::Psychic,
				MoveName::Recover,
			}}
		},
	}});
	attacker.pokemon().switch_in(environment, true);

	auto defender = Team<generation>({{
		{
			.species = Species::Gengar,
			.moves = {{
				MoveName::Thunderbolt,
			}}
		},
	}});
	defender.pokemon().set_status(StatusName::freeze, environment);
	defender.pokemon().set_hp(environment, 12_bi);
	defender.pokemon().switch_in(environment, true);

	CHECK(determine_best_selection(make_strategy(1_bi), attacker, defender, environment).selection == MoveName::Psychic);
	CHECK(determine_best_selection(make_strategy(2_bi), attacker, defender, environment).selection == MoveName::Psychic);
}

} // namespace
} // namespace technicalmachine
