// Vanishing moves
// Copyright (C) 2015 David Stone
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

#include "vanish.hpp"
#include <cstdint>
#include "../move/moves.hpp"

namespace technicalmachine {

auto Vanish::flip(VanishTypes const flipped) -> bool {
	m_state = (m_state == VanishTypes::none) ? flipped : VanishTypes::none;
	return m_state != VanishTypes::none;
}

auto Vanish::bounce() -> bool {
	return flip(VanishTypes::bounce);
}

auto Vanish::dig() -> bool {
	return flip(VanishTypes::dig);
}

auto Vanish::dive() -> bool {
	return flip(VanishTypes::dive);
}

auto Vanish::fly() -> bool {
	return flip(VanishTypes::fly);
}

auto Vanish::shadow_force() -> bool {
	return flip(VanishTypes::shadow_force);
}

auto Vanish::doubles_move_power(Moves const move) const -> bool {
	switch (move) {
		case Moves::Earthquake:
		case Moves::Magnitude:
			return doubles_ground_power();
		case Moves::Gust:
		case Moves::Twister:
			return doubles_wind_power();
		case Moves::Surf:
			return doubles_surf_power();
		default:
			return false;
	}
}

auto Vanish::doubles_ground_power() const -> bool {
	return m_state == VanishTypes::dig;
}

auto Vanish::doubles_surf_power() const -> bool {
	return m_state == VanishTypes::dive;
}

auto Vanish::doubles_wind_power() const -> bool {
	switch (m_state) {
		case VanishTypes::bounce:
		case VanishTypes::fly:
			return true;
		default:
			return false;
	}
}

auto operator==(Vanish const lhs, Vanish const rhs) -> bool {
	return lhs.m_state == rhs.m_state;
}

}	// namespace technicalmachine
