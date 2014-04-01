// Pokemon Online stat order
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

#ifndef POKEMON_ONLINE__STAT_ORDER_HPP_
#define POKEMON_ONLINE__STAT_ORDER_HPP_

#include "../../stat/stat.hpp"

#include <bounded_integer/array.hpp>

namespace technicalmachine {
namespace po {

constexpr bounded::array<StatNames, 5> stat_order = {
	StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE
};

}	// namespace po
}	// namespace technicalmachine
#endif	// POKEMON_ONLINE__STAT_ORDER_HPP_
