// Priority data structure
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

#include "priority.hpp"

#include <cstdint>

#include "moves.hpp"

namespace technicalmachine {
namespace {

int8_t get_priority(Moves move);

}	// unnamed namespace

Priority::Priority(Moves const move) :
	cached_priority(get_priority(move)) {
}

bool operator== (Priority const lhs, Priority const rhs) {
	return lhs.cached_priority == rhs.cached_priority;
}
bool operator!= (Priority const lhs, Priority const rhs) {
	return !(lhs == rhs);
}
bool operator< (Priority const lhs, Priority const rhs) {
	return lhs.cached_priority < rhs.cached_priority;
}
bool operator> (Priority const lhs, Priority const rhs) {
	return lhs.cached_priority > rhs.cached_priority;
}
bool operator<= (Priority const lhs, Priority const rhs) {
	return !(lhs > rhs);
}
bool operator>= (Priority const lhs, Priority const rhs) {
	return !(lhs < rhs);
}

namespace {

int8_t get_priority(Moves const move) {
	switch (move) {
		case Moves::SWITCH0:
		case Moves::SWITCH1:
		case Moves::SWITCH2:
		case Moves::SWITCH3:
		case Moves::SWITCH4:
		case Moves::SWITCH5:
			return 6;
		case Moves::HELPING_HAND:
			return 5;
		case Moves::MAGIC_COAT:
		case Moves::SNATCH:
			return 4;
		case Moves::DETECT:
		case Moves::ENDURE:
		case Moves::FOLLOW_ME:
		case Moves::PROTECT:
			return 3;
		case Moves::FEINT:
			return 2;
		case Moves::AQUA_JET:
		case Moves::BIDE:
		case Moves::BULLET_PUNCH:
		case Moves::EXTREMESPEED:
		case Moves::FAKE_OUT:
		case Moves::ICE_SHARD:
		case Moves::MACH_PUNCH:
		case Moves::QUICK_ATTACK:
		case Moves::SHADOW_SNEAK:
		case Moves::SUCKER_PUNCH:
		case Moves::VACUUM_WAVE:
			return 1;
		case Moves::VITAL_THROW:
			return -1;
		case Moves::FOCUS_PUNCH:
			return -2;
		case Moves::AVALANCHE:
		case Moves::REVENGE:
			return -3;
		case Moves::COUNTER:
		case Moves::MIRROR_COAT:
			return -4;
		case Moves::ROAR:
		case Moves::WHIRLWIND:
			return -5;
		case Moves::TRICK_ROOM:
			return -6;
		default:
			return 0;
	}
}
}	// unnamed namespace
}	// namespace technicalmachine
