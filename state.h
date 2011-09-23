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

namespace technicalmachine {

class Team;
class Weather;

struct State {
	Team ai;
	Team foe;
	Weather weather;
	int64_t score;
	int depth;
	bool operator== (State const & other) const {
		return ai == other.ai and foe == other.foe and weather == other.weather;
	}
	State () : ai (true), foe (false) {}
};

}
#endif		// STATE_H_
