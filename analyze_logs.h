// Analyze Shoddy Battle logs header
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <fstream>
#include <map>
#include <string>
#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

class Log {
	public:
		std::string input;
		std::string line;
		std::string search;
	private:
		size_t newline1;
		size_t newline2;
	public:
		// active and inactive keep track of the Pokemon that are the "major" Pokemon of that line. This helps keep track of information on future lines so I can do things like assign critical hits to the right move.
		Team* active;
		Team* inactive;
		Team* first;
		Team* last;
		bool phaze;
		bool move_damage;

		Log ();
		bool getline ();
		bool search_is_first ();
};

bool analyze_turn (Team &ai, Team &foe, Weather &weather, Map const &map);

void analyze_line (Team &ai, Team &foe, Weather &weather, Map const &map, Log &log);

void log_pokemon (Team &team, Team &target, Weather &weather, Map const &map, Log &log);

void log_move (Log &log, Weather const &weather, Map const &map);

void log_misc (Log &log, Map const &map, bool &shed_skin);

void do_turn (Team &first, Team &last, Weather &weather);

void initialize_turn (Team &ai, Team &foe, Log &log);

void initialize_team (Team &team);

void normalize_hp (Team &first, Team &last);

void normalize_hp_team (Team &team);

}
