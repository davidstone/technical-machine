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

void trick_room (Weather &weather) {
	if (weather.trick_room == 0)
		weather.trick_room = 5;
	else
		weather.trick_room = 0;
}
void gravity (Weather &weather) {
	if (weather.gravity == 0)
		weather.gravity = 5;
}
void uproar (Weather &weather, int duration) {
	if (weather.uproar < duration)
		weather.uproar = duration;
}
void hail (Weather &weather, int duration) {
	if (weather.hail == 0) {
		weather.sun = 0;
		weather.sand = 0;
		weather.rain = 0;
		weather.hail = duration;
	}
}
void sun (Weather &weather, int duration) {
	if (weather.sun == 0) {
		weather.hail = 0;
		weather.sand = 0;
		weather.rain = 0;
		weather.sun = duration;
	}
}
void sand (Weather &weather, int duration) {
	if (weather.sand == 0) {
		weather.hail = 0;
		weather.sun = 0;
		weather.rain = 0;
		weather.sand = duration;
	}
}
void rain (Weather &weather, int duration) {
	if (weather.rain == 0) {
		weather.hail = 0;
		weather.sun = 0;
		weather.sand = 0;
		weather.rain = duration;
	}
}

}
