// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluate_settings;
import tm.evaluate.expectiminimax;
import tm.evaluate.predict_selection;
import tm.evaluate.scored_selection;
import tm.evaluate.score_selections;
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


import tm.move.legal_selections;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto evaluate_settings = EvaluateSettings{
	.hp = 1024_bi,
	.hidden = 80_bi,
	.spikes = -150_bi,
	.stealth_rock = -200_bi,
	.toxic_spikes = -100_bi
};

constexpr auto make_depth(DepthInt const depth) {
	return Depth(depth, 0_bi);
}

template<Generation generation>
auto determine_best_selection(Team<generation> const & ai, Team<generation> const & foe, Environment const environment, Evaluate<generation> const evaluate, Depth const depth) {
	auto const moves = expectiminimax(
		State<generation>(ai, foe, environment, depth),
		get_legal_selections(ai, foe, environment),
		get_legal_selections(foe, ai, environment),
		evaluate
	);
	return *containers::max_element(moves, [](ScoredSelection const lhs, ScoredSelection const rhs) {
		return lhs.score > rhs.score;
	});
}

TEST_CASE("expectiminimax OHKO", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(1_bi);

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
		auto const best = determine_best_selection(team1, team2, environment, evaluate, depth);
		CHECK(best.selection == MoveName::Thunderbolt);
		CHECK(best.score == victory<generation>);
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
		auto const best = determine_best_selection(team1, team3, environment, evaluate, depth);
		CHECK(best.selection == MoveName::Shadow_Ball);
		CHECK(best.score == victory<generation>);
	}
}

TEST_CASE("expectiminimax one-turn damage", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(1_bi);
	
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

	auto const best = determine_best_selection(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Shadow_Ball);
}

TEST_CASE("expectiminimax BellyZard", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(2_bi);

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

	auto const best = determine_best_selection(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Belly_Drum);
	CHECK(best.score == victory<generation>);
}

TEST_CASE("expectiminimax Hippopotas vs Wobbuffet", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(11_bi);

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

	auto const best = determine_best_selection(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Curse);
	CHECK(best.score == victory<generation>);
}


TEST_CASE("expectiminimax Baton Pass middle of turn", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto environment = Environment();
	constexpr auto depth = Depth(1_bi, 0_bi);

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

	auto const best = determine_best_selection(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == Switch(1_bi));
}


TEST_CASE("expectiminimax Baton Pass start of turn", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = Depth(3_bi, 0_bi);

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

	auto const best = determine_best_selection(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Belly_Drum);
	CHECK(best.score == victory<generation>);
}


TEST_CASE("expectiminimax replace fainted", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto environment = Environment();
	constexpr auto depth = make_depth(2_bi);

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

	auto const best = determine_best_selection(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == Switch(2_bi));
}


TEST_CASE("expectiminimax Latias vs Suicune", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(3_bi);

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

	auto const best = determine_best_selection(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Calm_Mind);
}

TEST_CASE("expectiminimax Sleep Talk", "[expectiminimax]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto environment = Environment();
	constexpr auto depth = make_depth(1_bi);

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
	CHECK(determine_best_selection(attacker, defender, environment, evaluate, depth).selection == MoveName::Thunderbolt);

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
	CHECK(determine_best_selection(attacker, defender, environment, evaluate, depth).selection == MoveName::Sleep_Talk);

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
	CHECK(determine_best_selection(attacker, defender, environment, evaluate, depth).selection == MoveName::Sleep_Talk);

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
	CHECK(determine_best_selection(attacker, defender, environment, evaluate, depth).selection == MoveName::Sleep_Talk);

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
		CHECK(determine_best_selection(attacker, defender, environment, evaluate, depth).selection == ?);
	#endif
}

TEST_CASE("Generation 1 frozen last Pokemon", "[expectiminimax]") {
	constexpr auto generation = Generation::one;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
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

	CHECK(determine_best_selection(attacker, defender, environment, evaluate, make_depth(1_bi)).selection == MoveName::Psychic);
	CHECK(determine_best_selection(attacker, defender, environment, evaluate, make_depth(2_bi)).selection == MoveName::Psychic);
}

template<Generation generation>
auto determine_best_selection2(Team<generation> const & ai, Team<generation> const & foe, Environment const environment, Evaluate<generation> const evaluate, Depth const depth) {
	auto const ai_selections = get_legal_selections(ai, foe, environment);
	auto const foe_selections = get_legal_selections(foe, ai, environment);
	auto const selections = score_selections(
		State<generation>(ai, foe, environment, depth),
		ai_selections,
		predict_selection(
			foe,
			foe_selections,
			ai,
			ai_selections,
			environment,
			evaluate,
			Depth(1_bi, 1_bi)
		),
		evaluate
	);
	return *containers::max_element(selections, [](ScoredSelection const lhs, ScoredSelection const rhs) {
		return lhs.score > rhs.score;
	});
}

