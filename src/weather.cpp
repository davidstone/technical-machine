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
namespace {
void conditional_decrement (int8_t & n) {
	if (n > 0)
		--n;
}
}	// unnamed namespace

Weather::Weather () :
	fog (false),
	trick_room_duration (0),
	gravity_duration (0),
	uproar_duration (0),
	hail_duration (0),
	sun_duration (0),
	sand_duration (0),
	rain_duration (0)
	{
}

bool Weather::operator== (Weather const & other) const {
	// I could theoretically speed this up by comparing the block of memory.
	// Weather occupies a single 8-byte section of memory.
	return trick_room_duration == other.trick_room_duration and
			fog == other.fog and
			gravity_duration == other.gravity_duration and
			uproar_duration == other.uproar_duration and
			hail_duration == other.hail_duration and
			sun_duration == other.sun_duration and
			sand_duration == other.sand_duration and
			rain_duration == other.rain_duration;
}

bool Weather::trick_room () const {
	return trick_room_duration;
}
bool Weather::gravity () const {
	return gravity_duration;
}
bool Weather::uproar () const {
	return uproar_duration;
}
bool Weather::hail () const {
	return hail_duration;
}
bool Weather::sun () const {
	return sun_duration;
}
bool Weather::sand () const {
	return sand_duration;
}
bool Weather::rain () const {
	return rain_duration;
}

void Weather::decrement () {
	conditional_decrement (trick_room_duration);
	conditional_decrement (gravity_duration);
	conditional_decrement (uproar_duration);
	conditional_decrement (hail_duration);
	conditional_decrement (sun_duration);
	conditional_decrement (sand_duration);
	conditional_decrement (rain_duration);
}

void Weather::set_trick_room () {
	trick_room_duration = trick_room() ? 0 : 5;
}

void Weather::set_gravity () {
	if (!gravity())
		gravity_duration = 5;
}

void Weather::set_uproar (int8_t const duration) {
	if (uproar_duration < duration)
		uproar_duration = duration;
}

void Weather::set_hail (Duration const duration) {
	set_weather (hail_duration, duration);
}
void Weather::set_sun (Duration const duration) {
	set_weather (sun_duration, duration);
}
void Weather::set_sand (Duration const duration) {
	set_weather (sand_duration, duration);
}
void Weather::set_rain (Duration const duration) {
	set_weather (rain_duration, duration);
}

void Weather::set_weather (int8_t & primary, Duration const duration) {
	if (primary == 0 or duration.value == Duration::permanent) {
		hail_duration = 0;
		sand_duration = 0;
		rain_duration = 0;
		sun_duration = 0;
		primary = duration.value;
	}
}

template<>
bool Weather::blocks_status<Status::FREEZE> () const {
	return sun();
}

template<>
bool Weather::blocks_status<Status::SLEEP> () const {
	return uproar();
}

template<>
bool Weather::blocks_status<Status::REST> () const {
	return blocks_status<Status::SLEEP>();
}

Weather::hash_type Weather::hash () const {
	// All of weather requires fewer than 22 bits to represent exactly, so this
	// hash has absolutely no collisions. There are a lot of illegal values
	// (such as sun having 4 turns left and rain having 3 turns left), and so
	// it should be possible to write a 0-collision hash for weather that needs
	// 16 bits of information or fewer.
	
	// The + 1 is because they have a minimum value of -1 to represent infinite
	// weather
	return static_cast<hash_type> (trick_room_duration) + 5 *
			(static_cast<hash_type> (fog) + 2 *
			(static_cast<hash_type> (gravity_duration + 1) + 6 *
			(static_cast<hash_type> (uproar_duration + 1) + 6 *
			(static_cast<hash_type> (hail_duration + 1) + 9 *
			(static_cast<hash_type> (sun_duration + 1) + 9 *
			(static_cast<hash_type> (sand_duration + 1) + 9 *
			static_cast<hash_type> (rain_duration + 1)))))));
}

}	// namespace technicalmachine
