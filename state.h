// Structure that stores everything about the state of the game
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STATE_H_
#define STATE_H_

#include "team.h"
#include "weather.h"

namespace technicalmachine {

struct State {
	teams ai;
	teams foe;
	Weather weather;
	long score;
	int depth;
	bool operator== (const State &other) const {
		return this->ai == other.ai and this->foe == other.foe and this->weather == other.weather;
	}
};

}
#endif
