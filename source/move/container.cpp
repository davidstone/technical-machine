// Collection of moves with index indicating current move
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

#include "container.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <vector>

#include "move.hpp"
#include "shared.hpp"

namespace technicalmachine {

MoveContainer::MoveContainer(unsigned const my_team_size):
	shared(my_team_size) {
}

Move const & MoveContainer::operator[](size_t const index) const {
	assert(index < size());
	return (index < regular.size()) ? regular[index] : shared[index - regular.size()];
}

Move const & MoveContainer::regular_move(size_t const index) const {
	assert(index < regular.size());
	return regular[index];
}
Move & MoveContainer::regular_move(size_t const index) {
	assert(index < regular.size());
	return regular[index];
}

size_t MoveContainer::size() const {
	return regular.size() + shared.size();
}

size_t MoveContainer::number_of_regular_moves() const {
	return regular.size();
}

void MoveContainer::remove_switch() {
	shared.remove_switch();
}

bool operator==(MoveContainer const & lhs, MoveContainer const & rhs) {
	// I may not need to check if lhs.shared == rhs.shared, because whenever I
	// compare two moves, it's in the context of comparing an entire team, and I
	// believe other parts of the team will always give me all the information
	// contained in lhs.shared == rhs.shared.
	return lhs.regular == rhs.regular and
			lhs.shared == rhs.shared;
}

}	// namespace technicalmachine
