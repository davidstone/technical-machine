// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.make_seen_team;

import tm.pokemon.level;
import tm.pokemon.nickname;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.string_conversions.species;

import tm.gender;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct SeenPokemonInit {
	Species species;
	Nickname nickname = to_string(species);
	Level level = Level(100_bi);
	Gender gender = Gender::genderless;
};

export template<Generation generation, std::size_t size>
constexpr auto make_seen_team(containers::c_array<SeenPokemonInit, size> const & init) {
	auto team = SeenTeam<generation>(bounded::constant<size>);
	for (SeenPokemonInit const pokemon : init) {
		team.add_pokemon(SeenPokemon<generation>(
			pokemon.species,
			pokemon.nickname,
			pokemon.level,
			pokemon.gender
		));
	}
	return team;
}

} // namespace technicalmachine
