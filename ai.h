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
#include "evaluate.h"
#include "move.h"
#include "movefunction.h"
#include "statfunction.h"
#include "team.h"
#include "teampredictor.h"
#include "weather.h"

void initialize (teams &ai, teams &foe, weathers &weather, score_variables &sv, Map &map, int detailed [][7]) {
	set_species_map (map.specie);
	set_ability_map (map.ability);
	set_item_map (map.item);
	set_nature_map (map.nature);
	set_move_map (map.move);
	detailed_stats (map, detailed);
	loadteam (ai, "ai.sbt", map, detailed);
	ai.me = true;
	for (std::vector<pokemon>::iterator it = ai.member.begin(); it != ai.member.end(); ++it)
		reset_variables (*it);

	pokemon member;
	member.name = SUICUNE;
	member.level = 100;
	member.nickname = "Suicune";
	foe.member.push_back (member);
	loadteam (foe, "", map, detailed);
	foe.me = false;
	for (std::vector<pokemon>::iterator it = foe.member.begin(); it != foe.member.end(); ++it)
		reset_variables (*it);

	weather.trick_room = 0;
	weather.fog = false;
	weather.gravity = 0;
	weather.uproar = 0;
	weather.hail = 0;
	weather.sun = 0;
	weather.sand = 0;
	weather.rain = 0;
	
	ai.replacement = 0;
	ai.active = ai.member.begin();
	switchpokemon (ai, *foe.active, weather);
	ai.active->moved = false;
	ai.active->move = ai.active->moveset.begin();
	
	foe.replacement = 0;
	foe.active = foe.member.begin();
	switchpokemon (foe, *ai.active, weather);
	foe.active->moved = false;
	foe.active->move = foe.active->moveset.begin();

	std::ifstream file ("evaluate.txt");
	std::string line;
	getline (file, line);
	while (!file.eof()) {
		size_t x = line.find ('\t');
		std::string data = line.substr (0, x);
		if (data == "Transposition Table")
			sv.transposition_table = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Light Screen")
			sv.light_screen = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Lucky Chant")
			sv.lucky_chant = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Mist")
			sv.mist = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Reflect")
			sv.reflect = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Safeguard")
			sv.safeguard = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Tailwind")
			sv.tailwind = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Wish")
			sv.wish = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Spikes")
			sv.spikes = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Stealth Rock")
			sv.stealth_rock = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Toxic Spikes")
			sv.toxic_spikes = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Members")
			sv.members = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "HP")
			sv.hp = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Aqua Ring")
			sv.aqua_ring = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Curse")
			sv.curse = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Imprison")
			sv.imprison = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Ingrain")
			sv.ingrain = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Leech Seed")
			sv.leech_seed = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Loaf")
			sv.loaf = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Nightmare")
			sv.nightmare = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Torment")
			sv.torment = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Trapped")
			sv.trapped = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Burn")
			sv.burn = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Freeze")
			sv.freeze = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Paralysis")
			sv.paralysis = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Poison")
			sv.poison = boost::lexical_cast<int> (line.substr (x + 1));
		else if (data == "Sleep")
			sv.sleep = boost::lexical_cast<int> (line.substr (x + 1));
		
		getline (file, line);
	}
}

#endif
