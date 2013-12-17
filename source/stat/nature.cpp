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

#include "stat_names.hpp"

#include "../rational.hpp"

namespace technicalmachine {
namespace {
Nature::Natures lookup_nature(StatNames const boost, StatNames const drop) {
	switch (boost) {
		case StatNames::ATK:
			switch (drop) {
				case StatNames::ATK:
					return Nature::BASHFUL;
				case StatNames::DEF:
					return Nature::LONELY;
				case StatNames::SPA:
					return Nature::ADAMANT;
				case StatNames::SPD:
					return Nature::NAUGHTY;
				default:	// case StatNames::SPE:
					return Nature::BRAVE;
			}
		case StatNames::DEF:
			switch (drop) {
				case StatNames::ATK:
					return Nature::BOLD;
				case StatNames::DEF:
					return Nature::DOCILE;
				case StatNames::SPA:
					return Nature::IMPISH;
				case StatNames::SPD:
					return Nature::LAX;
				default:	// case StatNames::SPE:
					return Nature::RELAXED;
			}
		case StatNames::SPA:
			switch (drop) {
				case StatNames::ATK:
					return Nature::MODEST;
				case StatNames::DEF:
					return Nature::MILD;
				case StatNames::SPA:
					return Nature::HARDY;
				case StatNames::SPD:
					return Nature::RASH;
				default:	// case StatNames::SPE:
					return Nature::QUIET;
			}
		case StatNames::SPD:
			switch (drop) {
				case StatNames::ATK:
					return Nature::CALM;
				case StatNames::DEF:
					return Nature::GENTLE;
				case StatNames::SPA:
					return Nature::CAREFUL;
				case StatNames::SPD:
					return Nature::QUIRKY;
				default:	// case StatNames::SPE:
					return Nature::SASSY;
			}
		default:	// case StatNames::SPE:
			switch (drop) {
				case StatNames::ATK:
					return Nature::TIMID;
				case StatNames::DEF:
					return Nature::HASTY;
				case StatNames::SPA:
					return Nature::JOLLY;
				case StatNames::SPD:
					return Nature::NAIVE;
				default:	// case StatNames::SPE:
					return Nature::SERIOUS;
			}
	}
}
}	// unnamed namespace

Nature::Nature():
	name(END) {
}

Nature::Nature(Natures nature):
	name(nature) {
}

Nature::Nature(StatNames const boosted, StatNames const dropped):
	name(lookup_nature(boosted, dropped)) {
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
bool Nature::boosts_stat<StatNames::ATK>() const {
	switch (name) {
		case ADAMANT:
		case BRAVE:
		case LONELY:
		case NAUGHTY:
			return true;
		default:
			return false;
	}
}
template<>
bool Nature::boosts_stat<StatNames::SPA>() const {
	switch (name) {
		case MILD:
		case MODEST:
		case QUIET:
		case RASH:
			return true;
		default:
			return false;
	}
}

template<>
bool Nature::boosts_stat<StatNames::DEF>() const {
	switch (name) {
		case BOLD:
		case IMPISH:
		case LAX:
		case RELAXED:
			return true;
		default:
			return false;
	}
}
template<>
bool Nature::boosts_stat<StatNames::SPD>() const {
	switch (name) {
		case CALM:
		case CAREFUL:
		case GENTLE:
		case SASSY:
			return true;
		default:
			return false;
	}
}
bool boosts_defending_stat(Nature const nature) {
	return nature.boosts_stat<StatNames::DEF>() or nature.boosts_stat<StatNames::SPD>();
}

template<>
bool Nature::boosts_stat<StatNames::SPE>() const {
	switch (name) {
		case HASTY:
		case JOLLY:
		case NAIVE:
		case TIMID:
			return true;
		default:
			return false;
	}
}

template<>
bool Nature::lowers_stat<StatNames::ATK>() const {
	switch (name) {
		case BOLD:
		case CALM:
		case MODEST:
		case TIMID:
			return true;
		default:
			return false;
	}
}
template<>
bool Nature::lowers_stat<StatNames::SPA>() const {
	switch (name) {
		case ADAMANT:
		case CAREFUL:
		case IMPISH:
		case JOLLY:
			return true;
		default:
			return false;
	}
}
bool lowers_attacking_stat(Nature const nature) {
	return nature.lowers_stat<StatNames::ATK>() or nature.lowers_stat<StatNames::SPA>();
}

template<>
bool Nature::lowers_stat<StatNames::DEF>() const {
	switch (name) {
		case GENTLE:
		case HASTY:
		case LONELY:
		case MILD:
			return true;
		default:
			return false;
	}
}
template<>
bool Nature::lowers_stat<StatNames::SPD>() const {
	switch (name) {
		case LAX:
		case NAIVE:
		case NAUGHTY:
		case RASH:
			return true;
		default:
			return false;
	}
}
bool lowers_defending_stat(Nature const nature) {
	return nature.lowers_stat<StatNames::DEF>() or nature.lowers_stat<StatNames::SPD>();
}

template<>
bool Nature::lowers_stat<StatNames::SPE>() const {
	switch (name) {
		case BRAVE:
		case QUIET:
		case RELAXED:
		case SASSY:
			return true;
		default:
			return false;
	}
}


template<StatNames stat>
Rational Nature::boost() const {
	if (boosts_stat<stat>()) {
		return Rational(11, 10);
	}
	else if (lowers_stat<stat>()) {
		return Rational(9, 10);
	}
	else {
		return Rational(1);
	}
}

template Rational Nature::boost<StatNames::ATK>() const;
template Rational Nature::boost<StatNames::DEF>() const;
template Rational Nature::boost<StatNames::SPA>() const;
template Rational Nature::boost<StatNames::SPD>() const;
template Rational Nature::boost<StatNames::SPE>() const;

bool operator== (Nature const lhs, Nature const rhs) {
	return lhs.name == rhs.name;
}

bool operator!= (Nature const lhs, Nature const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
