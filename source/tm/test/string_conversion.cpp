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

TEST_CASE("Generation 1 pokemon", "[string_conversion]") {
	constexpr auto make = [](StatusName const status, InitialMoves const moves) {
		auto pokemon = Pokemon<Generation::one>({
			.species = Species::Mewtwo,
			.stats = {
				.evs = OldGenEVs(
					EV(4_bi),
					EV(252_bi),
					EV(255_bi),
					EV(0_bi),
					EV(32_bi)
				),
			},
			.moves = moves
		});
		pokemon.set_status(status);

		return to_string(pokemon);
	};

	CHECK(
		make(
			StatusName::clear,
			InitialMoves({
				MoveName::Psychic
			})
		) ==
		std::string_view(
			"Mewtwo (100.0% HP)\n"
			"\tEVs: 4 HP / 0 Spe / 32 Spc\n"
			"\t- Psychic"
		)
	);

	CHECK(
		make(
			StatusName::clear,
			InitialMoves({
				MoveName::Psychic,
				MoveName::Recover,
				MoveName::Calm_Mind,
				MoveName::Taunt
			})
		) ==
		std::string_view(
			"Mewtwo (100.0% HP)\n"
			"\tEVs: 4 HP / 0 Spe / 32 Spc\n"
			"\t- Psychic\n"
			"\t- Recover\n"
			"\t- Calm Mind\n"
			"\t- Taunt"
		)
	);

	CHECK(
		make(
			StatusName::burn,
			InitialMoves({
				MoveName::Psychic
			})
		) ==
		std::string_view(
			"Mewtwo (100.0% HP)\n"
			"\tStatus: Burn\n"
			"\tEVs: 4 HP / 0 Spe / 32 Spc\n"
			"\t- Psychic"
		)
	);
}

TEST_CASE("Generation 3 pokemon", "[string_conversion]") {
	constexpr auto make = [](StatusName const status, InitialMoves const moves) {
		auto pokemon = Pokemon<Generation::three>({
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

		return to_string(pokemon);
	};

	CHECK(
		make(
			StatusName::clear,
			InitialMoves({
				MoveName::Psychic
			})
		) ==
		std::string_view(
			"Mewtwo (100.0% HP) @ Leftovers\n"
			"\tAbility: Pressure\n"
			"\tNature: Modest\n"
			"\tEVs: 4 HP / 12 Atk / 24 Def / 32 SpD / 100 Spe\n"
			"\t- Psychic"
		)
	);

	CHECK(
		make(
			StatusName::burn,
			InitialMoves({
				MoveName::Psychic
			})
		) ==
		std::string_view(
			"Mewtwo (100.0% HP) @ Leftovers\n"
			"\tAbility: Pressure\n"
			"\tStatus: Burn\n"
			"\tNature: Modest\n"
			"\tEVs: 4 HP / 12 Atk / 24 Def / 32 SpD / 100 Spe\n"
			"\t- Psychic"
		)
	);
}

} // namespace technicalmachine
