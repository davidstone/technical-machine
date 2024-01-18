// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

#include <bounded/assert.hpp>

export module tm.test.string_conversion;

import tm.move.initial_move;
import tm.move.move;
import tm.move.move_name;
import tm.move.move_names;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.pokemon_from_string;
import tm.string_conversions.pokemon_to_string;
import tm.string_conversions.species;
import tm.string_conversions.status_name;
import tm.string_conversions.type;
import tm.string_conversions.weather;

import tm.test.pokemon_init;

import tm.type.type;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.item;
import tm.weather;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

template<typename Enum>
constexpr auto test_generic() -> bool {
	for (auto const original : containers::enum_range<Enum>()) {
		auto const str = to_string(original);
		auto const result = from_string<Enum>(str);
		BOUNDED_ASSERT(original == result);
	}
	return true;
}

TEST_CASE("ability", "[string_conversion]") {
	test_generic<Ability>();
}
static_assert(test_generic<Gender>());
TEST_CASE("item", "[string_conversion]") {
	test_generic<Item>();
}
TEST_CASE("move", "[string_conversion]") {
	test_generic<MoveName>();
}
static_assert(test_generic<Nature>());
TEST_CASE("species", "[string_conversion]") {
	test_generic<Species>();
}
static_assert(test_generic<StatusName>());
static_assert(test_generic<Type>());
static_assert(test_generic<Weather>());

TEST_CASE("pokemon", "[string_conversion]") {
	constexpr auto generation = Generation::three;

	constexpr auto check = [](InitialMoves const moves, StatusName const status) {
		auto pokemon = make_pokemon<generation>({
			.species = Species::Mewtwo,
			.item = Item::Leftovers,
			.ability = Ability::Pressure,
			.stats = {
				.nature = Nature::Modest,
				.evs = EVs(
					EV(4_bi),
					EV(12_bi),
					EV(24_bi),
					EV(0_bi),
					EV(32_bi),
					EV(100_bi)
				),
			},
			.moves = moves
		});
		pokemon.set_status(status);

		auto const str = to_string(pokemon);
		auto const result = pokemon_from_string<generation>(str);
		CHECK(pokemon == result);
	};

	constexpr auto moves = InitialMoves({
		MoveName::Psychic,
		MoveName::Recover,
		MoveName::Calm_Mind,
		MoveName::Taunt,
	});
	for (auto const n : containers::integer_range(1_bi, 4_bi)) {
		check(InitialMoves(containers::take(moves, n)), StatusName::clear);
	}
	check(moves, StatusName::burn);
}

} // namespace technicalmachine
