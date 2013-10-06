// Effectiveness of a type
// Copyright (C) 2013 David Stone
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

#include "effectiveness.hpp"
#include <algorithm>
#include "collection.hpp"
#include "type.hpp"
#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {
Rational lookup_effectiveness(Type::Types const attacking, Type::Types const defending);
bool check_effectiveness(std::vector<Rational> const & effectiveness, std::initializer_list<unsigned> const & results);
}	// namespace

Effectiveness::Effectiveness(Type::Types const type, Pokemon const & defender) {
	for (Type const target_type : defender.type().types)
		effectiveness.emplace_back(lookup_effectiveness(type, target_type.type));
}

Effectiveness::Effectiveness(Type::Types const attacking, Type::Types const defending):
	effectiveness({ lookup_effectiveness(attacking, defending) }) {
}

Effectiveness Effectiveness::stealth_rock_effectiveness(Pokemon const & pokemon) {
	return Effectiveness(Type::Rock, pokemon);
}

bool Effectiveness::is_super_effective() const {
	return check_effectiveness(effectiveness, { 8, 16 });
}

bool Effectiveness::is_not_very_effective() const {
	return check_effectiveness(effectiveness, { 1, 2 });
}

bool Effectiveness::has_no_effect() const {
	return check_effectiveness(effectiveness, { 0 });
}


namespace {

bool check_effectiveness(std::vector<Rational> const & effectiveness, std::initializer_list<unsigned> const & results) {
	unsigned value = 4;
	for (Rational const & e : effectiveness) {
		value *= e;
	}
	return std::find(std::begin(results), std::end(results), value) != std::end(results);
}

Rational lookup_effectiveness(Type::Types const attacking, Type::Types const defending) {
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
	return Rational(effectiveness[attacking][defending], 2);
}

}	// unnamed namespace
}	// namespace technicalmachine
