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
#include "status.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

long const VICTORY = 65536;		// Highest score possible. 100% chance to win.

class score_variables {
	public:
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
		int magnet_rise;
		int nightmare;
		int substitute;
		int torment;
		int trapped;

		int burn;
		int freeze;
		int paralysis;
		int poison;
		int sleep;
		
		int atk_stage;
		int def_stage;
		int spa_stage;
		int spd_stage;
		int spe_stage;
		int focus_energy;
	
		int baton_pass;
		int no_pp;
	
		score_variables ();
};

long evaluate (Team const &ai, Team const &foe, Weather const &weather, score_variables const &sv);
long scoreteam (Team const &team, score_variables const &sv);
long scorepokemon (Pokemon const &member, Team const &ai, Team const &foe, Weather const &weather, score_variables const &sv);
long scoremove (Pokemon const &member, Team const &ai, Team const &foe, Weather const &weather, score_variables const &sv);
long win (Team const &team);

}
#endif
