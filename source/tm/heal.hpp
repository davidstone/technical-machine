// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/ability.hpp>
#include <tm/item.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

void heal_exactly(any_mutable_active_pokemon auto const pokemon, Weather const weather, bounded::bounded_integer auto const hp_healed) {
	if (pokemon.hp().current() == 0_bi) {
		return;
	}
	if (hp_healed > 0_bi) {
		change_hp(pokemon, weather, bounded::max(hp_healed, 1_bi));
	} else if (!blocks_secondary_damage(pokemon.ability())) {
		change_hp(pokemon, weather, bounded::min(hp_healed, -1_bi));
	}
}

template<typename Numerator, typename Denominator>
void heal(any_mutable_active_pokemon auto const pokemon, Weather const weather, rational<Numerator, Denominator> const scale) {
	heal_exactly(pokemon, weather, pokemon.hp().max() * scale);
}

constexpr auto healing_multiplier(Item const item) {
	return rational(
		BOUNDED_CONDITIONAL(item == Item::Big_Root, 13_bi, 10_bi),
		10_bi
	);
}

} // namespace technicalmachine
