// Substitute class
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

#include "substitute.hpp"
#include <cassert>

namespace technicalmachine {

Substitute::Substitute():
	hp(0)
	{
}

namespace {
constexpr unsigned max_hp = 714;
}	// unnamed namespace

void Substitute::create(unsigned const total_hp) {
	assert(total_hp <= max_hp / 4);
	hp = total_hp;
}

void Substitute::damage(unsigned const damage_taken) {
	hp = (damage_taken >= hp) ? 0 : hp - damage_taken;
}

void Substitute::destroy() {
	hp = 0;
}

Substitute::operator bool() const {
	return hp != 0;
}

Substitute::hash_type Substitute::hash() const {
	return hp;
}

Substitute::hash_type Substitute::max_hash() {
	return max_hp / 4 + 1;
}

bool operator== (Substitute const & lhs, Substitute const & rhs) {
	return lhs.hp == rhs.hp;
}
bool operator!= (Substitute const & lhs, Substitute const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
