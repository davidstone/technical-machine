// Partial trap timer (things like Wrap and Clamp)
// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/pokemon/end_of_turn_counter.hpp>

#include <tm/compress.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/operators.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

namespace technicalmachine {

template<Generation>
struct MutableActivePokemon;

// This handles the number of turns remaining on Bind, Clamp, Fire Spin,
// Magma Storm, Sand Tomb, Whirlpool, and Wrap
struct PartialTrap {
	constexpr auto is_active() const {
		return m_base.is_active();
	}
	constexpr auto activate() {
		m_base.activate();
	}
	template<Generation generation>
	constexpr auto damage(MutableActivePokemon<generation> pokemon, Weather const weather) -> void {
		if (is_active()) {
			heal(pokemon, weather, rational(-1_bi, 16_bi));
			m_base.advance_one_turn();
		}
	}

	friend auto operator==(PartialTrap const &, PartialTrap const &) -> bool = default;
	friend constexpr auto compress(PartialTrap const value) {
		return compress(value.m_base);
	}
private:
	EndOfTurnCounter<true, 7, CounterOperations::is_active, CounterOperations::activate, CounterOperations::advance_one_turn_variable> m_base;
};

}	// namespace technicalmachine
