// Load Pokemon Lab teams
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
#include "file.h"

namespace technicalmachine {

namespace pl {

unsigned team_size (std::string const &name) {
	std::ifstream file (name.c_str());
	std::string line;
	unsigned size = 0;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (line.find ("<pokemon species") != std::string::npos)
			++size;
	}
	file.close();
	return size;
}

void load_team (Team &team, std::string const &name, Map const &map, unsigned size) {
	team.size = team_size (name);
	std::ifstream file (name.c_str());
	for (unsigned n = 0; n != team.size; ++n)
		load_pokemon (team, file, map, size);
	file.close ();
}

void load_pokemon (Team& team, std::ifstream &file, Map const &map, unsigned size) {	// Replace this with a real XML parser. Couldn't figure out TinyXML, should try Xerces.
	std::string output2;	// Some lines have more than one data point.
	std::string output1 = search (file, output2, "species=\"");
	Pokemon member (map.specie.find (output1)->second, team.size);
	member.nickname = search (file, output2, "<nickname>");
	if (member.nickname == "")
		member.nickname = output1;
	member.level = boost::lexical_cast <int> (search (file, output2, "<level>"));
	member.happiness = boost::lexical_cast <int> (search (file, output2, "<happiness>"));
	member.gender = map.gender.find (search (file, output2, "<gender>"))->second;
	member.nature = map.nature.find (search (file, output2, "<nature>"))->second;
	member.item = map.item.find (search (file, output2, "<item>"))->second;
	member.ability = map.ability.find (search (file, output2, "<ability>"))->second;
	
	
	for (unsigned n = 0; ; ++n) {
		output1 = search (file, output2, "\">");
		if ("No" == output1)
			break;
		moves_list name = map.move.find (output1)->second;
		int pp_ups = boost::lexical_cast <int> (output2);
		Move move (name, pp_ups, size);
		member.move.set.insert (member.move.set.begin() + n, move);
	}
	
	member.hp.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.hp.ev = boost::lexical_cast <int> (output2) / 4;
	member.atk.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.atk.ev = boost::lexical_cast <int> (output2) / 4;
	member.def.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.def.ev = boost::lexical_cast <int> (output2) / 4;
	member.spe.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.spe.ev = boost::lexical_cast <int> (output2) / 4;
	member.spa.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.spa.ev = boost::lexical_cast <int> (output2) / 4;
	member.spd.iv = boost::lexical_cast <int> (search (file, output2, "iv=\""));
	member.spd.ev = boost::lexical_cast <int> (output2) / 4;
	
	team.pokemon.set.push_back (member);
}

std::string search (std::ifstream &file, std::string &output2, std::string const &data) {
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

}

}
