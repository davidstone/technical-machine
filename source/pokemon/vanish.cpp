// Vanishing moves
// Copyright (C) 2012 David Stone
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

enum class Vanish::VanishTypes : uint8_t {
	none, bounce, dig, dive, fly, shadow_force, end
};

Vanish::Vanish () :
	vanish (VanishTypes::none)
	{
}

void Vanish::reset() {
	vanish = VanishTypes::none;
}

void Vanish::flip(VanishTypes const flipped) {
	if (vanish == VanishTypes::none)
		vanish = flipped;
	else
		reset();
}

void Vanish::bounce() {
	flip(VanishTypes::bounce);
}

void Vanish::dig() {
	flip(VanishTypes::dig);
}

void Vanish::dive() {
	flip(VanishTypes::dive);
}

void Vanish::fly() {
	flip(VanishTypes::fly);
}

void Vanish::shadow_force() {
	flip(VanishTypes::shadow_force);
}

bool Vanish::doubles_move_power(Moves const move) const {
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

bool Vanish::doubles_ground_power() const {
	return vanish == VanishTypes::dig;
}

bool Vanish::doubles_surf_power() const {
	return vanish == VanishTypes::dive;
}

bool Vanish::doubles_wind_power() const {
	switch (vanish) {
		case VanishTypes::bounce:
		case VanishTypes::fly:
			return true;
		default:
			return false;
	}
}

Vanish::hash_type Vanish::hash() const {
	return static_cast<hash_type>(vanish);
}

Vanish::hash_type Vanish::max_hash() {
	return static_cast<hash_type>(VanishTypes::end);
}

bool operator== (Vanish const lhs, Vanish const rhs) {
	return lhs.vanish == rhs.vanish;
}
bool operator!= (Vanish const lhs, Vanish const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
