// Collection of moves with index indicating current move
// Copyright (C) 2018 David Stone
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

#include "container.hpp"

#include "shared.hpp"

#include <algorithm>
#include <cassert>
#include <utility>

namespace technicalmachine {

auto operator+(MoveIterator const lhs, MoveIterator::difference_type const rhs) -> MoveIterator {
	using RegularDistance = RegularMoveContainer::const_iterator::difference_type;
	using range_t = MoveIterator::range_t;
	return (size(lhs.m_regular) > rhs) ?
		MoveIterator(range_t(lhs.m_regular.begin() + static_cast<RegularDistance>(rhs), lhs.m_regular.end()), lhs.m_shared) :
		MoveIterator(range_t(lhs.m_regular.end(), lhs.m_regular.end()), lhs.m_shared + static_cast<SharedMovesIterator::difference_type>(rhs - size(lhs.m_regular)));
}
auto operator-(MoveIterator const lhs, MoveIterator const rhs) -> MoveIterator::difference_type {
	// The +-1 accounts for the one-past-the-end index.
	using RegularMoveDifference = decltype(std::declval<RegularMoveIndex>() - std::declval<RegularMoveIndex>() + std::declval<bounded::integer<-1, 1>>());
	return (lhs.m_shared - rhs.m_shared) + static_cast<RegularMoveDifference>(lhs.m_regular.begin() - rhs.m_regular.begin());
}

}	// namespace technicalmachine
