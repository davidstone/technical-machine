// Weather functions
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

#include "weather.hpp"
#include <cstdint>
#include "status.hpp"

namespace technicalmachine {

Weather::Weather () :
	trick_room (0),
	fog (false),
	gravity (0),
	uproar (0),
	hail (0),
	sun (0),
	sand (0),
	rain (0)
	{
}

bool Weather::operator == (Weather const & other) const {
	// I could theoretically speed this up by comparing the block of memory.
	// Weather occupies a single 8-byte section of memory.
	return trick_room == other.trick_room and
			fog == other.fog and
			gravity == other.gravity and
			uproar == other.uproar and
			hail == other.hail and
			sun == other.sun and
			sand == other.sand and
			rain == other.rain;
}

void Weather::set_trick_room () {
	trick_room = (trick_room == 0) ? 5 : 0;
}

void Weather::set_gravity () {
	if (gravity == 0)
		gravity = 5;
}

void Weather::set_uproar (int8_t duration) {
	if (uproar < duration)
		uproar = duration;
}

void Weather::set_hail (int8_t duration) {
	if (hail == 0) {
		sun = 0;
		sand = 0;
		rain = 0;
		hail = duration;
	}
}
void Weather::set_sun (int8_t duration) {
	if (sun == 0) {
		hail = 0;
		sand = 0;
		rain = 0;
		sun = duration;
	}
}
void Weather::set_sand (int8_t duration) {
	if (sand == 0) {
		hail = 0;
		sun = 0;
		rain = 0;
		sand = duration;
	}
}
void Weather::set_rain (int8_t duration) {
	if (rain == 0) {
		hail = 0;
		sun = 0;
		sand = 0;
		rain = duration;
	}
}

template<>
bool Weather::blocks_status<Status::FREEZE> () const {
	return sun;
}

template<>
bool Weather::blocks_status<Status::SLEEP> () const {
	return uproar;
}

template<>
bool Weather::blocks_status<Status::REST> () const {
	return blocks_status<Status::SLEEP>();
}

uint32_t Weather::hash () const {
	// All of weather requires fewer than 22 bits to represent exactly, so this
	// hash has absolutely no collisions. There are a lot of illegal values
	// (such as sun having 4 turns left and rain having 3 turns left), and so
	// it should be possible to write a 0-collision hash for weather that needs
	// 16 bits of information or fewer.
	
	// The + 1 is because they have a minimum value of -1 to represent infinite
	// weather
	return static_cast<unsigned> (trick_room) + 5 *
			(static_cast<unsigned> (fog) + 2 *
			(static_cast<unsigned> (gravity + 1) + 6 *
			(static_cast<unsigned> (uproar + 1) + 6 *
			(static_cast<unsigned> (hail + 1) + 9 *
			(static_cast<unsigned> (sun + 1) + 9 *
			(static_cast<unsigned> (sand + 1) + 9 *
			static_cast<unsigned> (rain + 1)))))));
}

}	// namespace technicalmachine
