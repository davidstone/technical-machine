// Reverse damage calculator
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

#include "reversedamage.hpp"

#include <cstdint>
#include <vector>

#include "damage.hpp"
#include "item.hpp"
#include "pokemon.hpp"
#include "stat.hpp"
#include "team.hpp"
#include "weather.hpp"
#include "unknown.hpp"

namespace technicalmachine {

/*
This function takes damage and calculates all possible values that can cause that damage. It removes any element from the list that is unable to cause a given damage. Given enough data points, the hidden values are reduced to a single data point.
*/

void reversedamagecalculator (Team & attacker, Team const & defender, Weather const & weather, unsigned damage, std::vector<Unknown> & hidden) {

	attacker.pokemon->hp.iv = 31;
	attacker.pokemon->atk.iv = 31;
	attacker.pokemon->def.iv = 31;
	attacker.pokemon->spa.iv = 31;
	attacker.pokemon->spd.iv = 31;
	attacker.pokemon->spe.iv = 31;
	
	std::vector<Unknown> refined_hidden;
	Item old_item;
	uint8_t old_spe_ev = 32;		// Illegal value to force recalculation of speed stat
	uint8_t old_offense_ev = 32;	// Illegal value to force recalculation of attacking stat
	
	unsigned const effectiveness = get_effectiveness (attacker.pokemon->move->type, *defender.pokemon);
	std::vector <unsigned> const effectiveness_vector = get_effectiveness_variables (attacker.pokemon->move->type, *defender.pokemon);

	unsigned rl;						// Reflect / Light Screen (2)
	unsigned weather_mod;		// Sunny Day / Rain Dance (1 if weakened, 3 if strengthened) / 2
	unsigned ff;					// Flash Fire: 3 / 2
	unsigned mf;					// Me First: 3 / 2
	unsigned known;			// Never used unitialized. If variable is true, set before entering the loop. If variable is false, set after entering the loop but before it's used in the loop.
	bool const variable = attacker.pokemon->move->name != Move::HIDDEN_POWER and attacker.pokemon->move->name != Move::NATURAL_GIFT;
	if (variable)
		known = damageknown (attacker, defender, weather, rl, weather_mod, ff, mf);

	unsigned stab;		// Same Type Attack Bonus: 3 / 2
	unsigned aem;		// Ability Effectiveness Multiplier: Solid Rock (3), Filter (3) / 4
	unsigned eb;		// Expert Belt: 6 / 5
	unsigned tl;			// Tinted Lens (2)
	unsigned rb;		// Resistance berries (2)
	unsigned nonrandom;

	for (Unknown const & unknown : hidden) {
		attacker.pokemon->item.name = static_cast<Item::Items> (unknown.item);
		attacker.pokemon->nature.name = static_cast<Nature::Natures> (unknown.nature);
		attacker.pokemon->hp.ev = unknown.hpev;
		attacker.pokemon->atk.ev = unknown.atkev;
		attacker.pokemon->spe.ev = unknown.speev;
		bool const different_item = old_item.name != attacker.pokemon->item.name;
		if (old_spe_ev != attacker.pokemon->spe.ev or different_item) {
			calculate_speed (attacker, weather);
			old_spe_ev = attacker.pokemon->spe.ev;
		}
		if (different_item) {
			movepower (attacker, defender, weather);
			if (!variable)
				known = damageknown (attacker, defender, weather, rl, weather_mod, ff, mf);
			old_item.name = attacker.pokemon->item.name;
		}
		if (old_offense_ev != attacker.pokemon->atk.ev or different_item) {
			calculate_attacking_stat (attacker, weather);
			old_offense_ev = attacker.pokemon->atk.ev;
			nonrandom = damagenonrandom (attacker, defender, rl, weather_mod, ff, mf, stab, effectiveness, aem, eb, tl, rb, known);
		}

		
		// First check to see if the damage is higher than it can possibly be, then check to see if it's lower than it can possibly be, then binary search the remainder. Profiling showed the removal of the "too high" and "too low" damage points to be valid optimizations.
		
		attacker.pokemon->move->r = 85;
		unsigned estimate = damagerandom (*attacker.pokemon, defender, stab, effectiveness_vector, aem, eb, tl, rb, nonrandom);
		if (estimate >= damage) {
			if (estimate == damage)
				refined_hidden.push_back (unknown);
			continue;
		}
		attacker.pokemon->move->r = 100;
		estimate = damagerandom (*attacker.pokemon, defender, stab, effectiveness_vector, aem, eb, tl, rb, nonrandom);
		if (estimate <= damage) {
			if (estimate == damage)
				refined_hidden.push_back (unknown);
			continue;
		}
		
		int high = 99;
		int low = 86;
		bool found = false;
		
		// Mathematical analysis shows that checking r == 93 first has an average case of 3.09 calculations (based on the probability distribution of various values for r in the damage formula), as opposed to 3.23 for r == 91, r == 92, or r == 94. The worst case for r == 93 is 4 calculations, which happens 34% of the time, compared to r == 91 which has a worst case of 5 calculations 5.7% of the time and 4 calculations 43% if the time. r == 92 or r == 94 also have worst case scenarios more likely than r == 93. The average time reduction for r == 93 over its best competitor is 4.3%, assuming an r value generates a legal damage. If it does not, it has the best worst-case performance, so it is still the fastest.
		attacker.pokemon->move->r = 93;
		while (low <= high) {
			estimate = damagerandom (*attacker.pokemon, defender, stab, effectiveness_vector, aem, eb, tl, rb, nonrandom);
			if (estimate > damage)
				high = attacker.pokemon->move->r - 1;
			else if (estimate < damage)
				low = attacker.pokemon->move->r + 1;
			else {
				found = true;
				break;
			}
			attacker.pokemon->move->r = (high + low) / 2;
		}
		if (found)
			refined_hidden.push_back (unknown);
	}
	hidden = refined_hidden;
}

}
