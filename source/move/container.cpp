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
#include <vector>

namespace technicalmachine {

MoveContainer::MoveContainer(TeamSize const my_team_size):
	shared(my_team_size) {
}

auto MoveContainer::unchecked_regular_move(RegularMoveIndex const index) const -> Move const & {
	return regular[index.value()];
}
auto MoveContainer::unchecked_regular_move(RegularMoveIndex const index) -> Move & {
	return regular[index.value()];
}

auto MoveContainer::operator[](index_type const index) const -> Move const & {
	assert(index < size());
	return (index < number_of_regular_moves()) ?
		unchecked_regular_move(RegularMoveIndex(index, bounded::non_check)) :
		shared[static_cast<SharedMoves::index_type>(index - number_of_regular_moves())];
}

auto MoveContainer::regular_move(RegularMoveIndex const index) const -> Move const & {
	assert(index < number_of_regular_moves());
	return unchecked_regular_move(index);
}
auto MoveContainer::regular_move(RegularMoveIndex const index) -> Move & {
	assert(index < number_of_regular_moves());
	return unchecked_regular_move(index);
}

auto MoveContainer::size() const -> size_type {
	return number_of_regular_moves() + shared.size();
}

auto MoveContainer::number_of_regular_moves() const -> RegularMoveSize {
	return static_cast<RegularMoveSize>(regular.size());
}

auto MoveContainer::remove_switch() -> void {
	shared.remove_switch();
}

auto MoveContainer::hash() const -> hash_type {
	static constexpr auto initial = static_cast<hash_type>(0);
	return std::accumulate(regular.begin(), regular.end(), initial, [](hash_type const & value, Move const & move) {
		return value * move.max_hash() + move.hash();
	});
}

auto MoveContainer::max_hash() const -> hash_type {
	static constexpr auto initial = static_cast<hash_type>(0);
	return std::accumulate(regular.begin(), regular.end(), initial, [](hash_type const & value, Move const & move) {
		return value + move.max_hash();
	});
}


auto operator==(MoveContainer const & lhs, MoveContainer const & rhs) -> bool {
	// I may not need to check if lhs.shared == rhs.shared, because whenever I
	// compare two moves, it's in the context of comparing an entire team, and I
	// believe other parts of the team will always give me all the information
	// contained in lhs.shared == rhs.shared.
	return lhs.regular == rhs.regular and
			lhs.shared == rhs.shared;
}

}	// namespace technicalmachine
