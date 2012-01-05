// Load Pokemon Lab teams
// Copyright (C) 2011 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "file.hpp"
#include "../pokemon.hpp"
#include "../team.hpp"

namespace technicalmachine {
namespace pl {

static unsigned team_size (std::string const & name);
static void load_pokemon (Team & team, std::ifstream & file, unsigned size);
static std::string search (std::ifstream & file, std::string & output2, std::string const & data);

unsigned team_size (std::string const & name) {
	std::ifstream file (name);
	std::string line;
	unsigned size = 0;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.find ("<pokemon species") != std::string::npos)
			++size;
	}
	file.close();
	return size;
}

void load_team (Team & team, std::string const & name, unsigned size) {
	team.size = team_size (name);
	std::ifstream file (name);
	for (unsigned n = 0; n != team.size; ++n)
		load_pokemon (team, file, size);
	file.close ();
}

void load_pokemon (Team & team, std::ifstream & file, unsigned size) {
	// Replace this with a real XML parser. Couldn't figure out TinyXML, should try Xerces.
	std::string output2;		// Some lines have more than one data point.
	std::string output1 = search (file, output2, "species=\"");
	Pokemon member (Pokemon::from_string (output1), team.size);
	member.nickname = search (file, output2, "<nickname>");
	if (member.nickname == "")
		member.nickname = output1;
	member.level = std::stoi (search (file, output2, "<level>"));
	member.happiness = std::stoi (search (file, output2, "<happiness>"));
	member.gender.set_name_from_string (search (file, output2, "<gender>"));
	member.nature.set_name_from_string (search (file, output2, "<nature>"));
	member.item.set_name_from_string (search (file, output2, "<item>"));
	member.ability.set_name_from_string (search (file, output2, "<ability>"));
	
	
	for (unsigned n = 0; ; ++n) {
		output1 = search (file, output2, "\">");
		if ("No" == output1)
			break;
		Move::Moves const name = Move::from_string (output1);
		int const pp_ups = std::stoi (output2);
		Move const move (name, pp_ups, size);
		member.move.set.insert (member.move.set.begin() + n, move);
	}
	
	member.hp.iv = std::stoi (search (file, output2, "iv=\""));
	member.hp.ev = std::stoi (output2) / 4;
	member.atk.iv = std::stoi (search (file, output2, "iv=\""));
	member.atk.ev = std::stoi (output2) / 4;
	member.def.iv = std::stoi (search (file, output2, "iv=\""));
	member.def.ev = std::stoi (output2) / 4;
	member.spe.iv = std::stoi (search (file, output2, "iv=\""));
	member.spe.ev = std::stoi (output2) / 4;
	member.spa.iv = std::stoi (search (file, output2, "iv=\""));
	member.spa.ev = std::stoi (output2) / 4;
	member.spd.iv = std::stoi (search (file, output2, "iv=\""));
	member.spd.ev = std::stoi (output2) / 4;
	
	team.pokemon.set.push_back (member);
}

std::string search (std::ifstream & file, std::string & output2, std::string const & data) {
	std::string output1 = "";
	while (!file.eof() and "" == output1) {
		std::string line;
		getline (file, line);
		size_t x = data.length();
		size_t a = line.find (data);
		size_t b = line.find ("</", a + x);
		if (b == std::string::npos)
			b = line.find ("\" ev");
		if (b == std::string::npos)
			b = line.find ("\">", a + x);
		if (a != std::string::npos and b != std::string::npos) {
			output1 = line.substr (a + x, b - a - x);			// finds the data between the tags
			if (output1 == "")
				break;
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

} // namespace pl
} // namespace technicalmachine
