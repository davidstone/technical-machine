// Screens
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

#include "screen.hpp"

namespace technicalmachine {

Screen::Screen():
	turns_remaining(0) {
}

uint8_t Screen::max_length() {
	return 5;
}

void Screen::set_duration(uint8_t const duration) {
	if (turns_remaining == 0)
		turns_remaining = duration;
}

void Screen::activate() {
	set_duration(max_length());
}

void Screen::decrement() {
	if (turns_remaining > 0)
		--turns_remaining;
}

Screen::operator bool() const {
	return turns_remaining != 0;
}

uint64_t Screen::hash() const {
	return turns_remaining;
}
uint64_t Screen::max_hash() {
	return max_length();
}

bool operator==(Screen const & lhs, Screen const & rhs) {
	return lhs.turns_remaining == rhs.turns_remaining;
}
bool operator!=(Screen const & lhs, Screen const & rhs) {
	return !(lhs == rhs);
}


uint8_t ReflectLightScreen::max_length() {
	return 8;
}

void ReflectLightScreen::activate(bool const is_extended) {
	enum Duration : uint8_t { standard = 5, extended = 8 };
	set_duration(is_extended ? extended : standard);
}

void ReflectLightScreen::clear() {
	set_duration(0);
}

uint64_t ReflectLightScreen::max_hash() {
	return max_length();
}


uint8_t Tailwind::max_length() {
	return 3;
}

void Tailwind::activate() {
	set_duration(max_length());
}

uint64_t Tailwind::max_hash() {
	return max_length();
}

}	// namespace technicalmachine
