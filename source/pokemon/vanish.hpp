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

#include "../operators.hpp"

#include "../move/moves.hpp"

#include <bounded/integer.hpp>

#include <cstdint>

namespace technicalmachine {

// Various states a Pokemon can be in due to vanishing moves.

struct Vanish {
private:
	enum class VanishTypes : std::uint8_t {
		none, bounce, dig, dive, fly, shadow_force, end
	};

	constexpr auto flip(VanishTypes const flipped) {
		m_state = (m_state == VanishTypes::none) ? flipped : VanishTypes::none;
		return m_state != VanishTypes::none;
	}

	constexpr auto doubles_ground_power() const {
		return m_state == VanishTypes::dig;
	}
	constexpr auto doubles_surf_power() const {
		return m_state == VanishTypes::dive;
	}
	constexpr auto doubles_wind_power() const {
		switch (m_state) {
			case VanishTypes::bounce:
			case VanishTypes::fly:
				return true;
			default:
				return false;
		}
	}
public:
	// Returns whether the Pokemon ends up in a Vanished state
	constexpr auto bounce() {
		return flip(VanishTypes::bounce);
	}
	constexpr auto dig() {
		return flip(VanishTypes::dig);
	}
	constexpr auto dive() {
		return flip(VanishTypes::dive);
	}
	constexpr auto fly() {
		return flip(VanishTypes::fly);
	}
	constexpr auto shadow_force() {
		return flip(VanishTypes::shadow_force);
	}

	constexpr auto doubles_move_power(Moves const move) const {
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
	
	friend constexpr auto compare(Vanish const lhs, Vanish const rhs) {
		return bounded::compare(lhs.m_state, rhs.m_state);
	}

private:
	VanishTypes m_state = VanishTypes::none;
};

}	// namespace technicalmachine
