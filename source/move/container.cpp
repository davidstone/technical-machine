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
#include <functional>
#include <vector>

#include "move.hpp"
#include "shared.hpp"

namespace technicalmachine {

MoveContainer::MoveContainer(SharedMoves & s):
	shared(&s) {
}

Move const & MoveContainer::operator[](uint8_t const index) const {
	assert(index < size());
	return (index < regular.size()) ? regular[index] : (*shared)[index - regular.size()];
}
Move & MoveContainer::operator[](uint8_t const index) {
	assert(index < size());
	return (index < regular.size()) ? regular[index] : (*shared)[index - regular.size()];
}

bool MoveContainer::empty() const {
	// A move container is never empty, it always contains at least Struggle
	return false;
}

// Skips Struggle and switches
void MoveContainer::for_each_regular_move (std::function<void(Move const &)> const & f) const {
	std::for_each(regular.cbegin(), regular.cend(), f);
}
void MoveContainer::for_each_regular_move (std::function<void(Move &)> const & f) {
	std::for_each(regular.begin(), regular.end(), f);
}

void MoveContainer::for_each_shared (std::function<void(Move const &)> const & f) const {
	shared->for_each(f);
}
void MoveContainer::for_each_shared (std::function<void(Move &)> const & f) {
	shared->for_each(f);
}

std::vector<Move> MoveContainer::concatenate() const {
	std::vector<Move> v = regular;
	shared->for_each([&](Move const & move) {
		v.emplace_back(move);
	});
	return v;
}

Move const * MoveContainer::find_if (std::function<bool(Move const &)> const & condition) const {
	for (Move const & move : regular) {
		if (condition (move))
			return & move;
	}
	return nullptr;
}
Move * MoveContainer::find_if (std::function<bool(Move const &)> const & condition) {
	for (Move & move : regular) {
		if (condition (move))
			return & move;
	}
	return nullptr;
}

uint8_t MoveContainer::size() const {
	return regular.size() + shared->size();
}

uint8_t MoveContainer::number_of_regular_moves() const {
	return regular.size();
}

void MoveContainer::update_shared_moves(SharedMoves & s) {
	shared = &s;
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
