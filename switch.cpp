// Switching-related functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <vector>
#include "pokemon.h"
#include "simple.h"
#include "statfunction.h"
#include "statusfunction.h"
#include "switch.h"
#include "team.h"
#include "weather.h"

#include <iostream>

namespace technicalmachine {

void reset_variables (Pokemon &member) {
	//  Initialize all variables that switches reset.
	member.aqua_ring = false;
	member.attract = false;
	member.charge = false;
	member.curse = false;
	member.damaged = false;
	member.defense_curl = false;
	member.destiny_bond = false;
	member.ff = false;
	member.flinch = false;
	member.focus_energy = false;
	member.identified = false;
	member.imprison = false;
	member.ingrain = false;
	member.leech_seed = false;
	member.loaf = false;			// Do I set to true or false? True makes it wrong when a fainted Pokemon is replaced; false makes it wrong otherwise
	member.lock_on = false;
	member.mf = false;
	member.minimize = false;
	member.mud_sport = false;
	member.nightmare = false;
	member.roost = false;
	member.torment = false;
	member.trapped = false;
	member.water_sport = false;
	member.bide = 0;
	member.confused = 0;
	member.embargo = 0;
	member.encore = 0;
	member.heal_block = 0;
	member.magnet_rise = 0;
	member.partial_trap = 0;
	member.perish_song = 0;
	member.rampage = 0;
	member.atk.stage = 0;
	member.def.stage = 0;
	member.spa.stage = 0;
	member.spd.stage = 0;
	member.spe.stage = 0;
	member.accuracy = 0;
	member.evasion = 0;
	member.stockpile = 0;
	member.taunt = 0;
	member.toxic = 0;
	member.uproar = 0;
	member.vanish = LANDED;
	member.yawn = 0;

	for (std::vector<Move>::iterator it = member.move.set.begin(); it != member.move.set.end(); ++it) {
		it->disable = 0;
		it->times_used = 0;
	}
}

void switchpokemon (Team &user, Pokemon &target, Weather &weather) {
	if (user.active->hp.stat == 0) {
		user.active.set.erase (user.active.set.begin() + user.active.index);
		if (user.active.set.size() == 0)		// The last Pokemon is fainted; there is nothing left to do.
			return;
		if (user.active.index > user.replacement)
			user.active.index = user.replacement;
		else
			user.active.index = user.replacement - 1;
		for (std::vector<Pokemon>::iterator active = user.active.set.begin(); active != user.active.set.end(); ++active)
			active->move.set.pop_back();		// You cannot switch to a fainted Pokemon
	}
	else {
		// Cure the status of a Natural Cure Pokemon as it switches out
		if (NATURAL_CURE == user.active->ability)
			user.active->status = NO_STATUS;
		
		reset_variables (*user.active);
	
		// Change the active Pokemon to the one switching in.
		user.active.index = user.replacement;
	}
	
	if (grounded (*user.active, weather) and MAGIC_GUARD != user.active->ability) {
		if (0 != user.toxic_spikes) {
			if (istype(*user.active, POISON))
				user.toxic_spikes = 0;
			else if (1 == user.toxic_spikes)
				poison_normal (*user.active, *user.active, weather);
			else
				poison_toxic (*user.active, *user.active, weather);
		}
		if (0 != user.spikes)
			heal (*user.active, -16, user.spikes + 1);
	}
	if (user.stealth_rock)
		heal (*user.active, -32, effectiveness [ROCK] [user.active->type1] * effectiveness [ROCK] [user.active->type2]);	// effectiveness [][] outputs a value between 0 and 4, with higher numbers being more effective, meaning effectiveness [][] * effectiveness [][] is a value between 0 and 16. 4 * effective Stealth Rock does 16 / 32 damage.
	
	if (user.active->hp.stat > 0) {
		if (SLOW_START == user.active->ability)
			user.active->slow_start = 5;
		else
			user.active->slow_start = 0;
	
		// Activate abilities upon switching in
	
		if (user.active->ability == DOWNLOAD) {
			if (target.def.stat >= target.spd.stat)
				statboost (user.active->spa.stage, 1);
			else
				statboost (user.active->atk.stage, 1);
		}
		else if (user.active->ability == DRIZZLE)
			rain (weather, -1);
		else if (user.active->ability == DROUGHT)
			sun (weather, -1);
		else if (user.active->ability == FORECAST) {	// fix
		}
		else if (user.active->ability == INTIMIDATE)
			statboost (target.atk.stage, -1);
		else if (user.active->ability == SAND_STREAM)
			sand (weather, -1);
		else if (user.active->ability == SNOW_WARNING)
			hail (weather, -1);
		else if (user.active->ability == TRACE) {
		}
	}
}

}
