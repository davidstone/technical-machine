// Class that handles DelayedAttack
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

#include "delayed_attack.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned max_turns = 3;
}	// unnamed namespace

DelayedAttack::DelayedAttack() :
	turns_before_hitting(0)
	{
}

bool DelayedAttack::is_active() const {
	return turns_before_hitting != 0;
}

void DelayedAttack::activate() {
	if (is_active())
		return;
	// TODO: implement
}

bool DelayedAttack::decrement() {
	if (!is_active())
		return false;

	--turns_before_hitting;
	return turns_before_hitting == 0;
}

void DelayedAttack::reset() {
	turns_before_hitting = 0;
}

DelayedAttack::hash_type DelayedAttack::hash() const {
	return turns_before_hitting;
}

DelayedAttack::hash_type DelayedAttack::max_hash() {
	return max_turns;
}

bool operator== (DelayedAttack const & lhs, DelayedAttack const & rhs) {
	return lhs.turns_before_hitting == rhs.turns_before_hitting;
}

bool operator!= (DelayedAttack const & lhs, DelayedAttack const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
