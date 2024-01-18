// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <iostream>
#include <catch2/catch_test_macros.hpp>

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluate_settings;
import tm.evaluate.move_probability;
import tm.evaluate.predict_action;
import tm.evaluate.state;

import tm.move.move_name;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.ability;
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

constexpr auto evaluate_settings = EvaluateSettings{
	.hp = 1024_bi,
	.hidden = 80_bi,
	.spikes = -150_bi,
	.stealth_rock = -200_bi,
	.toxic_spikes = -100_bi
};

template<Generation generation>
auto predict_action(Team<generation> const & ai, Team<generation> const & foe, Environment const environment, Evaluate<generation> const evaluate) {
	return predict_action(
		ai,
		get_legal_selections(ai, foe, environment),
		foe,
		get_legal_selections(foe, ai, environment),
		environment,
		evaluate,
		Depth(1_bi, 1_bi)
	);
}


TEST_CASE("predict_action one move", "[predict_action]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();

	auto team1 = Team<generation>({{
		{
			.species = Species::Jolteon,
			.ability = Ability::Volt_Absorb,
			.moves = {{
				MoveName::Thunderbolt,
			}}
		},
	}});
	team1.pokemon().switch_in(environment);

	auto team2 = Team<generation>({{
		{
			.species = Species::Snorlax,
			.ability = Ability::Thick_Fat,
			.moves = {{
				MoveName::Snore,
			}}
		},
	}});
	team2.pokemon().switch_in(environment);

	auto const moves = predict_action(team1, team2, environment, evaluate);
	CHECK(moves == MoveProbabilities({MoveProbability{MoveName::Thunderbolt, 1.0}}));
}

TEST_CASE("predict_action winning and losing move", "[predict_action]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();

	auto team1 = Team<generation>({{
		{
			.species = Species::Jolteon,
			.ability = Ability::Volt_Absorb,
			.moves = {{
				MoveName::Thunderbolt,
				MoveName::Hidden_Power,
			}}
		},
	}});
	team1.pokemon().switch_in(environment);

	auto team2 = Team<generation>({{
		{
			.species = Species::Gyarados,
			.ability = Ability::Intimidate,
			.moves = {{
				MoveName::Earthquake,
			}}
		},
	}});
	team2.pokemon().switch_in(environment);

	auto const moves = predict_action(team1, team2, environment, evaluate);
	CHECK(moves == MoveProbabilities({MoveProbability{MoveName::Thunderbolt, 1.0}}));
}

TEST_CASE("predict_action good and bad move", "[predict_action]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();

	auto team1 = Team<generation>({{
		{
			.species = Species::Suicune,
			.ability = Ability::Pressure,
			.moves = {{
				MoveName::Surf,
				MoveName::Ice_Beam,
			}}
		},
	}});
	team1.pokemon().switch_in(environment);

	auto team2 = Team<generation>({{
		{
			.species = Species::Latias,
			.ability = Ability::Levitate,
			.moves = {{
				MoveName::Calm_Mind,
				MoveName::Dragon_Pulse,
			}}
		},
	}});
	team2.pokemon().switch_in(environment);

	auto const moves = predict_action(team1, team2, environment, evaluate);
	auto const ptr = containers::maybe_find_if(moves, [](MoveProbability const move) {
		return move.name == MoveName::Ice_Beam;
	});
	REQUIRE(ptr);
	CHECK(ptr->probability > 0.9);
}

TEST_CASE("predict_action good bad and useless move", "[predict_action]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();

	auto team1 = Team<generation>({{
		{
			.species = Species::Suicune,
			.ability = Ability::Pressure,
			.moves = {{
				MoveName::Surf,
				MoveName::Ice_Beam,
				MoveName::Roar,
			}}
		},
	}});
	team1.pokemon().switch_in(environment);

	auto team2 = Team<generation>({{
		{
			.species = Species::Latias,
			.ability = Ability::Levitate,
			.moves = {{
				MoveName::Calm_Mind,
				MoveName::Dragon_Pulse,
			}}
		},
	}});
	team2.pokemon().switch_in(environment);

	auto const moves = predict_action(team1, team2, environment, evaluate);
	auto const ptr = containers::maybe_find_if(moves, [](MoveProbability const move) {
		return move.name == MoveName::Ice_Beam;
	});
	REQUIRE(ptr);
	CHECK(ptr->probability > 0.9);
}

TEST_CASE("Magneton vs Skarmory big team", "[predict_action]") {
	constexpr auto generation = Generation::three;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();

	auto team1 = Team<generation>({{
		{
			.species = Species::Magneton,
			.ability = Ability::Magnet_Pull,
			.moves = {{
				MoveName::Thunderbolt,
				MoveName::Substitute,
			}}
		},
		{
			.species = Species::Zapdos,
			.ability = Ability::Pressure,
			.moves = {{
				MoveName::Thunderbolt,
				MoveName::Substitute,
			}}
		},
		{
			.species = Species::Snorlax,
			.ability = Ability::Thick_Fat,
			.moves = {{
				MoveName::Curse,
				MoveName::Body_Slam,
			}}
		},
	}});
	team1.pokemon().switch_in(environment);

	auto team2 = Team<generation>({{
		{
			.species = Species::Skarmory,
			.ability = Ability::Sturdy,
			.moves = {{
				MoveName::Spikes,
				MoveName::Drill_Peck,
				MoveName::Roar,
			}}
		},
		{
			.species = Species::Blissey,
			.ability = Ability::Natural_Cure,
			.moves = {{
				MoveName::Soft_Boiled,
				MoveName::Seismic_Toss,
			}}
		},
		{
			.species = Species::Suicune,
			.ability = Ability::Pressure,
			.moves = {{
				MoveName::Surf,
				MoveName::Ice_Beam,
				MoveName::Calm_Mind,
				MoveName::Roar,
			}}
		},
	}});
	team2.pokemon().switch_in(environment);

	auto const moves = predict_action(team1, team2, environment, evaluate);
	auto const ptr = containers::maybe_find_if(moves, [](MoveProbability const move) {
		return move.name == MoveName::Thunderbolt;
	});
	REQUIRE(ptr);
	CHECK(ptr->probability > 0.9);
}


} // namespace
} // namespace technicalmachine
