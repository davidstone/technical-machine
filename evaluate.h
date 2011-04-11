// Evaluate header
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef EVALUATE_H_
#define EVALUATE_H_

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <string>
#include "move.h"
#include "pokemon.h"
#include "simple.h"
#include "status.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

const long VICTORY = 65536;		// Highest score possible. 100% chance to win.

struct score_variables {
	int transposition_table;	
	
	int light_screen;
	int lucky_chant;
	int mist;
	int reflect;
	int safeguard;
	int tailwind;

	int wish;

	int spikes;
	int stealth_rock;
	int toxic_spikes;

	int members;
	int hp;
	int aqua_ring;
	int curse;
	int imprison;
	int ingrain;
	int leech_seed;
	int loaf;
	int nightmare;
	int torment;
	int trapped;

	int burn;
	int freeze;
	int paralysis;
	int poison;
	int sleep;
	
	score_variables () {
		std::ifstream file ("evaluate.txt");
		std::string line;
		for (getline (file, line); !file.eof(); getline (file, line)) {
			size_t x = line.find ('\t');
			std::string data = line.substr (0, x);
			if (data == "Transposition Table")
				transposition_table = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Light Screen")
				light_screen = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Lucky Chant")
				lucky_chant = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Mist")
				mist = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Reflect")
				reflect = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Safeguard")
				safeguard = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Tailwind")
				tailwind = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Wish")
				wish = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Spikes")
				spikes = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Stealth Rock")
				stealth_rock = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Toxic Spikes")
				toxic_spikes = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Members")
				members = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "HP")
				hp = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Aqua Ring")
				aqua_ring = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Curse")
				curse = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Imprison")
				imprison = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Ingrain")
				ingrain = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Leech Seed")
				leech_seed = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Loaf")
				loaf = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Nightmare")
				nightmare = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Torment")
				torment = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Trapped")
				trapped = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Burn")
				burn = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Freeze")
				freeze = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Paralysis")
				paralysis = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Poison")
				poison = boost::lexical_cast<int> (line.substr (x + 1));
			else if (data == "Sleep")
				sleep = boost::lexical_cast<int> (line.substr (x + 1));
		}
		file.close();
	}
};

long evaluate (const Team &ai, const Team &foe, const Weather &weather, const score_variables &sv);
long scorepokemon (const pokemon &member, const Team &ai, const Team &foe, const Weather &weather, const score_variables &sv);
long scoremove (const moves &move, const Team &ai, const Team &foe, const Weather &weather, const score_variables &sv);
long int win (const Team &team);

}
#endif
