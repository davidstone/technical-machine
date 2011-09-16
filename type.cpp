// Type function definitions
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
#include "team.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

bool is_type (Team const & team, Type type) {
	for (std::vector <Type>::const_iterator it = team.pokemon->type.begin(); it != team.pokemon->type.end(); ++it) {
		if (*it == type and (type != FLYING or !team.roost))
			return true;
	}
	return false;
}

// Calculate the effectiveness of attacking type on defending type. 0 is no effect, 1 is not very effective, 2 is neutral, and 4 is super effective.

unsigned lookup_effectiveness (Type attacking, Type defending) {
	static unsigned const effectiveness [18][18] = {
		{	2,	4,	2,	2,	1,	1,	1,	1,	4,	2,	2,	2,	1,	4,	2,	1,	2,	2	},	// Bug
		{	2,	1,	2,	2,	1,	2,	2,	4,	2,	2,	2,	2,	2,	4,	2,	1,	2,	2	},	// Dark
		{	2,	2,	4,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	1,	2,	2	},	// Dragon
		{	2,	2,	1,	1,	2,	2,	4,	2,	1,	0,	2,	2,	2,	2,	2,	2,	4,	2	},	// Electric
		{	1,	4,	2,	2,	2,	2,	1,	0,	2,	2,	4,	4,	1,	1,	4,	4,	2,	2	},	// Fighting
		{	4,	2,	1,	2,	2,	1,	2,	2,	4,	2,	4,	2,	2,	2,	1,	4,	1,	2	},	// Fire
		{	4,	2,	2,	1,	4,	2,	2,	2,	4,	2,	2,	2,	2,	2,	1,	1,	2,	2	},	// Flying
		{	2,	1,	2,	2,	2,	2,	2,	4,	2,	2,	2,	0,	2,	4,	2,	1,	2,	2	},	// Ghost
		{	1,	2,	1,	2,	2,	1,	1,	2,	1,	4,	2,	2,	1,	2,	4,	1,	4,	2	},	// Grass
		{	1,	2,	2,	4,	2,	4,	0,	2,	1,	2,	2,	2,	4,	2,	4,	4,	2,	2	},	// Ground
		{	2,	2,	4,	2,	2,	1,	4,	2,	4,	4,	1,	2,	2,	2,	2,	1,	1,	2	},	// Ice
		{	2,	2,	2,	2,	2,	2,	2,	0,	2,	2,	2,	2,	2,	2,	1,	1,	2,	2	},	// Normal
		{	2,	2,	2,	2,	2,	2,	2,	1,	4,	1,	2,	2,	1,	2,	1,	0,	2,	2	},	// Poison
		{	2,	0,	2,	2,	4,	2,	2,	2,	2,	2,	2,	2,	4,	1,	2,	1,	2,	2	},	// Psychic
		{	4,	2,	2,	2,	1,	4,	4,	2,	2,	1,	4,	2,	2,	2,	2,	1,	2,	2	},	// Rock
		{	2,	2,	2,	1,	2,	1,	2,	2,	2,	2,	4,	2,	2,	2,	4,	1,	1,	2	},	// Steel
		{	2,	2,	1,	2,	2,	4,	2,	2,	1,	4,	2,	2,	2,	2,	4,	2,	1,	2	},	// Water
		{	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2,	2	}	// Typeless
		//	Bug	Drk	Drg	Elc	Ftg	Fir	Fly	Gho	Grs	Grd	Ice	Nrm	Psn	Psy	Rck	Stl	Wtr	Typ		
	};
	return effectiveness [attacking] [defending];
}

bool get_effectiveness (Type type, Pokemon const & pokemon) {
	unsigned effectiveness = 1;
	// Effectiveness on each of the defender's type (1 if NVE, 4 if SE) / 2
	for (std::vector <Type>::const_iterator it = pokemon.type.begin(); it != pokemon.type.end(); ++it)
		effectiveness *= lookup_effectiveness (type, *it);
	return effectiveness;
}

std::vector <unsigned> get_effectiveness_variables (Type type, Pokemon const & pokemon) {
	std::vector <unsigned> effectiveness;
	// Effectiveness on each of the defender's type (1 if NVE, 4 if SE) / 2
	for (std::vector <Type>::const_iterator it = pokemon.type.begin(); it != pokemon.type.end(); ++it)
		effectiveness.push_back (lookup_effectiveness (type, *it));
	return effectiveness;
}

bool grounded (Team const & team, Weather const & weather) {
	return (!is_type (team, FLYING) and team.pokemon->ability != LEVITATE and team.magnet_rise == 0) or weather.gravity != 0 or team.pokemon->item == IRON_BALL or team.ingrain;
}

}
