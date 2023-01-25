// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.faint;

import tm.pokemon.any_pokemon;

import tm.weather;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto faint(any_pokemon auto & pokemon) -> void {
	pokemon.set_hp(0_bi);
}

export constexpr auto faint(any_mutable_active_pokemon auto const pokemon) -> void {
	pokemon.set_hp(Weather(), 0_bi);
}

} // namespace technicalmachine