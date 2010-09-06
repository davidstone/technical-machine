// Reverse damage calculator
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <vector>
#include "damage.cpp"
#include "item.h"
#include "team.h"
#include "weather.h"

/*
This function takes damage and calculates all possible values that can cause that damage. It removes any element from the list that is unable to cause a given damage. Given enough data points, the hidden values are reduced to a single data point.
*/

void reversedamagecalculator (teams &attacker, const teams &defender, const weathers &weather, int damage, std::vector<Unknown> &hidden) {

	attacker.active->hp.iv = 31;
	attacker.active->atk.iv = 31;
	attacker.active->def.iv = 31;
	attacker.active->spa.iv = 31;
	attacker.active->spd.iv = 31;
	attacker.active->spe.iv = 31;
	
	std::vector<Unknown> refined_hidden;
	items old_item = END_ITEM;
	unsigned char old_spe_ev = 32;		// Illegal value to force recalculation of speed stat
	unsigned char old_offense_ev = 32;	// Illegal value to force recalculation of attacking stat
	
	int type1 = effectiveness [attacker.active->move->type] [defender.active->type1];		// Effectiveness on the defender's first type (1 if NVE, 4 if SE) / 2
	int type2 = effectiveness [attacker.active->move->type] [defender.active->type2];		// Effectiveness on the defender's second type (1 if NVE, 4 if SE) / 2
	for (std::vector<Unknown>::const_iterator it = hidden.begin(); it != hidden.end(); ++it) {
		attacker.active->item = static_cast<items> (it->item);
		attacker.active->nature = static_cast<natures> (it->nature);
		attacker.active->hp.ev = it->hpev;
		attacker.active->atk.ev = it->atkev;
		if (old_offense_ev != attacker.active->atk.ev) {
			attack (*attacker.active, weather);
			old_offense_ev = attacker.active->atk.ev;
		}
		attacker.active->spe.ev = it->speev;
		if (old_spe_ev != attacker.active->spe.ev) {
			speed (attacker, weather);
			old_spe_ev = attacker.active->spe.ev;
		}
		if (old_item != attacker.active->item) {
			movepower (*attacker.active, *defender.active, weather);
			old_item = attacker.active->item;
		}
		int stab;		// Same Type Attack Bonus: 3 / 2
		int aem;		// Ability Effectiveness Multiplier: Solid Rock (3), Filter (3) / 4
		int eb;		// Expert Belt: 6 / 5
		int tl;			// Tinted Lens (2)
		int rb;		// Resistance berries (2)
		// I don't check for the move not having an effect on the target because this function is only called if damage != 0
		int nonrandom = damagenonrandom (*attacker.active, defender, weather, stab, type1, type2, aem, eb, tl, rb);
		
		// First check to see if the damage is higher than it can possibly be, then check to see if it's lower than it can possibly be, then binary search the remainder. Profiling showed the removal of the "too high" and "too low" damage points to be valid optimizations.
		
		attacker.active->move->r = 85;
		int temp = damagerandom (*attacker.active, defender, stab, type1, type2, aem, eb, tl, rb, nonrandom);
		if (temp == damage)
			refined_hidden.push_back (*it);
		if (temp >= damage)
			continue;
		attacker.active->move->r = 100;
		temp = damagerandom (*attacker.active, defender, stab, type1, type2, aem, eb, tl, rb, nonrandom);
		if (temp == damage)
			refined_hidden.push_back (*it);
		if (temp <= damage)
			continue;
		
		int high = 99;
		int low = 86;
		bool found = false;
		
		// Mathematical analysis shows that checking r == 93 first has an average case of 3.09 calculations, as opposed to 3.23 for r == 91 or 3.23 for r == 92 or r == 94. The worst case for r == 93 is 4 calculations, which happens 34% of the time, compared to r == 91 which has a worst case of 5 calculations 5.7% of the time and 4 calculations 43% if the time. r == 92 or r == 94 also have worst case scenarios more likely than r == 93. The average time reduction for r == 93 over its best competitor is 4.3%, assuming an r value generates a legal damage. If it does not, it has the lowest worst-case performance, so it is still the fastest.
		attacker.active->move->r = 93;
		while (low <= high) {
			int estimate = damagerandom (*attacker.active, defender, stab, type1, type2, aem, eb, tl, rb, nonrandom);
			if (estimate > damage)
				high = attacker.active->move->r - 1;
			else if (estimate < damage)
				low = attacker.active->move->r + 1;
			else {
				found = true;
				break;
			}
			attacker.active->move->r = (high + low) / 2;
		}
		if (found)
			refined_hidden.push_back (*it);
	}
	hidden = refined_hidden;
}
