// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.handle_curse;

import tm.pokemon.any_pokemon;

import tm.heal;
import tm.rational;
import tm.weather;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto handle_curse(any_mutable_active_pokemon auto const pokemon, Weather const weather) -> void {
	if (!pokemon.is_cursed()) {
		return;
	}
	heal(pokemon, weather, rational(-1_bi, 4_bi));
}

} // namespace technicalmachine
