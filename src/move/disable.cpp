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

#include "disable.hpp"

#include <cstdint>

namespace technicalmachine {
namespace {

constexpr uint8_t inactive = 0xFF;
constexpr uint8_t max_disable_length = 7;

}	// unnamed namespace

Disable::Disable() :
	counter(inactive) {
}

void Disable::activate() {
	counter = 0;
}

void Disable::advance_one_turn() {
	if (!*this)
		return;
	// TODO: update with proper probability actions
	if (counter < max_disable_length)
		++counter;
	else
		reset();
}

void Disable::reset () {
	counter = inactive;
}

Disable::operator bool() const {
	return counter != inactive;
}

uint64_t Disable::hash() const {
	return counter;
}

uint64_t Disable::max_hash() {
	return max_disable_length;
}

bool operator== (Disable const lhs, Disable const rhs) {
	return lhs.counter == rhs.counter;
}

bool operator!= (Disable const lhs, Disable const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
