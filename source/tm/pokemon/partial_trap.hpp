// Partial trap timer (things like Wrap and Clamp)
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>
#include <tm/pokemon/end_of_turn_counter.hpp>

#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {

// This handles the number of turns remaining on Bind, Clamp, Fire Spin,
// Magma Storm, Sand Tomb, Whirlpool, and Wrap
struct PartialTrap {
	constexpr auto is_active() const {
		return m_base.is_active();
	}
	constexpr auto activate() {
		m_base.activate();
	}
	constexpr auto damage(any_mutable_active_pokemon auto const pokemon, Weather const weather) -> void {
		if (is_active()) {
			heal(pokemon, weather, rational(-1_bi, 16_bi));
			m_base.advance_one_turn();
		}
	}

	friend auto operator==(PartialTrap, PartialTrap) -> bool = default;
	friend constexpr auto compress(PartialTrap const value) {
		return compress(value.m_base);
	}

private:
	EndOfTurnCounter<true, 7, CounterOperations::is_active, CounterOperations::activate, CounterOperations::advance_one_turn_variable> m_base;
};

} // namespace technicalmachine
