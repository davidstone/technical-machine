// Partial trap timer (things like Wrap and Clamp)
// Copyright (C) 2014 David Stone
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

#ifndef PARTIAL_TRAP_HPP_
#define PARTIAL_TRAP_HPP_

#include "end_of_turn_counter.hpp"
#include "../hash.hpp"

namespace technicalmachine {
class ActivePokemon;

// This class handles the number of turns remaining on Bind, Clamp, Fire Spin,
// Magma Storm, Sand Tomb, Whirlpool, and Wrap
class PartialTrap {
public:
	constexpr auto is_active() const {
		return m_base.is_active();
	}
	auto activate() {
		m_base.activate();
	}
	auto damage(ActivePokemon & pokemon) -> void;

	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_base);
	}
	friend constexpr auto operator==(PartialTrap const lhs, PartialTrap const rhs) -> bool {
		return lhs.m_base == rhs.m_base;
	}
private:
	EndOfTurnCounter<7, CounterOperations::is_active, CounterOperations::activate, CounterOperations::advance_one_turn> m_base;
};

constexpr auto operator!=(PartialTrap const lhs, PartialTrap const rhs) {
	return !(lhs == rhs);
}

constexpr auto hash(PartialTrap const partial_trap) noexcept {
	return partial_trap.hash();
}

}	// namespace technicalmachine
#endif	// PARTIAL_TRAP_HPP_