TEST_CASE("Pokemon might get locked into move and faint in same turn", "[expectiminimax]") {
	constexpr auto generation = Generation::one;
	constexpr auto evaluate = Evaluate<generation>({
		0_bi,
		0_bi,
		0_bi,
		0_bi,
		0_bi
	});
	auto environment = Environment();
	auto ai = Team<generation>({{
		{
			.species = Species::Tauros,
			.moves = {{
				MoveName::Hyper_Beam,
			}}
		},
		{
			.species = Species::Magikarp,
			.moves = {{
				MoveName::Splash,
			}}
		},
	}});
	ai.pokemon().switch_in(environment, true);

	auto foe = Team<generation>({{
		{
			.species = Species::Snorlax,
			.moves = {{
				MoveName::Body_Slam,
			}}
		},
	}});
	foe.pokemon().switch_in(environment, true);
	
	ai.pokemon().set_hp(environment, 52_bi);

	constexpr auto ai_selections = LegalSelections({MoveName::Hyper_Beam});
	constexpr auto foe_selections = LegalSelections({MoveName::Body_Slam});

	expectiminimax(
		State<generation>(foe, ai, environment, Depth(1_bi, 1_bi)),
		foe_selections,
		ai_selections,
		evaluate
	);
}

TEST_CASE("expectiminimax OHKO", "[score_selections]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(1_bi);

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
		auto const best = determine_best_selection2(team1, team2, environment, evaluate, depth);
		CHECK(best.selection == MoveName::Thunderbolt);
		CHECK(best.score == Catch::Approx(victory<generation>));
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
		auto const best = determine_best_selection2(team1, team3, environment, evaluate, depth);
		CHECK(best.selection == MoveName::Shadow_Ball);
		CHECK(best.score == victory<generation>);
	}
}

TEST_CASE("expectiminimax one-turn damage", "[score_selections]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(1_bi);
	
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

	auto const best = determine_best_selection2(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Shadow_Ball);
}

TEST_CASE("expectiminimax BellyZard", "[score_selections]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(2_bi);

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

	auto const best = determine_best_selection2(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Belly_Drum);
	CHECK(best.score == victory<generation>);
}

TEST_CASE("expectiminimax Hippopotas vs Wobbuffet", "[score_selections]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(11_bi);

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

	auto const best = determine_best_selection2(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Curse);
	CHECK(best.score == victory<generation> + 5.0);
}


TEST_CASE("expectiminimax Baton Pass", "[score_selections]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = Depth(3_bi, 0_bi);

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

	auto const best = determine_best_selection2(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Belly_Drum);
	CHECK(best.score == victory<generation>);
}


TEST_CASE("expectiminimax replace fainted", "[score_selections]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto environment = Environment();
	constexpr auto depth = make_depth(2_bi);

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

	auto const best = determine_best_selection2(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == Switch(2_bi));
	CHECK(best.score == victory<generation>);
}


TEST_CASE("expectiminimax Latias vs Suicune", "[score_selections]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	constexpr auto depth = make_depth(3_bi);

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

	auto const best = determine_best_selection2(attacker, defender, environment, evaluate, depth);
	CHECK(best.selection == MoveName::Calm_Mind);
}

TEST_CASE("expectiminimax Sleep Talk", "[score_selections]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto environment = Environment();
	constexpr auto depth = make_depth(1_bi);

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
	CHECK(determine_best_selection2(attacker, defender, environment, evaluate, depth).selection == MoveName::Thunderbolt);

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
	CHECK(determine_best_selection2(attacker, defender, environment, evaluate, depth).selection == MoveName::Sleep_Talk);

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
	CHECK(determine_best_selection2(attacker, defender, environment, evaluate, depth).selection == MoveName::Sleep_Talk);

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
	CHECK(determine_best_selection2(attacker, defender, environment, evaluate, depth).selection == MoveName::Sleep_Talk);

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
		CHECK(determine_best_selection2(attacker, defender, environment, evaluate, depth).selection == ?);
	#endif
}

TEST_CASE("Generation 1 frozen last Pokemon", "[score_selections]") {
	constexpr auto generation = Generation::one;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
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

	CHECK(determine_best_selection2(attacker, defender, environment, evaluate, make_depth(1_bi)).selection == MoveName::Psychic);
	CHECK(determine_best_selection2(attacker, defender, environment, evaluate, make_depth(2_bi)).selection == MoveName::Psychic);
}

} // namespace
} // namespace technicalmachine
