// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.make_seen_team;

import tm.pokemon.level;
import tm.pokemon.max_pokemon_per_team;
import tm.pokemon.nickname;
import tm.pokemon.seen_pokemon;
import tm.pokemon.species;

import tm.string_conversions.species;

import tm.gender;
import tm.generation;
import tm.team;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct SeenPokemonInit {
	TeamSize team_size;
	Species species;
	Nickname nickname = to_string(species);
	Level level = Level(100_bi);
	Gender gender = Gender::genderless;
};

export template<Generation generation>
constexpr auto make_seen_team(SeenPokemonInit const seen) -> SeenTeam<generation> {
	auto team = SeenTeam<generation>(seen.team_size);
	team.add_pokemon(SeenPokemon<generation>(
		seen.species,
		seen.nickname,
		seen.level,
		seen.gender
	));
	return team;
}

} // namespace technicalmachine
