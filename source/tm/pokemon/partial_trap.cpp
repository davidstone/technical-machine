// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.partial_trap;

import tm.pokemon.any_pokemon;
import tm.pokemon.end_of_turn_counter;

import tm.compress;
import tm.environment;
import tm.heal;
import tm.rational;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

// This handles the number of turns remaining on Bind, Clamp, Fire Spin,
// Magma Storm, Sand Tomb, Whirlpool, and Wrap
export struct PartialTrap {
	constexpr auto is_active() const {
		return m_base.is_active();
	}
	constexpr auto activate() {
		m_base.activate();
	}
	constexpr auto damage(any_mutable_active_pokemon auto const pokemon, Environment const environment) -> void {
		if (is_active()) {
			heal(pokemon, environment, rational(-1_bi, 16_bi));
			m_base.advance_one_turn();
		}
	}

	friend auto operator==(PartialTrap, PartialTrap) -> bool = default;
	friend constexpr auto compress(PartialTrap const value) {
		return compress(value.m_base);
	}

private:
	EndOfTurnCounter<true, 7> m_base;
};

} // namespace technicalmachine
