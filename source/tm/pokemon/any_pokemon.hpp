// Copyright David Stone.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <type_traits>

namespace technicalmachine {

template<Generation generation>
struct Pokemon;

template<Generation generation>
struct ActivePokemon;

template<Generation generation>
struct MutableActivePokemon;

template<typename T>
inline constexpr auto is_pokemon_impl = false;

template<Generation generation>
inline constexpr auto is_pokemon_impl<Pokemon<generation>> = true;

template<typename T>
concept any_pokemon = is_pokemon_impl<std::remove_cvref_t<T>>;


template<typename T>
inline constexpr auto is_any_active_pokemon_impl = false;

template<Generation generation>
inline constexpr auto is_any_active_pokemon_impl<ActivePokemon<generation>> = true;

template<typename T>
concept any_active_pokemon = is_any_active_pokemon_impl<std::remove_cvref_t<T>>;

template<typename T>
inline constexpr auto is_any_mutable_active_pokemon_impl = false;

template<Generation generation>
inline constexpr auto is_any_mutable_active_pokemon_impl<MutableActivePokemon<generation>> = true;

template<typename T>
concept any_mutable_active_pokemon = is_any_mutable_active_pokemon_impl<std::remove_cvref_t<T>>;

} // namespace technicalmachine
