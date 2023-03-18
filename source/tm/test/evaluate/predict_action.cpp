// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.evaluate.evaluate;
import tm.evaluate.evaluate_settings;
import tm.evaluate.predict_action;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;

import tm.ability;
import tm.block;
import tm.environment;
import tm.gender;
import tm.generation;
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

auto predict_action(auto const & ai, auto const & foe, Environment const environment, auto const evaluate) {
	return predict_action(
		ai,
		legal_selections(ai, foe, environment),
		foe,
		legal_selections(foe, ai, environment),
		environment,
		evaluate
	);
}

auto shuffled_regular_moves(Generation const generation, auto & random_engine, auto... ts) {
	// Random order to prevent ordering effects from accidentally arriving at
	// the correct move each time
	auto moves = RegularMoves{Move(generation, ts)...};
	std::shuffle(containers::legacy_iterator(containers::begin(moves)), containers::legacy_iterator(containers::end(moves)), random_engine);
	return moves;
}

TEST_CASE("predict_action one move", "[predict_action]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};

	auto team1 = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Volt_Absorb,
			default_combined_stats<generation>,
			regular_moves(MoveName::Thunderbolt)
		)
	}, true);
	team1.pokemon().switch_in(environment);
	team1.reset_start_of_turn();

	auto team2 = Team<generation>({
		Pokemon<generation>(
			Species::Snorlax,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Thick_Fat,
			default_combined_stats<generation>,
			regular_moves(MoveName::Snore)
		)
	});
	team2.pokemon().switch_in(environment);
	team2.reset_start_of_turn();

	auto const moves = predict_action(team1, team2, environment, evaluate);
	CHECK(moves == MoveProbabilities({MoveProbability{MoveName::Thunderbolt, 1.0}}));
}

TEST_CASE("predict_action winning and losing move", "[predict_action]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};

	auto team1 = Team<generation>({
		Pokemon<generation>(
			Species::Jolteon,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Volt_Absorb,
			default_combined_stats<generation>,
			regular_moves(MoveName::Thunderbolt, MoveName::Hidden_Power)
		)
	}, true);
	team1.pokemon().switch_in(environment);
	team1.reset_start_of_turn();

	auto team2 = Team<generation>({
		Pokemon<generation>(
			Species::Gyarados,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Intimidate,
			default_combined_stats<generation>,
			regular_moves(MoveName::Earthquake)
		)
	});
	team2.pokemon().switch_in(environment);
	team2.reset_start_of_turn();

	auto const moves = predict_action(team1, team2, environment, evaluate);
	CHECK(moves == MoveProbabilities({MoveProbability{MoveName::Thunderbolt, 1.0}}));
}

TEST_CASE("predict_action good and bad move", "[predict_action]") {
	constexpr auto generation = Generation::four;
	constexpr auto evaluate = Evaluate<generation>(evaluate_settings);
	auto const environment = Environment();
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};

	auto team1 = Team<generation>({
		Pokemon<generation>(
			Species::Suicune,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Pressure,
			default_combined_stats<generation>,
			regular_moves(MoveName::Surf, MoveName::Ice_Beam)
		)
	}, true);
	team1.pokemon().switch_in(environment);
	team1.reset_start_of_turn();

	auto team2 = Team<generation>({
		Pokemon<generation>(
			Species::Latias,
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Levitate,
			default_combined_stats<generation>,
			regular_moves(MoveName::Calm_Mind, MoveName::Dragon_Pulse)
		)
	});
	team2.pokemon().switch_in(environment);
	team2.reset_start_of_turn();

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
	auto random_engine = std::mt19937(std::random_device()());
	auto const regular_moves = [&](auto... args) {
		return shuffled_regular_moves(generation, random_engine, args...);
	};

	auto team1 = Team<generation>({
		Pokemon<generation>(
			Species::Suicune,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Pressure,
			default_combined_stats<generation>,
			regular_moves(MoveName::Surf, MoveName::Ice_Beam, MoveName::Roar)
		)
	}, true);
	team1.pokemon().switch_in(environment);
	team1.reset_start_of_turn();

	auto team2 = Team<generation>({
		Pokemon<generation>(
			Species::Latias,
			Level(100_bi),
			Gender::female,
			Item::None,
			Ability::Levitate,
			default_combined_stats<generation>,
			regular_moves(MoveName::Calm_Mind, MoveName::Dragon_Pulse)
		)
	});
	team2.pokemon().switch_in(environment);
	team2.reset_start_of_turn();

	auto const moves = predict_action(team1, team2, environment, evaluate);
	auto const ptr = containers::maybe_find_if(moves, [](MoveProbability const move) {
		return move.name == MoveName::Ice_Beam;
	});
	REQUIRE(ptr);
	CHECK(ptr->probability > 0.9);
}

} // namespace
} // namespace technicalmachine
