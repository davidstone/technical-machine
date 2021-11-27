// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <tm/pokemon/any_pokemon.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <containers/string.hpp>

#include <string_view>

namespace technicalmachine {

template<any_pokemon PokemonType>
auto to_string(PokemonType const & pokemon) -> containers::string;

template<Generation generation>
auto pokemon_from_string(std::string_view str) -> Pokemon<generation>;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(PokemonType) \
	extern template auto to_string(PokemonType const & pokemon) -> containers::string

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(Pokemon<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(KnownPokemon<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(SeenPokemon<generation>); \
	extern template auto pokemon_from_string<generation>(std::string_view str) -> Pokemon<generation>

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION
#undef TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL

} // namespace technicalmachine
