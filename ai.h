// Information for AI
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef AI_H_
#define AI_H_

#include "move.h"
#include "team.h"
#include "weather.h"
#include "move.cpp"

void initialize (teams &ai, teams &foe, weathers &weather) {
	loadteam (ai, "team.sbt");
	loadteam (foe, "team.sbt");

	weather.trick_room = 0;
	weather.fog = false;
	weather.gravity = 0;
	weather.uproar = 0;
	weather.hail = 0;
	weather.sun = 0;
	weather.sand = 0;
	weather.rain = 0;
	
	switchpokemon (foe, foe.member.at(1), weather);
	foe.active->moved = false;

	for (std::vector<moves>::iterator it = foe.active->moveset.begin(); it != foe.active->moveset.end(); ++it) {
		it->physical = false;
		it->ch = false;
	}
	
	switchpokemon (ai, ai.member.at(2), weather);
	ai.active->moved = false;
	
	for (std::vector<moves>::iterator it = ai.active->moveset.begin(); it != ai.active->moveset.end(); ++it) {
		it->physical = true;
		it->ch = false;
	}
}

#endif
