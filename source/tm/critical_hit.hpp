// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/move_name.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/any_pokemon.hpp>

#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {

auto critical_hit_probability(any_active_pokemon auto attacker, MoveName, Ability defender_ability, Weather) -> double;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto critical_hit_probability<ActivePokemon<generation>>(ActivePokemon<generation> attacker, MoveName, Ability defender_ability, Weather) -> double

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

} // namespace technicalmachine