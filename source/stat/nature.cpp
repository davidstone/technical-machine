// Nature functions
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

#include "nature.hpp"

#include <cassert>

#include "stat.hpp"

#include "../rational.hpp"

#include "../string_conversions/conversion.hpp"

namespace technicalmachine {

Nature::Nature():
	name(END) {
}

Nature::Nature(Natures nature):
	name(nature) {
}

Nature::Nature(std::string const & str):
	name(from_string<Natures>(str))
	{
}

bool Nature::is_set () const {
	return name != END;
}

void Nature::set_if_unknown (Natures const nature) {
	assert (nature < END);
	if (!is_set())
		name = nature;
}

template<>
Rational Nature::boost<Stat::ATK>() const {
	switch (name) {
		case ADAMANT:
		case BRAVE:
		case LONELY:
		case NAUGHTY:
			return Rational(11, 10);
		case BOLD:
		case CALM:
		case MODEST:
		case TIMID:
			return Rational(9, 10);
		default:
			return Rational(1);
	}
}

template<>
Rational Nature::boost<Stat::DEF>() const {
	switch (name) {
		case BOLD:
		case IMPISH:
		case LAX:
		case RELAXED:
			return Rational(11, 10);
		case GENTLE:
		case HASTY:
		case LONELY:
		case MILD:
			return Rational(9, 10);
		default:
			return Rational(1);
	}
}

template<>
Rational Nature::boost<Stat::SPA>() const {
	switch (name) {
		case MILD:
		case MODEST:
		case QUIET:
		case RASH:
			return Rational(11, 10);
		case ADAMANT:
		case CAREFUL:
		case IMPISH:
		case JOLLY:
			return Rational(9, 10);
		default:
			return Rational(1);
	}
}

template<>
Rational Nature::boost<Stat::SPD>() const {
	switch (name) {
		case CALM:
		case CAREFUL:
		case GENTLE:
		case SASSY:
			return Rational(11, 10);
		case LAX:
		case NAIVE:
		case NAUGHTY:
		case RASH:
			return Rational(9, 10);
		default:
			return Rational(1);
	}
}

template<>
Rational Nature::boost<Stat::SPE>() const {
	switch (name) {
		case HASTY:
		case JOLLY:
		case NAIVE:
		case TIMID:
			return Rational(11, 10);
		case BRAVE:
		case QUIET:
		case RELAXED:
		case SASSY:
			return Rational(9, 10);
		default:
			return Rational(1);
	}
}

bool operator== (Nature const lhs, Nature const rhs) {
	return lhs.name == rhs.name;
}

bool operator!= (Nature const lhs, Nature const rhs) {
	return !(lhs == rhs);
}

std::string Nature::to_string() const {
	return ::technicalmachine::to_string(name);
}

}	// namespace technicalmachine
