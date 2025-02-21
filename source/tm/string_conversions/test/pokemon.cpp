// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

export module tm.string_conversions.test.pokemon;

import tm.move.initial_move;
import tm.move.move_name;

import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.nature;

import tm.status.status_name;

import tm.string_conversions.pokemon;

import tm.ability;
import tm.generation;
import tm.item;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

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
