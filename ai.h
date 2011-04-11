// Information for AI
// Copyright 2011 David Stone
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

#include <map>
#include "move.h"
#include "movefunction.h"
#include "statfunction.h"
#include "team.h"
#include "teampredictor.h"
#include "weather.h"

namespace technicalmachine {

void initialize (teams &ai, teams &foe, Map &map, int detailed [][7]) {
	detailed_stats (map, detailed);

	ai.player = "Technical Machine";
	ai.me = true;
	std::string ai_file;
	std::ifstream settings ("settings.txt");
	for (getline (settings, line); !settings.eof(); getline (settings, line)) {
		size_t found = line.find ('\t');
		if (line.substr (0, found) == "team")
			ai_file = line.substr (found + 1);
	}
	
	loadteam (ai, "teams/" + ai_file, map, detailed);
	ai.active = ai.member.begin();

/*	pokemon member;
	member.name = SUICUNE;
	member.level = 100;
	member.nickname = "Suicune";
	foe.member.push_back (member);
*/
	foe.me = false;
	loadteam (foe, "", map, detailed);
}

}
#endif
