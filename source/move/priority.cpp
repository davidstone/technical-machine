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
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6;
		case Moves::Helping_Hand:
			return 5;
		case Moves::Magic_Coat:
		case Moves::Snatch:
			return 4;
		case Moves::Detect:
		case Moves::Endure:
		case Moves::Follow_Me:
		case Moves::Protect:
			return 3;
		case Moves::Feint:
			return 2;
		case Moves::Aqua_Jet:
		case Moves::Bide:
		case Moves::Bullet_Punch:
		case Moves::ExtremeSpeed:
		case Moves::Fake_Out:
		case Moves::Ice_Shard:
		case Moves::Mach_Punch:
		case Moves::Quick_Attack:
		case Moves::Shadow_Sneak:
		case Moves::Sucker_Punch:
		case Moves::Vacuum_Wave:
			return 1;
		case Moves::Vital_Throw:
			return -1;
		case Moves::Focus_Punch:
			return -2;
		case Moves::Avalanche:
		case Moves::Revenge:
			return -3;
		case Moves::Counter:
		case Moves::Mirror_Coat:
			return -4;
		case Moves::Roar:
		case Moves::Whirlwind:
			return -5;
		case Moves::Trick_Room:
			return -6;
		default:
			return 0;
	}
}
}	// unnamed namespace
}	// namespace technicalmachine
