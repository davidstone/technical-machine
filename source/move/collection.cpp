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

#include "move.hpp"
#include "moves_forward.hpp"

#include <bounded_integer/integer_range.hpp>

#include <cassert>
#include <vector>

namespace technicalmachine {

MoveCollection::MoveCollection(TeamSize const my_team_size):
	Base(my_team_size) {
}

auto MoveCollection::regular_move() const -> Move const & {
	return regular_move(static_cast<RegularMoveIndex>(index()));
}
auto MoveCollection::regular_move() -> Move & {
	return regular_move(static_cast<RegularMoveIndex>(index()));
}
auto MoveCollection::regular_move(RegularMoveIndex const get_index) const -> Move const & {
	return container.regular_move(check_range(get_index, number_of_regular_moves()));
}
auto MoveCollection::regular_move(RegularMoveIndex const get_index) -> Move & {
	return container.regular_move(check_range(get_index, number_of_regular_moves()));
}

auto MoveCollection::number_of_regular_moves() const -> RegularMoveSize {
	return container.number_of_regular_moves();
}

auto MoveCollection::set_index_if_found(Moves name) -> bool {
	for (index_type const new_index : bounded::integer_range(size())) {
		if (unchecked_value(new_index) == name) {
			Base::set_index(new_index);
			return true;
		}
	}
	return false;
}

auto MoveCollection::set_index(Moves const name) -> void {
	bool const found = set_index_if_found(name);
	assert(found);
}

auto MoveCollection::index(Moves const name) const -> bounded::optional<RegularMoveIndex> {
	for (RegularMoveIndex const n : bounded::integer_range(container.number_of_regular_moves())) {
		if (container.regular_move(n) == name) {
			return n;
		}
	}
	return bounded::none;
}

auto MoveCollection::size() const -> MoveCollection::size_type {
	return container.size();
}

auto MoveCollection::remove_switch() -> void {
	container.remove_switch();
}

auto MoveCollection::hash() const -> hash_type {
	return container.hash();
}

auto MoveCollection::max_hash() const -> hash_type {
	return container.max_hash();
}

}	// namespace technicalmachine
