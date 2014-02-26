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

#include "collection.hpp"

#include <cassert>
#include <vector>

#include "move.hpp"
#include "moves.hpp"

namespace technicalmachine {

MoveCollection::MoveCollection(TeamSize const my_team_size):
	Base(my_team_size) {
}

Move const & MoveCollection::regular_move() const {
	return regular_move(static_cast<regular_index_type>(index()));
}
Move & MoveCollection::regular_move() {
	return regular_move(static_cast<regular_index_type>(index()));
}
Move const & MoveCollection::regular_move(regular_index_type const get_index) const {
	return container.regular_move(static_cast<std::size_t>(check_range(get_index, number_of_regular_moves())));
}
Move & MoveCollection::regular_move(regular_index_type const get_index) {
	return container.regular_move(static_cast<std::size_t>(check_range(get_index, number_of_regular_moves())));
}

auto MoveCollection::number_of_regular_moves () const -> regular_size_type {
	return static_cast<regular_size_type>(container.number_of_regular_moves());
}

bool MoveCollection::set_index_if_found(Moves name) {
	for (index_type const new_index : bounded_integer::range(size())) {
		if (unchecked_value(new_index) == name) {
			Base::set_index(new_index);
			return true;
		}
	}
	return false;
}

void MoveCollection::set_index(Moves const name) {
	bool const found = set_index_if_found(name);
	assert(found);
}

Move const * MoveCollection::find (Moves name) const {
	return container.find_if([name](Move const & move) { return move == name; });
}

Move * MoveCollection::find (Moves name) {
	return container.find_if([name](Move const & move) { return move == name; });
}

auto MoveCollection::index(Moves const name) const -> bounded_integer::optional<regular_index_type> {
	for (regular_index_type const n : bounded_integer::range(static_cast<regular_size_type>(container.number_of_regular_moves()))) {
		if (container.regular_move(static_cast<std::size_t>(n)) == name) {
			return n;
		}
	}
	return bounded_integer::none;
}

auto MoveCollection::size() const -> MoveCollection::size_type {
	return static_cast<size_type>(container.size());
}

void MoveCollection::remove_switch() {
	container.remove_switch();
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
