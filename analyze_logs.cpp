// Analyze Shoddy Battle logs
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
#include <iostream>
#include <map>
#include "team.h"
#include "pokemon.h"
#include "move.h"
#include "ability.h"

void log_move (pokemon &member, pokemon* &previous, const std::string &line, const std::map <std::string, moves_list> &moves_map, const std::string &search) {
	previous = &member;
	// Account for Windows / Unix line endings
	size_t n = 1;
	if (line.find(".\r") != std::string::npos)
		n = 2;
	moves_list move_name = moves_map.find (line.substr (search.length(), line.length() - search.length() - n))->second;
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

void log_pokemon  (teams &team, const std::string &line, const std::map <std::string, species> &species_map, std::string &search1) {
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
		team.active->name = species_map.find (line.substr (found1 + search1.length(), found2 - found1 - search1.length()))->second;
	}
}

void first (teams &team, std::ifstream &file, const std::map <std::string, species> &species_map) {
	std::string line;
	getline (file, line);	// Data I need starts on the second line
	std::string search = " sent out ";
	team.player = line.substr (0, line.find (search));
	log_pokemon (team, line, species_map, search);
}

void output (std::ofstream &output, const teams &team) {
	output << team.player << ":\n";
	for (std::vector<pokemon>::const_iterator active = team.member.begin(); active != team.member.end(); ++active) {
		output << pokemon_name [active->name];
		if (active->item == LEFTOVERS)
			output << " @ Leftovers";
		else if (active->item == BLACK_SLUDGE)
			output << " @ Black Sludge";
		output << " ** " << active->nickname << '\n';
		if (active->ability != END_ABILITY)
			output << "\tAbility: " << ability_name [active->ability] << '\n';
		for (std::vector<moves>::const_iterator move = active->moveset.begin(); move != active->moveset.end(); ++move)
			output << "\t- " << move_name [move->name] << '\n';
	}
	output << '\n';
}

void isme (teams &team) {
	if (team.player == "graviton" or team.player == "Graviton" or team.player == "king of the king" or team.player == "Morlock" or team.player == "obi" or team.player == "Obi" or team.player == "reziarfg" or team.player == "Reziarfg" or team.player == "Specter" or team.player == "Sylar" or team.player == "Tracer Tong")
		team.me = true;
	else
		team.me = false;
}

int main (int argc, char* argv[]) {
	std::map <std::string, species> species_map;
	set_species_map (species_map);
	std::map <std::string, moves_list> moves_map;
	set_move_map (moves_map);
	std::map <std::string, abilities> abilities_map;
	set_ability_map (abilities_map);
	std::string input = "/media/C/Documents and Settings/David/Shoddy logs/";
	if (argc == 1)
		input += "hey young world OU_Machamp9 lose ST6.doc";
	else {
		input += argv [1];
		for (int n = 2; n != argc; ++n) {
			input += ' ';
			input += argv[n];
		}
	}
	std::ifstream file (input.c_str());
	if (!file.is_open()) {
		std::cout << "Not a valid file.\n";
		return 1;
	}
	std::string line;
	getline (file, line);
	teams player1;
	teams player2;
	// name sent out nickname (lvl x species ?).
	first (player1, file, species_map);
	first (player2, file, species_map);
	pokemon* previous;
	getline (file, line);
	while (!file.eof()) {
		if (line.find(": ") == std::string::npos) {		// Should be a player's comment, hopefully nobody puts : in anywhere in their names
			// name switched in nickname (lvl x species ?).
			std::string search = " switched in ";
			size_t found = line.find (search);
			if (found != std::string::npos) {
				if (line.find (player1.player) != std::string::npos)
					log_pokemon (player1, line, species_map, search);
				else
					log_pokemon (player2, line, species_map, search);
			}

			std::vector<pokemon>::iterator active;
			std::vector<pokemon>::iterator inactive;
			if (line.substr (0, player1.active->nickname.length()) == player1.active->nickname) {
				active = player1.active;
				inactive = player2.active;
			}
			else {
				active = player2.active;
				inactive = player1.active;
			}
			
			// It's best to include both nicknames in the search instead of just the invariant section. This prevents any combination of nicknames from causing an error. A Pokemon cannot have its own nickname plus something else in its nickname.
			// nickname used move.
			search = active->nickname + " used ";
			if (line.find (search) == 0)		// If the beginning of the line is this
				log_move (*active, previous, line, moves_map, search);

			if (active->item == END_ITEM) {
				if (active->nickname + "'s Black Sludge restored a little health!" == line)
					active->item = BLACK_SLUDGE;
				else if (active->nickname + "'s leftovers restored its health a little!" == line)
					active->item = LEFTOVERS;
			}
			if (active->ability == END_ABILITY) {
				if (active->nickname + " makes ground moves miss with Levitate!" == line)
					active->ability = LEVITATE;
				else if (active->nickname + "'s intimidate cut " + inactive->nickname + "'s attack!" == line)
					active->ability = INTIMIDATE;
				else if (active->nickname + "'s Poison Heal restored health!" == line)
					active->ability = POISON_HEAL;
				else if (active->nickname + " is exerting its pressure!" == line)
					active->ability = PRESSURE;
				else if (active->nickname + "'s Sand Stream whipped up a sandstorm!" == line)
					active->ability = SAND_STREAM;
				else if (active->nickname + "'s Speed Boost raised its speed!" == line)
					active->ability = SPEED_BOOST;
				else if (active->nickname + "'s Snow Warning whipped up a hailstorm!" == line or active->nickname + "'s Snow Warning whipped up a hailstorm! " == line)
					active->ability = SNOW_WARNING;
				else if (line.find (active->nickname + " traced " + inactive->nickname + "'s ") == 0)
					active->ability = TRACE;
			}
			if (inactive->ability == END_ABILITY) {
				if (active->nickname + " sucked up liquid ooze!" == line)
					inactive->ability = LIQUID_OOZE;
				search = active->nickname + " traced " + inactive->nickname + "'s ";
				if (line.find (search) != std::string::npos) {
					size_t n = 1;
					if (line.find(".\r") != std::string::npos)
						n = 2;
					inactive->ability = abilities_map.find (line.substr (search.length(), line.length() - search.length() - n))->second;
				}
			}
		}
		getline (file, line);
	}
	file.close ();

	isme (player1);
	isme (player2);
	if (!player1.me and !player2.me)
		std::cout << player1.player << " vs. " << player2.player << '\n';
	teams* player;
	if (player1.me)
		player = &player2;
	else
		player = &player1;
	std::ofstream out ("output.txt", std::ios::app);
	output (out, *player);
	out.close();
	return 0;
}
