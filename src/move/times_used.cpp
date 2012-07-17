// class Move
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

#include "times_used.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "../rational.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned max_relevant_counter_value = 10;
}	// unnamed namespace

TimesUsed::TimesUsed() :
	counter(0) {
}

void TimesUsed::reset () {
	counter = 0;
}

uint64_t TimesUsed::hash() const {
	return counter;
}

uint64_t TimesUsed::max_hash() const {
	return max_relevant_counter_value + 1;
}

void TimesUsed::increment() {
	if (counter < max_relevant_counter_value)
		++counter;
}

bool TimesUsed::was_used_last() const {
	return counter != 0;
}

unsigned TimesUsed::fury_cutter_power() const {
	// 10 * 2 ^ n
	return 10u << std::min(static_cast<unsigned>(counter), 4u);
}

unsigned TimesUsed::momentum_move_power() const {
	return 30u << std::min(static_cast<unsigned>(counter), 4u);
}

unsigned TimesUsed::triple_kick_power() const {
	return 10 * std::min(static_cast<unsigned>(counter), 3u);
}

Rational TimesUsed::metronome_boost() const {
	assert(counter <= 10);
	return Rational(10u + counter, 10);
}

bool operator== (TimesUsed const lhs, TimesUsed const rhs) {
	return lhs.counter == rhs.counter;
}
bool operator!= (TimesUsed const lhs, TimesUsed const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
