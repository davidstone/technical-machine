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
namespace {
Nature::Natures lookup_nature(Stat::Stats const boost, Stat::Stats const drop) {
	switch (boost) {
		case Stat::ATK:
			switch (drop) {
				case Stat::ATK:
					return Nature::BASHFUL;
				case Stat::DEF:
					return Nature::LONELY;
				case Stat::SPA:
					return Nature::ADAMANT;
				case Stat::SPD:
					return Nature::NAUGHTY;
				default:	// case Stat::SPE:
					return Nature::BRAVE;
			}
		case Stat::DEF:
			switch (drop) {
				case Stat::ATK:
					return Nature::BOLD;
				case Stat::DEF:
					return Nature::DOCILE;
				case Stat::SPA:
					return Nature::IMPISH;
				case Stat::SPD:
					return Nature::LAX;
				default:	// case Stat::SPE:
					return Nature::RELAXED;
			}
		case Stat::SPA:
			switch (drop) {
				case Stat::ATK:
					return Nature::MODEST;
				case Stat::DEF:
					return Nature::MILD;
				case Stat::SPA:
					return Nature::HARDY;
				case Stat::SPD:
					return Nature::RASH;
				default:	// case Stat::SPE:
					return Nature::QUIET;
			}
		case Stat::SPD:
			switch (drop) {
				case Stat::ATK:
					return Nature::CALM;
				case Stat::DEF:
					return Nature::GENTLE;
				case Stat::SPA:
					return Nature::CAREFUL;
				case Stat::SPD:
					return Nature::QUIRKY;
				default:	// case Stat::SPE:
					return Nature::SASSY;
			}
		default:	// case Stat::SPE:
			switch (drop) {
				case Stat::ATK:
					return Nature::TIMID;
				case Stat::DEF:
					return Nature::HASTY;
				case Stat::SPA:
					return Nature::JOLLY;
				case Stat::SPD:
					return Nature::NAIVE;
				default:	// case Stat::SPE:
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

Nature::Nature(Stat::Stats const boosted, Stat::Stats const dropped):
	name(lookup_nature(boosted, dropped)) {
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
bool Nature::boosts_stat<Stat::ATK>() const {
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
bool Nature::boosts_stat<Stat::SPA>() const {
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
bool boosts_attacking_stat(Nature const nature) {
	return nature.boosts_stat<Stat::ATK>() or nature.boosts_stat<Stat::SPA>();
}

template<>
bool Nature::boosts_stat<Stat::DEF>() const {
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
bool Nature::boosts_stat<Stat::SPD>() const {
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
	return nature.boosts_stat<Stat::DEF>() or nature.boosts_stat<Stat::SPD>();
}

template<>
bool Nature::boosts_stat<Stat::SPE>() const {
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
bool Nature::lowers_stat<Stat::ATK>() const {
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
bool Nature::lowers_stat<Stat::SPA>() const {
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
	return nature.lowers_stat<Stat::ATK>() or nature.lowers_stat<Stat::SPA>();
}

template<>
bool Nature::lowers_stat<Stat::DEF>() const {
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
bool Nature::lowers_stat<Stat::SPD>() const {
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
	return nature.lowers_stat<Stat::DEF>() or nature.lowers_stat<Stat::SPD>();
}

template<>
bool Nature::lowers_stat<Stat::SPE>() const {
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


template<Stat::Stats stat>
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

template Rational Nature::boost<Stat::ATK>() const;
template Rational Nature::boost<Stat::DEF>() const;
template Rational Nature::boost<Stat::SPA>() const;
template Rational Nature::boost<Stat::SPD>() const;
template Rational Nature::boost<Stat::SPE>() const;

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
