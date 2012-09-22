// Class that handles Slow Start's timer
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

#include "slow_start.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned initial_duration = 5;
}	// unnamed namespace

SlowStart::SlowStart() :
	turns_remaining(initial_duration)
	{
}

bool SlowStart::is_active() const {
	return turns_remaining != 0;
}

void SlowStart::decrement() {
	if (is_active())
		--turns_remaining;
}

void SlowStart::reset() {
	turns_remaining = initial_duration;
}

SlowStart::hash_type SlowStart::hash() const {
	return turns_remaining;
}

SlowStart::hash_type SlowStart::max_hash() {
	return initial_duration;
}

bool operator== (SlowStart const & lhs, SlowStart const & rhs) {
	return lhs.turns_remaining == rhs.turns_remaining;
}

bool operator!= (SlowStart const & lhs, SlowStart const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
