// Status enum
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STATUS_H_
#define STATUS_H_

#include <string>

namespace technicalmachine {

class Status {
	public:
		enum Statuses { NO_STATUS, BURN, FREEZE, PARALYSIS, POISON_NORMAL, POISON_TOXIC, REST, SLEEP, END_STATUS };
		Statuses name;
		Status ();
		bool is_sleeping () const;
		void clear ();
		std::string get_name () const;
};

class Pokemon;
class Team;
class Weather;

void burn (Team &user, Team &target, Weather const &weather);
void freeze (Pokemon const &user, Team &target, Weather const &weather);
void paralyze (Pokemon &user, Pokemon &target, Weather const &weather);
void sleep (Pokemon const &user, Pokemon &target, Weather const &weather);
void poison_normal (Team &user, Team &target, Weather const &weather);
void poison_toxic (Team &user, Team &target, Weather const &weather);

}
#endif		// STATUS_H_
