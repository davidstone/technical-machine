// Collection of moves with index indicating current move
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

#include "container.hpp"

#include "shared.hpp"

#include <algorithm>
#include <cassert>
#include <utility>
#include <vector>

namespace technicalmachine {

auto operator+(MoveIterator const lhs, MoveIterator::difference_type const rhs) -> MoveIterator {
	using RegularDistance = std::vector<Move>::const_iterator::difference_type;
	static constexpr auto regular_max = static_cast<intmax_t>(std::numeric_limits<RegularMoveSize>::max());
	auto const distance = static_cast<bounded::integer<-regular_max, regular_max>>(std::distance(lhs.m_regular, lhs.m_regular_end));
	return (distance > rhs) ?
		MoveIterator(lhs.m_regular + static_cast<RegularDistance>(rhs), lhs.m_regular_end, lhs.m_shared) :
		MoveIterator(lhs.m_regular_end, lhs.m_regular_end, lhs.m_shared + static_cast<SharedMovesIterator::difference_type>(rhs - distance));
}
auto operator-(MoveIterator const lhs, MoveIterator const rhs) -> MoveIterator::difference_type {
	// The +-1 accounts for the one-past-the-end index.
	using RegularMoveDifference = decltype(std::declval<RegularMoveIndex>() - std::declval<RegularMoveIndex>() + std::declval<bounded::integer<-1, 1>>());
	return (lhs.m_shared - rhs.m_shared) + static_cast<RegularMoveDifference>(lhs.m_regular - rhs.m_regular);
}

auto operator==(MoveIterator const lhs, MoveIterator const rhs) noexcept -> bool {
	return lhs.m_regular == rhs.m_regular and lhs.m_shared == rhs.m_shared;
}
auto operator<(MoveIterator const lhs, MoveIterator const rhs) noexcept -> bool {
	return lhs.m_regular < rhs.m_regular or lhs.m_shared < rhs.m_shared;
}

MoveContainer::MoveContainer(TeamSize const my_team_size):
	m_shared(my_team_size) {
}

auto MoveContainer::unchecked_regular_move(RegularMoveIndex const index) const -> Move const & {
	return m_regular[index.value()];
}
auto MoveContainer::unchecked_regular_move(RegularMoveIndex const index) -> Move & {
	return m_regular[index.value()];
}

auto MoveContainer::operator[](index_type const index) const -> Move const & {
	assert(index < size());
	return (index < number_of_regular_moves()) ?
		unchecked_regular_move(RegularMoveIndex(index, bounded::non_check)) :
		m_shared[static_cast<SharedMoves::index_type>(index - number_of_regular_moves())];
}

auto MoveContainer::size() const -> size_type {
	return number_of_regular_moves() + technicalmachine::size(m_shared);
}

auto MoveContainer::number_of_regular_moves() const -> RegularMoveSize {
	return static_cast<RegularMoveSize>(m_regular.size());
}

auto MoveContainer::remove_switch() -> void {
	m_shared.remove_switch();
}


auto operator==(MoveContainer const & lhs, MoveContainer const & rhs) -> bool {
	// I may not need to check if lhs.shared == rhs.shared, because whenever I
	// compare two moves, it's in the context of comparing an entire team, and I
	// believe other parts of the team will always give me all the information
	// contained in lhs.shared == rhs.shared.
	return lhs.m_regular == rhs.m_regular and
			lhs.m_shared == rhs.m_shared;
}

}	// namespace technicalmachine
