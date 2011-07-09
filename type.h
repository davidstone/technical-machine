// Type information
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TYPE_H_
#define TYPE_H_

namespace technicalmachine {

enum types { BUG, DARK, DRAGON, ELECTRIC, FIGHTING, FIRE, FLYING, GHOST, GRASS, GROUND, ICE, NORMAL, POISON, PSYCHIC_TYPE, ROCK, STEEL, WATER, TYPELESS };

// Calculate the effectiveness of attacking type on defending type. 0 is no effect, 1 is not very effective, 2 is neutral, and 4 is super effective.

int const effectiveness [18][18] = {
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

class Team;
class Weather;

bool istype (Team const &team, types type);
bool grounded (Team const &team, Weather const &weather);

}
#endif
