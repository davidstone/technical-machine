// Analyze Shoddy Battle logs
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/lexical_cast.hpp>
#include <map>
#include <string>
#include <iostream>
#include "analyze_logs.h"
#include "ability.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "team.h"

namespace technicalmachine {

void first (teams &team, std::string &line, const Map &map) {
	std::string search = " sent out ";
	team.player = line.substr (0, line.find (search));
	log_pokemon (team, line, map, search);
}

void log_pokemon  (teams &team, const std::string &line, const Map &map, std::string &search1) {
	search1 = team.player + search1;
	std::string search2 = " (lvl ";
	size_t found2 = line.find (search2);
	std::string nickname = line.substr (search1.length(), found2 - search1.length());
	bool found = false;
	for (std::vector<pokemon>::iterator it = team.member.begin(); it != team.member.end(); ++it) {
		if (nickname == it->nickname) {
			found = true;
			team.active = it;
			break;
		}
	}
	if (!found) {
		pokemon member;
		team.member.push_back (member);
		team.active = team.member.end() - 1;
		team.active->nickname = nickname;
		team.active->item = END_ITEM;
		team.active->ability = END_ABILITY;
		search1 = " ";
		size_t found1 = line.find (search1, found2 + 6);
		team.active->level = boost::lexical_cast<int> (line.substr (found2 + search2.length(), found1 - found2 - search2.length()));
		search2 = " ?).";
		found2 = line.find (search2);
		if (found2 == std::string::npos) {
			search2 = " ♂).";
			found2 = line.find (search2);
			if (found2 != std::string::npos)
				team.active->gender = MALE;
		}
		if (found2 == std::string::npos) {
			search2 = " ♀).";
			found2 = line.find (search2);
			if (found2 != std::string::npos)
				team.active->gender = FEMALE;
		}
		if (found2 == std::string::npos) {
			search2 = ").";
			found2 = line.find (search2);
		}
		team.active->name = map.specie.find (line.substr (found1 + search1.length(), found2 - found1 - search1.length()))->second;
	}
}

void log_move (pokemon &member, pokemon* &previous, const std::string &line, const Map &map, const std::string &search) {
	previous = &member;
	// Account for Windows / Unix line endings
	size_t n = 1;
	if (line.find(".\r") != std::string::npos)
		n = 2;
	moves_list move_name = map.move.find (line.substr (search.length(), line.length() - search.length() - n))->second;
	bool isfound = false;
	for (std::vector<moves>::iterator it = member.moveset.begin(); it != member.moveset.end(); ++it) {
		if (move_name == it->name) {
			isfound = true;
			member.move = it;
			break;
		}
	}
	if (!isfound) {
		moves move;
		member.moveset.push_back (move);
		member.move = member.moveset.end() - 1;
		member.move->name = move_name;
	}
}

void log_misc (pokemon &active, pokemon &inactive, const std::string &line, const Map &map) {
	if (active.item == END_ITEM) {
		if (active.nickname + "'s Black Sludge restored a little health!" == line)
			active.item = BLACK_SLUDGE;
		else if (active.nickname + "'s leftovers restored its health a little!" == line)
			active.item = LEFTOVERS;
	}
	if (active.ability == END_ABILITY) {
		if (active.nickname + " makes ground moves miss with Levitate!" == line)
			active.ability = LEVITATE;
		else if (active.nickname + "'s intimidate cut " + inactive.nickname + "'s attack!" == line)
			active.ability = INTIMIDATE;
		else if (active.nickname + "'s Poison Heal restored health!" == line)
			active.ability = POISON_HEAL;
		else if (active.nickname + " is exerting its pressure!" == line)
			active.ability = PRESSURE;
		else if (active.nickname + "'s Sand Stream whipped up a sandstorm!" == line)
			active.ability = SAND_STREAM;
		else if (active.nickname + "'s Speed Boost raised its speed!" == line)
			active.ability = SPEED_BOOST;
		else if (active.nickname + "'s Snow Warning whipped up a hailstorm!" == line or active.nickname + "'s Snow Warning whipped up a hailstorm! " == line)
			active.ability = SNOW_WARNING;
		else if (line.find (active.nickname + " traced " + inactive.nickname + "'s ") == 0)
			active.ability = TRACE;
	}
	if (inactive.ability == END_ABILITY) {
		if (active.nickname + " sucked up liquid ooze!" == line)
			inactive.ability = LIQUID_OOZE;
		std::string search = active.nickname + " traced " + inactive.nickname + "'s ";
		if (line.find (search) != std::string::npos) {
			size_t n = 1;
			if (line.find(".\r") != std::string::npos)
				n = 2;
			inactive.ability = map.ability.find (line.substr (search.length(), line.length() - search.length() - n))->second;
		}
	}
}

void isme (teams &team) {		// Top secret information. Can't let this leak out. Make sure to remove this function before uploading.
	if (team.player == "graviton" or team.player == "Graviton" or team.player == "king of the king" or team.player == "Morlock" or team.player == "obi" or team.player == "Obi" or team.player == "reziarfg" or team.player == "Reziarfg" or team.player == "Specter" or team.player == "Sylar" or team.player == "Tracer Tong")
		team.me = true;
	else
		team.me = false;
}

}
