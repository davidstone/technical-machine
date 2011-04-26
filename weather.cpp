// Weather functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "weather.h"

namespace technicalmachine {

void Weather::set_trick_room () {
	if (trick_room == 0)
		trick_room = 5;
	else
		trick_room = 0;
}
void Weather::set_gravity () {
	if (gravity == 0)
		gravity = 5;
}
void Weather::set_uproar (int duration) {
	if (uproar < duration)
		uproar = duration;
}
void Weather::set_hail (int duration) {
	if (hail == 0) {
		sun = 0;
		sand = 0;
		rain = 0;
		hail = duration;
	}
}
void Weather::set_sun (int duration) {
	if (sun == 0) {
		hail = 0;
		sand = 0;
		rain = 0;
		sun = duration;
	}
}
void Weather::set_sand (int duration) {
	if (sand == 0) {
		hail = 0;
		sun = 0;
		rain = 0;
		sand = duration;
	}
}
void Weather::set_rain (int duration) {
	if (rain == 0) {
		hail = 0;
		sun = 0;
		sand = 0;
		rain = duration;
	}
}

}
