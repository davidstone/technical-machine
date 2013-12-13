// Priority data structure
// Copyright (C) 2013 David Stone
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

#include "moves.hpp"

namespace technicalmachine {

using namespace bounded_integer::literal;
namespace {

bounded_integer::checked_integer<-6, 6> get_priority(Moves move);

}	// unnamed namespace

Priority::Priority(Moves const move) :
	priority(get_priority(move)) {
}

bool operator== (Priority const lhs, Priority const rhs) {
	return lhs.priority == rhs.priority;
}
bool operator!= (Priority const lhs, Priority const rhs) {
	return !(lhs == rhs);
}
bool operator< (Priority const lhs, Priority const rhs) {
	return lhs.priority < rhs.priority;
}
bool operator> (Priority const lhs, Priority const rhs) {
	return rhs < lhs;
}
bool operator<= (Priority const lhs, Priority const rhs) {
	return !(lhs > rhs);
}
bool operator>= (Priority const lhs, Priority const rhs) {
	return !(lhs < rhs);
}

namespace {

bounded_integer::checked_integer<-6, 6> get_priority(Moves const move) {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return 6_bi;
		case Moves::Helping_Hand:
			return 5_bi;
		case Moves::Magic_Coat:
		case Moves::Snatch:
			return 4_bi;
		case Moves::Detect:
		case Moves::Endure:
		case Moves::Follow_Me:
		case Moves::Protect:
			return 3_bi;
		case Moves::Feint:
			return 2_bi;
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
			return 1_bi;
		case Moves::Vital_Throw:
			return -1_bi;
		case Moves::Focus_Punch:
			return -2_bi;
		case Moves::Avalanche:
		case Moves::Revenge:
			return -3_bi;
		case Moves::Counter:
		case Moves::Mirror_Coat:
			return -4_bi;
		case Moves::Roar:
		case Moves::Whirlwind:
			return -5_bi;
		case Moves::Trick_Room:
			return -6_bi;
		default:
			return 0_bi;
	}
}
}	// unnamed namespace
}	// namespace technicalmachine
