// Interface for the stand-alone log analyzer / team extractor
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "analyze_logs.h"
#include "pokemon.h"
#include "team.h"

using namespace technicalmachine;

int main (int argc, char* argv[]) {

	Map map;

	std::string input = "";
	if (argc == 1)
		input += "/home/david/Shoddy logs/hey young world OU_Machamp9 lose ST6.doc";
	else {
		input += argv [1];
		// Account for spaces in file names
		for (int n = 2; n != argc; ++n) {
			input += ' ';
			input += argv[n];
		}
	}
	std::ifstream file (input.c_str());
	if (!file.is_open()) {
		std::cout << input + " is not a valid file.\n";
		return 1;
	}
	std::string line;
	Team player1;
	player1.player = "";
	Team player2;
	player2.player = "";
	pokemon* previous;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		analyze_line (player1, player2, previous, line, map);
	}
	file.close ();

	isme (player1);
	isme (player2);
	if (!player1.me and !player2.me)
		std::cout << player1.player << " vs. " << player2.player << '\n';
	Team* player;
	if (player1.me)
		player = &player2;
	else
		player = &player1;
	std::string str;
	output (str, *player);
	std::ofstream out ("output.txt", std::ios::app);
	out << str;
	out.close();
	return 0;
}
