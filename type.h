// Type information
// Copyright 2010 David Stone
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

enum types { BUG, DARK, DRAGON, ELECTRIC, FIGHTING, FIRE, FLYING, GHOST, GRASS, GROUND, ICE, NORMAL, POISON, PSYCHIC_TYPE, ROCK, STEEL, WATER, TYPELESS };

// Calculate the effectiveness of move_type on defender_type. 0 is no effect, 1 is not very effective, 2 is neutral, and 4 is super effective.

int effectiveness (types move_type, types defender_type) {
	
	int effectiveness;
	
	if (move_type == BUG) {
		if (defender_type == DARK or defender_type == GRASS or defender_type == PSYCHIC_TYPE)
			effectiveness = 4;
		else if (defender_type == FIGHTING or defender_type == FIRE or defender_type == FLYING or defender_type == GHOST or defender_type == POISON or defender_type == STEEL)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == DARK) {
		if (defender_type == GHOST or defender_type == PSYCHIC_TYPE)
			effectiveness = 4;
		else if (defender_type == DARK or defender_type == FIGHTING or defender_type == STEEL)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == DRAGON) {
		if (defender_type == DRAGON)
			effectiveness = 4;
		else if (defender_type == STEEL)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == ELECTRIC) {
		if (defender_type == FLYING or defender_type == WATER)
			effectiveness = 4;
		else if (defender_type == DRAGON or defender_type == ELECTRIC or defender_type == GRASS)
			effectiveness = 1;
		else if (defender_type == GROUND)
			effectiveness = 0;
		else
			effectiveness = 2;
	}
	
	else if (move_type == FIGHTING) {
		if (defender_type == DARK or defender_type == ICE or defender_type == NORMAL or defender_type == ROCK or defender_type == STEEL)
			effectiveness = 4;
		else if (defender_type == BUG or defender_type == FLYING or defender_type == POISON or defender_type == PSYCHIC_TYPE)
			effectiveness = 1;
		else if (defender_type == GHOST)
			effectiveness = 0;
		else
			effectiveness = 2;
	}
	
	else if (move_type == FIRE) {
		if (defender_type == BUG or defender_type == GRASS or defender_type == ICE or defender_type == STEEL)
			effectiveness = 4;
		else if (defender_type == DRAGON or defender_type == FIRE or defender_type == ROCK or defender_type == WATER)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == FLYING) {
		if (defender_type == BUG or defender_type == FIGHTING or defender_type == GRASS)
			effectiveness = 4;
		else if (defender_type == ELECTRIC or defender_type == ROCK or defender_type == STEEL)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == GHOST) {
		if (defender_type == GHOST or defender_type == PSYCHIC_TYPE)
			effectiveness = 4;
		else if (defender_type == DARK or defender_type == STEEL)
			effectiveness = 1;
		else if (defender_type == NORMAL)
			effectiveness = 0;
		else
			effectiveness = 2;
	}
	
	else if (move_type == GRASS) {
		if (defender_type == GROUND or defender_type == ROCK or defender_type == WATER)
			effectiveness = 4;
		else if (defender_type == BUG or defender_type == DRAGON or defender_type == FIRE or defender_type == FLYING or defender_type == GRASS or defender_type == POISON or defender_type == STEEL)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == GROUND) {
		if (defender_type == ELECTRIC or defender_type == FIRE or defender_type == POISON or defender_type == ROCK or defender_type == STEEL)
			effectiveness = 4;
		else if (defender_type == BUG or defender_type == GRASS)
			effectiveness = 1;
		else if (defender_type == FLYING)
			effectiveness = 0;
		else
			effectiveness = 2;
	}
	
	else if (move_type == ICE) {
		if (defender_type == DRAGON or defender_type == FLYING or defender_type == GRASS or defender_type == GROUND)
			effectiveness = 4;
		else if (defender_type == FIRE or defender_type == ICE or defender_type == STEEL or defender_type == WATER)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == NORMAL) {
		if (defender_type == ROCK or defender_type == STEEL)
			effectiveness = 1;
		else if (defender_type == GHOST)
			effectiveness = 0;
		else
			effectiveness = 2;
	}
	
	else if (move_type == POISON) {
		if (defender_type == GRASS)
			effectiveness = 4;
		else if (defender_type == GHOST or defender_type == GROUND or defender_type == POISON or defender_type == ROCK)
			effectiveness = 1;
		else if (defender_type == STEEL)
			effectiveness = 0;
		else
			effectiveness = 2;
	}
	
	else if (move_type == PSYCHIC_TYPE) {
		if (defender_type == FIGHTING or defender_type == POISON)
			effectiveness = 4;
		else if (defender_type == PSYCHIC_TYPE or defender_type == STEEL)
			effectiveness = 1;
		else if (defender_type == DARK)
			effectiveness = 0;
		else
			effectiveness = 2;
	}
	
	else if (move_type == ROCK) {
		if (defender_type == BUG or defender_type == FIRE or defender_type == FLYING or defender_type == ICE)
			effectiveness = 4;
		else if (defender_type == FIGHTING or defender_type == GRASS or defender_type == STEEL)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == STEEL) {
		if (defender_type == ICE or defender_type == ROCK)
			effectiveness = 4;
		else if (defender_type == ELECTRIC or defender_type == FIRE or defender_type == STEEL or defender_type == WATER)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else if (move_type == WATER) {
		if (defender_type == FIRE or defender_type == GROUND or defender_type == ROCK)
			effectiveness = 4;
		else if (defender_type == DRAGON or defender_type == GRASS or defender_type == WATER)
			effectiveness = 1;
		else
			effectiveness = 2;
	}
	
	else				// typeless moves
		effectiveness = 2;
	
	return effectiveness;
}

/*
std::map <std::string, types> types_map;
types_map["Bug"] = BUG;
types_map["Dark"] = DARK;
types_map["Dragon"] = DRAGON;
types_map["Electric"] = ELECTRIC;
types_map["Fighting"] = FIGHTING;
types_map["Fire"] = FIRE;
types_map["Flying"] = FLYING;
types_map["Ghost"] = GHOST;
types_map["Grass"] = GRASS;
types_map["Ground"] = GROUND;
types_map["Ice"] = ICE;
types_map["Normal"] = NORMAL;
types_map["Poison"] = POISON;
types_map["Psychic"] = PSYCHIC_TYPE;
types_map["Rock"] = ROCK;
types_map["Steel"] = STEEL;
types_map["Water"] = WATER;
types_map["Typeless"] = TYPELESS;
*/

#endif
