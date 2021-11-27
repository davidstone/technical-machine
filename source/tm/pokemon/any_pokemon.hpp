// Copyright David Stone.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <type_traits>

namespace technicalmachine {

template<Generation generation>
struct KnownPokemon;

template<Generation generation>
struct SeenPokemon;

template<Generation generation>
struct Pokemon;


template<typename PokemonType>
struct AnyActivePokemon;

template<typename PokemonType>
struct AnyMutableActivePokemon;

template<Generation generation>
using ActivePokemon = AnyActivePokemon<Pokemon<generation>>;

template<Generation generation>
using MutableActivePokemon = AnyMutableActivePokemon<Pokemon<generation>>;


template<typename T>
inline constexpr auto is_known_pokemon_impl = false;

template<Generation generation>
inline constexpr auto is_known_pokemon_impl<KnownPokemon<generation>> = true;

template<typename T>
concept any_known_pokemon = is_known_pokemon_impl<std::remove_cvref_t<T>>;

template<typename T>
inline constexpr auto is_seen_pokemon_impl = false;

template<Generation generation>
inline constexpr auto is_seen_pokemon_impl<SeenPokemon<generation>> = true;

template<typename T>
concept any_seen_pokemon = is_seen_pokemon_impl<std::remove_cvref_t<T>>;

template<typename T>
inline constexpr auto is_real_pokemon_impl = false;

template<Generation generation>
inline constexpr auto is_real_pokemon_impl<Pokemon<generation>> = true;

template<typename T>
concept any_real_pokemon = is_real_pokemon_impl<std::remove_cvref_t<T>>;

template<typename T>
concept any_pokemon = any_known_pokemon<T> or any_seen_pokemon<T> or any_real_pokemon<T>;


template<typename T>
inline constexpr auto is_any_active_pokemon_impl = false;

template<typename PokemonType>
inline constexpr auto is_any_active_pokemon_impl<AnyActivePokemon<PokemonType>> = true;

template<typename T>
concept any_active_pokemon = is_any_active_pokemon_impl<std::remove_cvref_t<T>>;

template<typename T>
inline constexpr auto is_any_mutable_active_pokemon_impl = false;

template<typename PokemonType>
inline constexpr auto is_any_mutable_active_pokemon_impl<AnyMutableActivePokemon<PokemonType>> = true;

template<typename T>
concept any_mutable_active_pokemon = is_any_mutable_active_pokemon_impl<std::remove_cvref_t<T>>;


template<typename PokemonType>
inline constexpr auto generation_from<AnyActivePokemon<PokemonType>> = generation_from<PokemonType>;

template<typename PokemonType>
inline constexpr auto generation_from<AnyMutableActivePokemon<PokemonType>> = generation_from<PokemonType>;

template<typename PokemonType>
concept any_known_active_pokemon = std::same_as<
	PokemonType,
	AnyActivePokemon<KnownPokemon<generation_from<PokemonType>>>
>;
template<typename PokemonType>
concept any_known_mutable_active_pokemon = std::same_as<
	PokemonType,
	AnyMutableActivePokemon<KnownPokemon<generation_from<PokemonType>>>
>;

template<typename PokemonType>
concept any_seen_active_pokemon = std::same_as<
	PokemonType,
	AnyActivePokemon<SeenPokemon<generation_from<PokemonType>>>
>;
template<typename PokemonType>
concept any_seen_mutable_active_pokemon = std::same_as<
	PokemonType,
	AnyMutableActivePokemon<SeenPokemon<generation_from<PokemonType>>>
>;

template<typename PokemonType>
concept any_real_active_pokemon = std::same_as<
	PokemonType,
	AnyActivePokemon<Pokemon<generation_from<PokemonType>>>
>;
template<typename PokemonType>
concept any_real_mutable_active_pokemon = std::same_as<
	PokemonType,
	AnyMutableActivePokemon<Pokemon<generation_from<PokemonType>>>
>;

} // namespace technicalmachine
