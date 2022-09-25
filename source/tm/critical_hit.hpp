// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/move_name.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/any_pokemon.hpp>

#include <tm/ability.hpp>
#include <tm/for_each_generation.hpp>
#include <tm/generation.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {

auto critical_hit_probability(any_active_pokemon auto attacker, MoveName, Ability defender_ability, Weather) -> double;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto critical_hit_probability<ActivePokemon<generation>>(ActivePokemon<generation> attacker, MoveName, Ability defender_ability, Weather) -> double

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXTERN_INSTANTIATION);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

} // namespace technicalmachine