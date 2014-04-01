// Base power calculation
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

#ifndef MOVE__BASE_POWER_HPP_
#define MOVE__BASE_POWER_HPP_

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/optional.hpp>

#include "moves_forward.hpp"

namespace technicalmachine {

// variable power returns non-0. Fixed damage is the uninitialized state.
bounded::optional<bounded::integer<0, 250>> base_power(Moves move);

}	// namespace technicalmachine
#endif	// MOVE__BASE_POWER_HPP_
