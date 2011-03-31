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

#include "move.h"
#include "pokemon.h"
#include "simple.h"
#include "status.h"
#include "team.h"
#include "weather.h"

namespace tm {

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
};

long evaluate (const teams &ai, const teams &foe, const weathers &weather, const score_variables &sv);
long scorepokemon (const pokemon &member, const teams &ai, const teams &foe, const weathers &weather, const score_variables &sv);
long scoremove (const moves &move, const teams &ai, const teams &foe, const weathers &weather, const score_variables &sv);
long int win (const teams &team);

}
#endif
