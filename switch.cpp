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
#include <iostream>
#include "heal.h"
#include "pokemon.h"
#include "statfunction.h"
#include "statusfunction.h"
#include "switch.h"
#include "team.h"
#include "typefunction.h"
#include "weather.h"

namespace technicalmachine {

void reset_variables (Team &team) {
	//  Reset all variables that switches reset.

	if (!team.pass) {
		team.aqua_ring = false;
		team.curse = false;
		team.focus_energy = false;
		team.gastro_acid = false;
		team.ingrain = false;
		team.leech_seed = false;
		team.lock_on = false;
		team.power_trick = false;
		team.confused = 0;
		team.embargo = 0;
		team.magnet_rise = 0;
		team.perish_song = 0;
		team.accuracy = 0;
		team.evasion = 0;
		team.substitute = 0;
	}
	team.attract = false;
	team.charge = false;
	team.damaged = false;
	team.defense_curl = false;
	team.destiny_bond = false;
	team.ff = false;
	team.flinch = false;
	team.identified = false;
	team.imprison = false;
	team.loaf = false;			// Do I set to true or false? True makes it wrong when a fainted Pokemon is replaced; false makes it wrong otherwise
	team.minimize = false;
	team.mf = false;
	team.mud_sport = false;
	team.nightmare = false;
	team.roost = false;
	team.torment = false;
	team.trapped = false;
	team.water_sport = false;
	team.bide = 0;
	team.encore = 0;
	team.heal_block = 0;
	team.partial_trap = 0;
	team.rampage = 0;
	team.pokemon->atk.stage = 0;
	team.pokemon->def.stage = 0;
	team.pokemon->spa.stage = 0;
	team.pokemon->spd.stage = 0;
	team.pokemon->spe.stage = 0;
	team.stockpile = 0;
	team.taunt = 0;
	team.toxic = 0;
	team.uproar = 0;
	team.vanish = LANDED;	// Whirlwind can hit Flying Pokemon, so it needs to be reset
	team.yawn = 0;

	for (std::vector<Move>::iterator it = team.pokemon->move.set.begin(); it != team.pokemon->move.set.end(); ++it) {
		it->disable = 0;
		it->times_used = 0;
	}
}

void switchpokemon (Team &user, Team &target, Weather &weather) {
	if (user.pokemon->hp.stat == 0) {
		reset_variables (user);
		// First, remove the active Pokemon because it has 0 HP.
		user.pokemon.set.erase (user.pokemon.set.begin() + user.pokemon.index);
		--user.size;

		// If the last Pokemon is fainted; there is nothing left to do.
		if (user.pokemon.set.size() == 0)
			return;

		// Then, remove the ability to bring out that Pokemon from Roar and Whirlwind in the foe's team.
		for (std::vector<Pokemon>::iterator pokemon = target.pokemon.set.begin(); pokemon != target.pokemon.set.end(); ++pokemon) {
			for (std::vector<Move>::iterator move = pokemon->move.set.begin(); move != pokemon->move.set.end(); ++move) {
				if (move->name == ROAR or move->name == WHIRLWIND)
					move->variable.set.pop_back();
			}
		}
		if (user.pokemon.index > user.replacement)
			user.pokemon.index = user.replacement;
		else
			user.pokemon.index = user.replacement - 1;
		// Finally, remove the ability to switch to that Pokemon.
		for (std::vector<Pokemon>::iterator pokemon = user.pokemon.set.begin(); pokemon != user.pokemon.set.end(); ++pokemon)
			pokemon->move.set.pop_back();
	}
	else {
		// Cure the status of a Natural Cure Pokemon as it switches out
		if (NATURAL_CURE == user.pokemon->ability)
			user.pokemon->status = NO_STATUS;
		
		reset_variables (user);
	
		// Change the active Pokemon to the one switching in.
		user.pokemon.index = user.replacement;
	}
	
	entry_hazards (user, weather);

	if (user.pokemon->hp.stat > 0)
		activate_ability (user, *target.pokemon, weather);
}

void entry_hazards (Team &user, Weather const &weather) {
	if (grounded (user, weather) and MAGIC_GUARD != user.pokemon->ability) {
		if (user.toxic_spikes != 0) {
			if (istype(user, POISON))
				user.toxic_spikes = 0;
			else if (1 == user.toxic_spikes)
				poison_normal (user, user, weather);
			else
				poison_toxic (user, user, weather);
		}
		if (user.spikes != 0)
			heal (*user.pokemon, -16, user.spikes + 1);
	}
	if (user.stealth_rock)
		heal (*user.pokemon, -32, effectiveness [ROCK] [user.pokemon->type1] * effectiveness [ROCK] [user.pokemon->type2]);	// effectiveness [][] outputs a value between 0 and 4, with higher numbers being more effective, meaning effectiveness [][] * effectiveness [][] is a value between 0 and 16. 4 * effective Stealth Rock does 16 / 32 damage.
}

void activate_ability (Team &user, Pokemon &target, Weather &weather) {
		// Activate abilities upon switching in

		user.slow_start = 0;
		if (user.pokemon->ability == SLOW_START)
			user.slow_start = 5;
		else if (user.pokemon->ability == DOWNLOAD) {
			if (target.def.stat >= target.spd.stat)
				user.pokemon->spa.boost (1);
			else
				user.pokemon->atk.boost (1);
		}
		else if (user.pokemon->ability == DRIZZLE)
			weather.set_rain (-1);
		else if (user.pokemon->ability == DROUGHT)
			weather.set_sun (-1);
		else if (user.pokemon->ability == FORECAST) {	// fix
		}
		else if (user.pokemon->ability == INTIMIDATE)
			target.atk.boost (-1);
		else if (user.pokemon->ability == SAND_STREAM)
			weather.set_sand (-1);
		else if (user.pokemon->ability == SNOW_WARNING)
			weather.set_hail (-1);
		else if (user.pokemon->ability == TRACE) {
		}
}

}
