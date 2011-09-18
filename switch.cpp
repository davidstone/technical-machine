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

#include <utility>
#include <vector>
#include "switch.h"
#include "ability.h"
#include "heal.h"
#include "pokemon.h"
#include "status.h"
#include "team.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

void reset_variables (Team &team) {
	// Reset all variables that switches reset.

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

void switchpokemon (Team &switcher, Team &other, Weather &weather) {
	if (switcher.pokemon->hp.stat == 0) {
		reset_variables (switcher);
		// First, remove the active Pokemon because it has 0 HP.
		switcher.pokemon.set.erase (switcher.pokemon.set.begin() + switcher.pokemon.index);
		--switcher.size;

		// If the last Pokemon is fainted; there is nothing left to do.
		if (switcher.pokemon.set.size() == 0)
			return;

		// Then, remove the ability to bring out that Pokemon from Roar and Whirlwind in the foe's team.
		for (std::vector<Pokemon>::iterator pokemon = other.pokemon.set.begin(); pokemon != other.pokemon.set.end(); ++pokemon) {
			for (std::vector<Move>::iterator move = pokemon->move.set.begin(); move != pokemon->move.set.end(); ++move) {
				if (move->is_phaze ()) {
					move->variable.set.pop_back();
					for (std::vector <std::pair <unsigned short, unsigned short> >::iterator variable = move->variable.set.begin(); variable != move->variable.set.end(); ++variable) {
						if (switcher.size > 2)
							variable->second = Move::max_probability / (switcher.size - 1);
						else
							variable->second = Move::max_probability;
					}
				}
			}
		}
		if (switcher.pokemon.index > switcher.replacement)
			switcher.pokemon.index = switcher.replacement;
		else
			switcher.pokemon.index = switcher.replacement - 1;
		// Finally, remove the ability to switch to that Pokemon.
		for (std::vector<Pokemon>::iterator pokemon = switcher.pokemon.set.begin(); pokemon != switcher.pokemon.set.end(); ++pokemon)
			pokemon->move.set.pop_back();
	}
	else {
		// Cure the status of a Natural Cure Pokemon as it switches out
		if (switcher.pokemon->ability.name == Ability::NATURAL_CURE)
			switcher.pokemon->status = NO_STATUS;
		
		reset_variables (switcher);
	
		// Change the active Pokemon to the one switching in.
		switcher.pokemon.index = switcher.replacement;
	}
	
	entry_hazards (switcher, weather);

	if (switcher.pokemon->hp.stat > 0)
		activate_ability (switcher, *other.pokemon, weather);
}

void entry_hazards (Team &switcher, Weather const &weather) {
	if (grounded (switcher, weather) and switcher.pokemon->ability.name != Ability::MAGIC_GUARD) {
		if (switcher.toxic_spikes != 0) {
			if (is_type (switcher, POISON))
				switcher.toxic_spikes = 0;
			else if (switcher.toxic_spikes == 1)
				poison_normal (switcher, switcher, weather);
			else
				poison_toxic (switcher, switcher, weather);
		}
		if (switcher.spikes != 0)
			heal (*switcher.pokemon, -16, switcher.spikes + 1);
	}
	// get_effectiveness () outputs a value between 0 and 16, with higher numbers being more effective. 4 * effective Stealth Rock does 16 / 32 damage.
	if (switcher.stealth_rock)
		heal (*switcher.pokemon, -32, get_effectiveness (ROCK, *switcher.pokemon));
}

void activate_ability (Team &switcher, Pokemon &other, Weather &weather) {
	// Activate abilities upon switching in

	switcher.slow_start = 0;
	switch (switcher.pokemon->ability.name) {
		case Ability::SLOW_START:
			switcher.slow_start = 5;
			break;
		case Ability::DOWNLOAD:
			if (other.def.stat >= other.spd.stat)
				switcher.pokemon->spa.boost (1);
			else
				switcher.pokemon->atk.boost (1);
			break;
		case Ability::DRIZZLE:
			weather.set_rain (-1);
			break;
		case Ability::DROUGHT:
			weather.set_sun (-1);
			break;
		case Ability::FORECAST:	// fix
			break;
		case Ability::INTIMIDATE:
			other.atk.boost (-1);
			break;
		case Ability::SAND_STREAM:
			weather.set_sand (-1);
			break;
		case Ability::SNOW_WARNING:
			weather.set_hail (-1);
			break;
		case Ability::TRACE:
			break;
		default:
			break;
	}
}

}
