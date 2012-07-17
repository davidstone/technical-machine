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

#include "collection.hpp"

#include <cstdint>
#include <functional>
#include <vector>

#include "move.hpp"
#include "moves.hpp"
#include "ranked.hpp"
#include "reorder.hpp"

namespace technicalmachine {
class SharedMoves;

MoveCollection::MoveCollection (SharedMoves & s):
	Base(MoveContainer (s)) {
}

MoveCollection::index_type MoveCollection::number_of_regular_moves () const {
	return container.number_of_regular_moves();
}

void MoveCollection::for_each (std::function<void(Move const &)> const & f) const {
	container.for_each_regular_move(f);
	container.for_each_shared(f);
}
void MoveCollection::for_each (std::function<void(Move &)> const & f) {
	container.for_each_regular_move(f);
	container.for_each_shared(f);
}
// Skips Struggle and switches
void MoveCollection::for_each_regular_move (std::function<void(Move const &)> const & f) const {
	container.for_each_regular_move(f);
}
void MoveCollection::for_each_regular_move (std::function<void(Move &)> const & f) {
	container.for_each_regular_move(f);
}

bool MoveCollection::set_index_if_found(Moves name) {
	for (index_type new_index = 0; new_index != number_of_regular_moves(); ++new_index) {
		if (unchecked_value(new_index).name == name) {
			set_index(new_index);
			return true;
		}
	}
	return false;
}

Move const * MoveCollection::find (Moves name) const {
	return container.find_if([name](Move const & move) { return move.name == name; });
}

Move * MoveCollection::find (Moves name) {
	return container.find_if([name](Move const & move) { return move.name == name; });
}

bool MoveCollection::regular_move_exists (std::function<bool(Move const &)> const & condition) const {
	return container.find_if(condition) != nullptr;
}

bool MoveCollection::exists (Moves name) const {
	return find(name) != nullptr;
}

bool MoveCollection::a_regular_move_is_selectable () const {
	return regular_move_exists ([](Move const & move) { return move.selectable(); });
}

Moves MoveCollection::name_of_last_used_move () const {
	Move const * move_ptr = container.find_if([] (Move const & move) {
		return move.was_used_last();
	});
	return (move_ptr != nullptr) ? move_ptr->name : Moves::END;
}

std::vector<RankedMove> MoveCollection::create_ordered_container (bool const ai) const {
	return reorder(container.concatenate(), ai);
}

MoveCollection::index_type MoveCollection::size () const {
	return container.size();
}

void MoveCollection::update_shared_moves(SharedMoves & s) {
	container.update_shared_moves(s);
}

MoveCollection::hash_type MoveCollection::hash() const {
	hash_type current_hash = 0;
	for_each_regular_move([& current_hash](Move const & next_move) {
		current_hash *= next_move.max_hash();
		current_hash += next_move.hash();
	});
	return current_hash;
}

MoveCollection::hash_type MoveCollection::max_hash() const {
	hash_type current_max = 0;
	for_each_regular_move([& current_max](Move const & next_move) {
		current_max += next_move.max_hash();
	});
	return current_max;
}

}	// namespace technicalmachine
