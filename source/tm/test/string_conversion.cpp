// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature;

import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.gender;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.item;
import tm.string_conversions.move_name;
import tm.string_conversions.nature;
import tm.string_conversions.pokemon;
import tm.string_conversions.species;
import tm.string_conversions.status_name;
import tm.string_conversions.type;
import tm.string_conversions.weather;

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
namespace {
using namespace bounded::literal;

using namespace std::string_view_literals;

template<typename Enum>
void test_generic() {
	for (auto const original : containers::enum_range<Enum>()) {
		auto const str = to_string(original);
		auto const result = from_string<Enum>(str);
		CHECK(original == result);
	}
}

TEST_CASE("ability", "[string_conversion]") {
	test_generic<Ability>();
}
TEST_CASE("gender", "[string_conversion]") {
	test_generic<Gender>();
}
TEST_CASE("item", "[string_conversion]") {
	test_generic<Item>();
}
TEST_CASE("move", "[string_conversion]") {
	test_generic<MoveName>();
}
TEST_CASE("nature", "[string_conversion]") {
	test_generic<Nature>();
}
TEST_CASE("species", "[string_conversion]") {
	test_generic<Species>();
}
TEST_CASE("status", "[string_conversion]") {
	test_generic<StatusName>();
}
TEST_CASE("type", "[string_conversion]") {
	test_generic<Type>();
}
TEST_CASE("weather", "[string_conversion]") {
	test_generic<NormalWeather>();
}

TEST_CASE("pokemon", "[string_conversion]") {
	constexpr auto generation = Generation::three;

	auto make_pokemon = [](RegularMoves const moves, StatusName const status = StatusName::clear) {
		auto pokemon = Pokemon<generation>(
			Species::Mewtwo,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Pressure,
			CombinedStats<generation>{
				Nature::Modest,
				max_dvs_or_ivs<generation>,
				EVs(
					EV(4_bi),
					EV(12_bi),
					EV(24_bi),
					EV(0_bi),
					EV(32_bi),
					EV(100_bi)
				)
			},
			moves
		);
		pokemon.set_status(status);
		return pokemon;
	};

	auto check = [](Pokemon<generation> const pokemon) {
		auto const str = to_string(pokemon);
		auto const result = pokemon_from_string<generation>(str);

		CHECK(pokemon == result);
	};

	auto moves = RegularMoves({Move(generation, MoveName::Psychic)});
	check(make_pokemon(moves));
	for (auto const move : {MoveName::Recover, MoveName::Calm_Mind, MoveName::Taunt}) {
		moves.push_back(Move(generation, move));
		check(make_pokemon(moves));
	}
	check(make_pokemon(moves, StatusName::burn));
}

} // namespace
} // namespace technicalmachine
