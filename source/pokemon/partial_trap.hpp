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

#include "../hash.hpp"

#include <bounded_integer/optional.hpp>

namespace technicalmachine {
class ActivePokemon;

// This class handles the number of turns remaining on Bind, Clamp, Fire Spin,
// Magma Storm, Sand Tomb, Whirlpool, and Wrap
class PartialTrap {
public:
	auto is_active() const -> bool;
	auto activate() -> void;
	auto damage(ActivePokemon & pokemon) -> void;
	friend auto operator==(PartialTrap lhs, PartialTrap rhs) -> bool;
	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_turns_active);
	}
	constexpr auto max_hash() const noexcept {
		return technicalmachine::max_hash(m_turns_active);
	}
private:
	friend class Evaluate;
	using type = bounded::integer<0, 7>;
	bounded::optional<type> m_turns_active;
};

auto operator!=(PartialTrap lhs, PartialTrap rhs) -> bool;

constexpr auto hash(PartialTrap const partial_trap) noexcept {
	return partial_trap.hash();
}
constexpr auto max_hash(PartialTrap const partial_trap) noexcept {
	return partial_trap.max_hash();
}

}	// namespace technicalmachine
#endif	// PARTIAL_TRAP_HPP_
