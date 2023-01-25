// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.other_pokemon;

import tm.pokemon.any_pokemon;
import tm.pokemon.known_pokemon;
import tm.pokemon.pokemon;
import tm.pokemon.seen_pokemon;

import tm.generation;

import std_module;

namespace technicalmachine {

template<typename T>
struct OtherPokemonImpl {
};

template<any_known_pokemon T>
struct OtherPokemonImpl<T> {
	using type = SeenPokemon<generation_from<T>>;
};

template<any_seen_pokemon T>
struct OtherPokemonImpl<T> {
	using type = KnownPokemon<generation_from<T>>;
};

template<any_real_pokemon T>
struct OtherPokemonImpl<T> {
	using type = T;
};

export template<typename T>
using OtherPokemon = typename OtherPokemonImpl<T>::type;

} // namespace technicalmachine
