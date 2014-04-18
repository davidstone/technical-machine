// Vanishing moves
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

#ifndef VANISH_HPP_
#define VANISH_HPP_

#include <cstdint>
#include "../move/moves_forward.hpp"

namespace technicalmachine {

// Various states a Pokemon can be in due to vanishing moves.

class Vanish {
public:
	Vanish();

	// Returns whether the Pokemon ends up in a Vanished state
	auto bounce() -> bool;
	auto dig() -> bool;
	auto dive() -> bool;
	auto fly() -> bool;
	auto shadow_force() -> bool;

	auto doubles_move_power(Moves move) const -> bool;
	using hash_type = uint64_t;
	auto hash() const -> hash_type;
	static auto max_hash() -> hash_type;
	friend auto operator== (Vanish const lhs, Vanish const rhs) -> bool;
private:
	auto doubles_ground_power() const -> bool;
	auto doubles_surf_power() const -> bool;
	auto doubles_wind_power() const -> bool;
	enum class VanishTypes : uint8_t;
	auto flip(VanishTypes const flipped) -> bool;
	VanishTypes m_state;
};

bool operator!= (Vanish const lhs, Vanish const rhs);

}	// namespace technicalmachine
#endif	// VANISH_HPP_
