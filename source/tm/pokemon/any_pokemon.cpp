// Copyright David Stone.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.any_pokemon;

import std_module;

namespace technicalmachine {

export template<typename T>
constexpr auto is_known_pokemon = false;

export template<typename T>
concept any_known_pokemon = is_known_pokemon<std::remove_cvref_t<T>>;


export template<typename T>
constexpr auto is_seen_pokemon = false;

export template<typename T>
concept any_seen_pokemon = is_seen_pokemon<std::remove_cvref_t<T>>;


export template<typename T>
constexpr auto is_real_pokemon = false;

export template<typename T>
concept any_real_pokemon = is_real_pokemon<std::remove_cvref_t<T>>;


export template<typename T>
concept any_pokemon = any_known_pokemon<T> or any_seen_pokemon<T> or any_real_pokemon<T>;


export template<typename T>
constexpr auto is_any_active_pokemon = false;

export template<typename T>
concept any_active_pokemon = is_any_active_pokemon<std::remove_cvref_t<T>>;

export template<typename T>
constexpr auto is_any_mutable_active_pokemon = false;

export template<typename T>
concept any_mutable_active_pokemon = is_any_mutable_active_pokemon<std::remove_cvref_t<T>>;

} // namespace technicalmachine
