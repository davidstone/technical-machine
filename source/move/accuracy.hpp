// Accuracy functions
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

#ifndef MOVE__ACCURACY_HPP_
#define MOVE__ACCURACY_HPP_

#include <bounded_integer/optional.hpp>
#include <bounded_integer/bounded_integer.hpp>
#include "moves.hpp"

namespace technicalmachine {

// A value that is not present indicates that the move cannot miss.
using BaseAccuracy = bounded::optional<bounded::integer<30, 100>>;
auto accuracy(Moves move) -> BaseAccuracy;

}	// namespace technicalmachine
#endif	// MOVE__ACCURACY_HPP_
