// Stat data structure (Attack, Defense, Special Attack, Special Defense, Speed)
// Copyright (C) 2014 David Stone
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

#ifndef STAT__STAT_HPP_
#define STAT__STAT_HPP_

#include <bounded_integer/bounded_integer.hpp>
#include "ev.hpp"
#include "iv.hpp"
#include "stat_names.hpp"
#include "../pokemon/species_forward.hpp"

namespace technicalmachine {
using namespace bounded_integer::literal;

class Stat {
public:
	using base_type = bounded_integer::checked_integer<5, 230>;

	Stat(Species name, StatNames stat, EV ev = EV(0_bi));

	base_type base;
	EV ev;
	IV iv;
};

}	// namespace technicalmachine
#endif	// STAT__STAT_HPP_
