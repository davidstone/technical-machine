// EVs
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

#include "ev.hpp"
#include <algorithm>
#include "invalid_ev.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned max_per_stat = 252;
}	// unnamed namespace

EV::EV(unsigned const evs) :
	// The points give a single point increase at level 100. EVs have a minimum
	// step of 4 EVs = 1 stat point
	internal(evs / 4)
	{
	if (evs > max_per_stat)
		throw InvalidEV(evs);
}

unsigned EV::value() const {
	return internal * 4u;
}

void EV::set_value(unsigned const evs) {
	if (evs > max_per_stat)
		throw InvalidEV(evs);
	internal = evs / 4u;
}

bool EV::is_maxed() const {
	return value() == max_per_stat;
}

void EV::add(unsigned const evs) {
	internal = std::min(evs / 4 + internal, max_per_stat / 4);
}

unsigned EV::points() const {
	return internal;
}

}	// namespace technicalmachine
