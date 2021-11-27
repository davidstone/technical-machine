// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/any_team.hpp>
#include <tm/generation.hpp>

namespace technicalmachine {

template<typename T>
concept any_known =
	any_known_pokemon<T> or
	any_known_active_pokemon<T> or
	any_known_mutable_active_pokemon<T> or
	any_known_team<T>;

template<typename T>
concept any_seen =
	any_seen_pokemon<T> or
	any_seen_active_pokemon<T> or
	any_seen_mutable_active_pokemon<T> or
	any_seen_team<T>;

template<typename T>
concept any_real =
	any_real_pokemon<T> or
	any_real_active_pokemon<T> or
	any_real_mutable_active_pokemon<T> or
	any_real_team<T>;

template<typename T>
using AssociatedTeam =
	std::conditional_t<any_known<T>, KnownTeam<generation_from<T>>,
	std::conditional_t<any_seen<T>, SeenTeam<generation_from<T>>,
	std::conditional_t<any_real<T>, Team<generation_from<T>>,
	void
>>>;

template<typename T>
using AssociatedPokemon =
	std::conditional_t<any_known<T>, KnownPokemon<generation_from<T>>,
	std::conditional_t<any_seen<T>, SeenPokemon<generation_from<T>>,
	std::conditional_t<any_real<T>, Pokemon<generation_from<T>>,
	void
>>>;

template<typename T>
using OtherPokemon =
	std::conditional_t<any_known_pokemon<T>, SeenPokemon<generation_from<T>>,
	std::conditional_t<any_seen_pokemon<T>, KnownPokemon<generation_from<T>>,
	std::conditional_t<any_real_pokemon<T>, Pokemon<generation_from<T>>,
	void
>>>;

template<typename T>
using OtherActivePokemon = AnyActivePokemon<OtherPokemon<AssociatedPokemon<T>>>;

template<typename T>
using OtherMutableActivePokemon = AnyMutableActivePokemon<OtherPokemon<AssociatedPokemon<T>>>;

template<typename T>
using OtherTeam =
	std::conditional_t<any_known<T>, SeenTeam<generation_from<T>>,
	std::conditional_t<any_seen<T>, KnownTeam<generation_from<T>>,
	std::conditional_t<any_real<T>, Team<generation_from<T>>,
	void
>>>;

} // namespace technicalmachine
