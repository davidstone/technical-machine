// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/active_pokemon_forward.hpp>

#include <tm/ability.hpp>
#include <tm/item.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

template<Generation generation, typename Numerator, typename Denominator>
void heal(MutableActivePokemon<generation> pokemon, Weather const weather, rational<Numerator, Denominator> const scale) {
	auto const hp = pokemon.hp();
	if (hp.current() == 0_bi) {
		return;
	}
	auto const hp_healed = hp.max() * scale;
	if (scale > 0_bi) {
		change_hp(pokemon, weather, bounded::max(hp_healed, 1_bi));
	} else if (!blocks_secondary_damage(pokemon.ability())) {
		change_hp(pokemon, weather, bounded::min(hp_healed, -1_bi));
	}
}

constexpr auto healing_multiplier(Item const item) {
	return rational(
		BOUNDED_CONDITIONAL(item == Item::Big_Root, 13_bi, 10_bi),
		10_bi
	);
}

}	// namespace technicalmachine
