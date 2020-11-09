// Copyright (C) 2020 David Stone
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

#pragma once

#include <bounded/integer.hpp>

namespace technicalmachine {

using namespace bounded::literal;

template<bounded::bounded_integer Integer>
constexpr auto saturating_add(Integer & x, bounded::bounded_integer auto other) {
	x = bounded::clamp(x + other, bounded::min_value<Integer>, bounded::max_value<Integer>);
}

constexpr auto saturating_increment(bounded::bounded_integer auto & x) {
	saturating_add(x, 1_bi);
}

constexpr auto saturating_decrement(bounded::bounded_integer auto & x) {
	saturating_add(x, -1_bi);
}

} // namespace technicalmachine
