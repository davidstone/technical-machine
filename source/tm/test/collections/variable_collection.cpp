// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

#include <bounded/assert.hpp>

export module tm.test.collections.variable_collection;

import tm.pokemon.pokemon_collection;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;
import tm.move.side_effects;

import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.test.pokemon_init;

import tm.ability;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

using EffectIndex = bounded::integer<0, 4>;
template<Generation generation>
constexpr auto validate(Team<generation> const & team, EffectIndex const effect_index, TeamIndex const current_index) -> void {
	static constexpr auto expected_index = containers::array<bounded::integer<0, 5>, 6_bi, 5_bi>{{
		{1_bi, 2_bi, 3_bi, 4_bi, 5_bi},
		{0_bi, 2_bi, 3_bi, 4_bi, 5_bi},
		{0_bi, 1_bi, 3_bi, 4_bi, 5_bi},
		{0_bi, 1_bi, 2_bi, 4_bi, 5_bi},
		{0_bi, 1_bi, 2_bi, 3_bi, 5_bi},
		{0_bi, 1_bi, 2_bi, 3_bi, 4_bi}
	}};

	auto const expected = expected_index[current_index][effect_index];
	auto const calculated = team.all_pokemon().index();
	BOUNDED_ASSERT(expected == calculated);
}

constexpr auto test_phaze(Team<generation> user, Team<generation> team) -> bool {
	auto environment = Environment();
	for (auto const current_index : containers::integer_range(team.size())) {
		team.all_pokemon().set_index(current_index);
		auto const side_effects = possible_side_effects(MoveName::Whirlwind, user.pokemon().as_const(), team, environment);
		auto const expected_size = bounded::increase_min<0>(team.size() - 1_bi);
		BOUNDED_ASSERT(containers::size(side_effects) == expected_size);
		for (auto const effect_index : containers::integer_range(expected_size)) {
			auto const & side_effect = side_effects[effect_index];
			BOUNDED_ASSERT(side_effect.probability == 1.0 / double(team.size() - 1_bi));
			team.all_pokemon().set_index(current_index);
			side_effect.function(user, team, environment, 0_bi);
			validate(team, EffectIndex(effect_index), current_index);
		}
	}
	return true;
}

TEST_CASE("Phaze against 2 Pokemon", "[Side Effect]") {
	CHECK(test_phaze(
		make_team<generation>({
			{.species = Species::Lugia},
		}),
		make_team<generation>({
			{.species = Species::Bulbasaur},
			{.species = Species::Ivysaur},
		})
	));
}
TEST_CASE("Phaze against 3 Pokemon", "[Side Effect]") {
	CHECK(test_phaze(
		make_team<generation>({
			{.species = Species::Lugia},
		}),
		make_team<generation>({
			{.species = Species::Bulbasaur},
			{.species = Species::Ivysaur},
			{.species = Species::Venusaur},
		})
	));
}
TEST_CASE("Phaze against 4 Pokemon", "[Side Effect]") {
	CHECK(test_phaze(
		make_team<generation>({
			{.species = Species::Lugia},
		}),
		make_team<generation>({
			{.species = Species::Bulbasaur},
			{.species = Species::Ivysaur},
			{.species = Species::Venusaur},
			{.species = Species::Charmander},
		})
	));
}
TEST_CASE("Phaze against 5 Pokemon", "[Side Effect]") {
	CHECK(test_phaze(
		make_team<generation>({
			{.species = Species::Lugia},
		}),
		make_team<generation>({
			{.species = Species::Bulbasaur},
			{.species = Species::Ivysaur},
			{.species = Species::Venusaur},
			{.species = Species::Charmander},
			{.species = Species::Charmeleon},
		})
	));
}
TEST_CASE("Phaze against 6 Pokemon", "[Side Effect]") {
	CHECK(test_phaze(
		make_team<generation>({
			{.species = Species::Lugia},
		}),
		make_team<generation>({
			{.species = Species::Bulbasaur},
			{.species = Species::Ivysaur},
			{.species = Species::Venusaur},
			{.species = Species::Charmander},
			{.species = Species::Charmeleon},
			{.species = Species::Charizard},
		})
	));
}

} // namespace technicalmachine
