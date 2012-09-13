// Class that handles Outrage, Petal Dance, and Thrash
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

#include "rampage.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned min_duration = 2;
constexpr unsigned max_duration = 3;
}	// unnamed namespace

Rampage::Rampage() :
	turns_remaining(0)
	{
}

bool Rampage::is_active() const {
	return turns_remaining != 0;
}

void Rampage::activate() {
	if (!is_active())
		turns_remaining = max_duration;
}

bool Rampage::decrement() {
	if (!is_active())
		return false;
	--turns_remaining;
	return !is_active();
}

void Rampage::reset() {
	turns_remaining = 0;
}

Rampage::hash_type Rampage::hash() const {
	return turns_remaining;
}

Rampage::hash_type Rampage::max_hash() {
	return max_duration;
}

bool operator== (Rampage const & lhs, Rampage const & rhs) {
	return lhs.turns_remaining == rhs.turns_remaining;
}

bool operator!= (Rampage const & lhs, Rampage const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
