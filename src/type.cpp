// Type function definitions
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

#include "type.hpp"

#include <vector>

#include "ability.hpp"
#include "status.hpp"
#include "weather.hpp"

#include "pokemon/pokemon.hpp"

namespace technicalmachine {

Type::Type (Types name):
	type (name) {
}

bool operator== (Type const lhs, Type const rhs) {
	return lhs.type == rhs.type;
}

bool operator!= (Type const lhs, Type const rhs) {
	return !(lhs == rhs);
}

bool Type::is_boosted_by_flash_fire () const {
	return type == FIRE;
}

bool Type::is_immune_to_hail () const {
	return type == ICE;
}

bool Type::is_immune_to_sandstorm () const {
	switch (type) {
		case GROUND:
		case ROCK:
		case STEEL:
			return true;
		default:
			return false;
	}
}

bool Type::is_strengthened_by_weather (Weather const & weather) const {
	return (weather.rain() and type == WATER) or (weather.sun() and type == FIRE);
}

bool Type::is_weakened_by_weather (Weather const & weather) const {
	return (weather.rain() and type == FIRE) or (weather.sun() and type == WATER);
}

template<>
bool Type::blocks_status<Status::BURN> () const {
	return type == FIRE;
}

template<>
bool Type::blocks_status<Status::FREEZE> () const {
	return type == ICE;
}

template<>
bool Type::blocks_status<Status::POISON> () const {
	switch (type) {
		case POISON:
		case STEEL:
			return true;
		default:
			return false;
	}
}

template<>
bool Type::blocks_status<Status::POISON_TOXIC> () const {
	return blocks_status<Status::POISON> ();
}

unsigned Type::lookup_effectiveness (Types const attacking, Type const defending) {
	constexpr static unsigned effectiveness [18][18] = {
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
	return effectiveness [attacking] [defending.type];
}

unsigned Type::get_effectiveness (Type::Types const type, Pokemon const & defender) {
	unsigned effectiveness = 1;
	// Effectiveness on each of the defender's types (1 if NVE, 4 if SE) / 2
	for (Type const target_type : defender.type.types)
		effectiveness *= lookup_effectiveness (type, target_type);
	if (defender.type.types.size () == 1)
		effectiveness *= 2;
	return effectiveness;
}

unsigned Type::lookup_effectiveness (Type const defending) const {
	return lookup_effectiveness (type, defending);
}

unsigned Type::get_effectiveness (Pokemon const & defender) const {
	return get_effectiveness (type, defender);
}

std::vector <unsigned> Type::get_effectiveness_variables (Pokemon const & target) const {
	std::vector <unsigned> effectiveness;
	// Effectiveness on each of the defender's type (1 if NVE, 4 if SE) / 2
	for (Type const target_type : target.type.types)
		effectiveness.push_back (lookup_effectiveness (target_type));
	return effectiveness;
}

unsigned Type::stealth_rock_effectiveness (Pokemon const & pokemon) {
	return get_effectiveness (ROCK, pokemon);
}

}	// namespace technicalmachine
