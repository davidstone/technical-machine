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

#ifndef ANALYZE_LOGS_H_
#define ANALYZE_LOGS_H_

#include <fstream>
#include <map>
#include <string>
#include "gender.h"
#include "move.h"
#include "species.h"

namespace technicalmachine {

class Map;
class Team;
class Weather;

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
		bool shed_skin;

		Log (Team & ai, Team & foe);
		bool getline ();
		bool search_is_first ();
		bool search_is_first (std::string const & str);
		Move::moves_list find_move_name (Map const & map) const;
		void analyze_line (Team &ai, Team &foe, Map const &map);
	private:
		bool ignore_line (Team const & ai, Team const & foe);
	public:
		void log_pokemon (Team &team, Team &target, Map const &map);
		bool seen_pokemon (Team & team, species nickname);
	private:
		void get_pokemon_info (Map const & map, species & name, std::string & nickname, int & level, genders & gender);
	public:
		void pokemon_sent_out (Map const & map, species name, std::string const & nickname, int level, genders gender, Team & team, Team & other);
	private:
		void add_pokemon (Team & team, species name, std::string const & nickname, int level, genders gender);
	public:
		void log_move (Move::moves_list name);
	private:
		bool side_effect ();
	public:
		void log_misc (Map const &map);
		void initialize_turn (Team &ai, Team &foe);
	private:
		void initialize_team (Team &team);
};

bool analyze_turn (Team &ai, Team &foe, Weather &weather, Map const &map);

void do_turn (Team &first, Team &last, Weather &weather);

void normalize_hp (Team &first, Team &last);

void normalize_hp_team (Team &team);

}
#endif
