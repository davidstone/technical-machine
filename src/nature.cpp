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

namespace technicalmachine {

Nature::Nature ():
	name (END) {
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
unsigned Nature::boost<Stat::ATK> () const {
	switch (name) {
		case ADAMANT:
		case BRAVE:
		case LONELY:
		case NAUGHTY:
			return 11;
		case BOLD:
		case CALM:
		case MODEST:
		case TIMID:
			return 9;
		default:
			return 10;
	}
}

template<>
unsigned Nature::boost<Stat::DEF> () const {
	switch (name) {
		case BOLD:
		case IMPISH:
		case LAX:
		case RELAXED:
			return 11;
		case GENTLE:
		case HASTY:
		case LONELY:
		case MILD:
			return 9;
		default:
			return 10;
	}
}

template<>
unsigned Nature::boost<Stat::SPA> () const {
	switch (name) {
		case MILD:
		case MODEST:
		case QUIET:
		case RASH:
			return 11;
		case ADAMANT:
		case CAREFUL:
		case IMPISH:
		case JOLLY:
			return 9;
		default:
			return 10;
	}
}

template<>
unsigned Nature::boost<Stat::SPD> () const {
	switch (name) {
		case CALM:
		case CAREFUL:
		case GENTLE:
		case SASSY:
			return 11;
		case LAX:
		case NAIVE:
		case NAUGHTY:
		case RASH:
			return 9;
		default:
			return 10;
	}
}

template<>
unsigned Nature::boost<Stat::SPE> () const {
	switch (name) {
		case HASTY:
		case JOLLY:
		case NAIVE:
		case TIMID:
			return 11;
		case BRAVE:
		case QUIET:
		case RELAXED:
		case SASSY:
			return 9;
		default:
			return 10;
	}
}

}	// namespace technicalmachine
