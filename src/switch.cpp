// Switching-related functions
// Copyright (C) 2012 David Stone
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
namespace {

void reset_variables (Team & team);
void entry_hazards (Team & switcher, Weather const & weather);
void replace_fainted_pokemon (Team & switcher, Team & other);
void remove_fainted_from_phazing_moves (Team const & switcher, Team & other);
void remove_ability_to_switch_to_fainted (Team & switcher);

}	// unnamed namespace

void switchpokemon (Team & switcher, Team & other, Weather & weather) {
	reset_variables (switcher);

	if (switcher.pokemon().hp.stat > 0) {
		// Cure the status of a Natural Cure Pokemon as it switches out
		if (switcher.pokemon().ability.clears_status_on_switch ())
			switcher.pokemon().status.clear ();
		
		// Change the active Pokemon to the one switching in.
		switcher.pokemon.set_index(switcher.replacement);
	}
	else {
		replace_fainted_pokemon (switcher, other);
		// If the last Pokemon is fainted; there is nothing left to do.
		if (switcher.pokemon.is_empty())
			return;
	}
	
	entry_hazards (switcher, weather);

	if (switcher.pokemon().hp.stat > 0)
		Ability::activate_on_switch (switcher, other, weather);
}

namespace {

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
		for (Stat::Stats stat = Stat::ATK; stat != Stat::END; stat = static_cast <Stat::Stats> (stat + 1))
			team.stage [stat] = 0;
		team.substitute = 0;
	}
	team.attract = false;
	team.charge = false;
	team.damaged = false;
	team.defense_curl = false;
	team.destiny_bond = false;
	team.flash_fire = false;
	team.flinch = false;
	team.identified = false;
	team.imprison = false;
	// Do I set to true or false? true makes it wrong when a fainted Pokemon is
	// replaced; false makes it wrong otherwise
	team.loaf = false;
	team.minimize = false;
	team.me_first = false;
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
	team.slow_start = 0;
	team.stockpile = 0;
	team.taunt = 0;
	team.toxic = 0;
	team.uproar = 0;
	// Whirlwind can hit Flying Pokemon, so it needs to be reset
	team.vanish = LANDED;
	team.yawn = 0;

	team.pokemon().move.for_each([](Move & move) {
		move.reset();
	});
}

void replace_fainted_pokemon (Team & switcher, Team & other) {
	// First, remove the active Pokemon because it has 0 HP.
	switcher.pokemon.remove_active ();
	--switcher.size;

	// If the last Pokemon is fainted; there is nothing left to do.
	if (switcher.pokemon.is_empty())
		return;

	remove_fainted_from_phazing_moves(switcher, other);

	uint8_t const new_index = (switcher.pokemon.index() > switcher.replacement) ?
		switcher.replacement :
		switcher.replacement - 1;
	switcher.pokemon.set_index(new_index);

	remove_ability_to_switch_to_fainted(switcher);
}

void remove_fainted_from_phazing_moves (Team const & switcher, Team & other) {
	other.pokemon.for_each([& switcher](Pokemon & pokemon) {
		pokemon.move.for_each([& switcher](Move & move) {
			if (move.is_phaze ()) {
				move.variable.pop_back();
				move.variable.for_each([& switcher](std::pair<uint16_t, uint16_t> & variable) {
					variable.second = (switcher.size > 2) ?
						Move::max_probability / (switcher.size - 1) :
						Move::max_probability;
				});
			}
		});
	});
}

void remove_ability_to_switch_to_fainted (Team & switcher) {
	switcher.pokemon.for_each([& switcher](Pokemon & pokemon) {
		pokemon.move.pop_back();
		// If there is only one Pokemon, there is no switching.
		if (switcher.pokemon.size() == 1)
			pokemon.move.pop_back();
	});
}

void entry_hazards (Team & switcher, Weather const & weather) {
	if (switcher.pokemon().ability.blocks_secondary_damage())
		return;

	if (grounded (switcher, switcher.pokemon(), weather)) {
		if (switcher.toxic_spikes != 0) {
			if (is_type (switcher, Type::POISON))
				switcher.toxic_spikes = 0;
			else if (switcher.toxic_spikes == 1)
				Status::poison (switcher.pokemon(), switcher.pokemon(), weather);
			else
				Status::poison_toxic (switcher.pokemon(), switcher.pokemon(), weather);
		}
		if (switcher.spikes != 0)
			heal (switcher.pokemon(), -16, switcher.spikes + 1u);
	}
	// get_effectiveness () outputs a value between 0 and 16, with higher
	// numbers being more effective. 4 * effective Stealth Rock does
	// 16 / 32 damage.
	if (switcher.stealth_rock)
		heal (switcher.pokemon(), -32, Type::stealth_rock_effectiveness(switcher.pokemon()));
}

}	// unnamed namespace
}	// namespace technicalmachine
