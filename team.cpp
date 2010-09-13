// Load teams
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <map>
#include <string>
#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "stat.h"
#include "team.h"
#include "type.h"
#include "stat.cpp"

// Warning: Almost everything you see here is a hack.

std::string search (std::ifstream &file, std::string &output2, const std::string &data);
void loadteam (teams &team, const std::string &name);
void loadpokemon (pokemon &member);
void loadmove (moves &move);
void pokelabteam (teams &team, const std::string &name);
void pokelabpokemon (teams &team, std::ifstream &file);

// I do no error checking because I assume Pokelab's teams will always be in the proper format. This must be changed if I ever allow arbitary teams to be used.

void loadteam (teams &team, const std::string &name) {
	team.counter = 0;
	team.light_screen = 0;
	team.lucky_chant = 0;
	team.mist = 0;
	team.reflect = 0;
	team.safeguard = 0;
	team.tailwind = 0;
	team.stealth_rock = false;
	team.spikes = 0;
	team.toxic_spikes = 0;
	team.wish = 0;
	if (name != "") {
		pokelabteam (team, name);
		team.active = team.member.begin();
	}
	for (std::vector<pokemon>::iterator it = team.member.begin(); it != team.member.end(); ++it)
		loadpokemon (*it);
}

void loadpokemon (pokemon &member) {
	member.status = NO_STATUS;
	
	member.type1 = get_pokemon_type [member.pokemon][0];
	member.type2 = get_pokemon_type [member.pokemon][1];

	member.hp.base = base_stat [member.pokemon][0];
	member.atk.base = base_stat [member.pokemon][1];
	member.def.base = base_stat [member.pokemon][2];
	member.spa.base = base_stat [member.pokemon][3];
	member.spd.base = base_stat [member.pokemon][4];
	member.spe.base = base_stat [member.pokemon][5];
	hitpoints (member);
	member.hp.stat = member.hp.max;

	member.mass = mass [member.pokemon];
	
	for (std::vector<moves>::iterator move = member.moveset.begin(); move != member.moveset.end(); ++move)
		loadmove (*move);

}

void loadmove (moves &move) {
	move.pp = move.pp_max;
	move_type (move);
	move.basepower = base_power [move.name];
	move_priority (move);
	move.r = 100;
}

void pokelabteam (teams &team, const std::string &name) {
	std::ifstream file (name.c_str());
	for (int n = 0; n != 6; ++n)
		pokelabpokemon(team, file);
	file.close ();
}

void pokelabpokemon (teams& team, std::ifstream &file) {	// Replace this with a real XML parser. Couldn't figure out TinyXML, should try Xerces.
	pokemon member;
	std::string output2;	// Some lines have more than one data point.
	std::string output1 = search (file, output2, "species=\"");
	std::map <std::string, species> species_map;
	set_species_map (species_map);
	member.pokemon = species_map[output1];
	output1 = search (file, output2, "level>");
	member.level = boost::lexical_cast <int> (output1);
	output1 = search (file, output2, "happiness>");
	member.happiness = boost::lexical_cast <int> (output1);
	output1 = search (file, output2, "gender>");
	std::map <std::string, genders> genders_map;
	set_gender_map (genders_map);
	member.gender = genders_map[output1];
	output1 = search (file, output2, "nature>");
	std::map <std::string, natures> natures_map;
	set_nature_map (natures_map);
	member.nature = natures_map[output1];
	output1 = search (file, output2, "item>");
	std::map <std::string, items> items_map;
	set_item_map (items_map);
	member.item = items_map[output1];
	output1 = search (file, output2, "ability>");
	std::map <std::string, abilities> abilities_map;
	set_ability_map (abilities_map);
	member.ability = abilities_map[output1];
	
	std::map <std::string, moves_list> moves_map;
	set_move_map (moves_map);
	
	while (true) {
		output1 = search (file, output2, "\">");
		if ("No" == output1)
			break;
		moves move;
		move.name = moves_map[output1];
		move.pp_max = boost::lexical_cast <int> (output2);
		member.moveset.push_back (move);
	}
	
	output1 = search (file, output2, "iv=\"");
	member.hp.iv = boost::lexical_cast <int> (output1);
	member.hp.ev = boost::lexical_cast <int> (output2) / 4;
	output1 = search (file, output2, "iv=\"");
	member.atk.iv = boost::lexical_cast <int> (output1);
	member.atk.ev = boost::lexical_cast <int> (output2) / 4;
	output1 = search (file, output2, "iv=\"");
	member.def.iv = boost::lexical_cast <int> (output1);
	member.def.ev = boost::lexical_cast <int> (output2) / 4;
	output1 = search (file, output2, "iv=\"");
	member.spe.iv = boost::lexical_cast <int> (output1);
	member.spe.ev = boost::lexical_cast <int> (output2) / 4;
	output1 = search (file, output2, "iv=\"");
	member.spa.iv = boost::lexical_cast <int> (output1);
	member.spa.ev = boost::lexical_cast <int> (output2) / 4;
	output1 = search (file, output2, "iv=\"");
	member.spd.iv = boost::lexical_cast <int> (output1);
	member.spd.ev = boost::lexical_cast <int> (output2) / 4;
	
	team.member.push_back (member);
}

std::string search (std::ifstream &file, std::string &output2, const std::string &data) {
	std::string output1 = "";
	while (file.eof() == false and "" == output1) {
		std::string line;
		getline (file, line);
		unsigned int x = data.length();
		unsigned int a = line.find (data);
		unsigned int b = line.find ("</", a + x);
		if (std::string::npos == b)
			b = line.find ("\" ev");
		if (std::string::npos == b)
			b = line.find ("\">", a + x);
		if (std::string::npos != a and std::string::npos != b) {
			output1 = line.substr (a + x, b - a - x);			// finds the data between the tags
			if ("iv=\"" == data) {
				a = line.find ("ev=\"");
				b = line.find ("\" /");
				output2 = line.substr (a + x, b - a - x);
			}
			else if ("\">" == data) {
				x = 7;
				a = line.find ("pp-up=\"");
				b = line.find (data);
				output2 = line.substr (a + x, b - a - x);
			}
		}
		if (line.find ("/moveset") != std::string::npos and "\">" == data)
			output1 = "No";
	}
	return output1;
}
