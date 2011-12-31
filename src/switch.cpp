// Switching-related functions
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

#include "switch.hpp"

#include <utility>
#include <vector>

#include "ability.hpp"
#include "heal.hpp"
#include "pokemon.hpp"
#include "stat.hpp"
#include "status.hpp"
#include "team.hpp"
#include "type.hpp"
#include "weather.hpp"

namespace technicalmachine {

static void entry_hazards (Team & switcher, Weather const & weather);
static void activate_ability (Team & switcher, Team & other, Weather & weather);

void reset_variables (Team & team) {
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
		for (Stat::Stats stat = Stat::ATK; stat != Stat::END_STAT; stat = static_cast <Stat::Stats> (stat + 1))
			team.stage [stat] = 0;
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
	team.pass = false;
	team.roost = false;
	team.torment = false;
	team.trapped = false;
	team.u_turning = false;
	team.water_sport = false;
	team.bide = 0;
	team.encore = 0;
	team.heal_block = 0;
	team.partial_trap = 0;
	team.rampage = 0;
	team.stockpile = 0;
	team.taunt = 0;
	team.toxic = 0;
	team.uproar = 0;
	team.vanish = LANDED;	// Whirlwind can hit Flying Pokemon, so it needs to be reset
	team.yawn = 0;

	for (Move & move : team.pokemon().move.set) {
		move.disable = 0;
		move.times_used = 0;
	}
}

void switchpokemon (Team & switcher, Team & other, Weather & weather) {
	if (switcher.pokemon().hp.stat == 0) {
		reset_variables (switcher);
		// First, remove the active Pokemon because it has 0 HP.
		switcher.pokemon.set.erase (switcher.pokemon.set.begin() + switcher.pokemon.index);
		--switcher.size;

		// If the last Pokemon is fainted; there is nothing left to do.
		if (switcher.pokemon.set.size() == 0)
			return;

		// Then, remove the ability to bring out that Pokemon from Roar and Whirlwind in the foe's team.
		for (Pokemon & pokemon : other.pokemon.set) {
			for (Move & move : pokemon.move.set) {
				if (move.is_phaze ()) {
					move.variable.set.pop_back();
					for (std::pair <uint16_t, uint16_t> & variable : move.variable.set) {
						if (switcher.size > 2)
							variable.second = Move::max_probability / (switcher.size - 1);
						else
							variable.second = Move::max_probability;
					}
				}
			}
		}
		if (switcher.pokemon.index > switcher.replacement)
			switcher.pokemon.index = switcher.replacement;
		else
			switcher.pokemon.index = switcher.replacement - 1;
		// Finally, remove the ability to switch to that Pokemon.
		for (Pokemon & pokemon : switcher.pokemon.set) {
			pokemon.move.set.pop_back();
			// If there is only one Pokemon, there is no switching.
			if (switcher.pokemon.set.size () == 1)
				pokemon.move.set.pop_back ();
		}
	}
	else {
		// Cure the status of a Natural Cure Pokemon as it switches out
		if (switcher.pokemon().ability.name == Ability::NATURAL_CURE)
			switcher.pokemon().status.clear ();
		
		reset_variables (switcher);
	
		// Change the active Pokemon to the one switching in.
		switcher.pokemon.index = switcher.replacement;
	}
	
	entry_hazards (switcher, weather);

	if (switcher.pokemon().hp.stat > 0)
		activate_ability (switcher, other, weather);
}

void entry_hazards (Team & switcher, Weather const & weather) {
	if (grounded (switcher, weather) and switcher.pokemon().ability.name != Ability::MAGIC_GUARD) {
		if (switcher.toxic_spikes != 0) {
			if (is_type (switcher, Type::POISON))
				switcher.toxic_spikes = 0;
			else if (switcher.toxic_spikes == 1)
				Status::poison (switcher, switcher, weather);
			else
				Status::poison_toxic (switcher, switcher, weather);
		}
		if (switcher.spikes != 0)
			heal (switcher.pokemon(), -16, switcher.spikes + 1);
	}
	// get_effectiveness () outputs a value between 0 and 16, with higher numbers being more effective. 4 * effective Stealth Rock does 16 / 32 damage.
	if (switcher.stealth_rock)
		heal (switcher.pokemon(), -32, get_effectiveness (Type::ROCK, switcher.pokemon()));
}

void activate_ability (Team & switcher, Team & other, Weather & weather) {
	// Activate abilities upon switching in

	switcher.slow_start = 0;
	switch (switcher.pokemon().ability.name) {
		case Ability::SLOW_START:
			switcher.slow_start = 5;
			break;
		case Ability::DOWNLOAD:
			calculate_defense (switcher, other, weather);
			calculate_special_defense (switcher, other, weather);
			if (other.pokemon().def.stat >= other.pokemon().spd.stat)
				Stat::boost (switcher.stage [Stat::SPA], 1);
			else
				Stat::boost (switcher.stage [Stat::ATK], 1);
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
			Stat::boost (other.stage [Stat::ATK], -1);
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
