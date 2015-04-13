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
#include "moves.hpp"

#include <bounded_integer/integer_range.hpp>

#include <cassert>
#include <vector>

namespace technicalmachine {

MoveCollection::MoveCollection(TeamSize const my_team_size):
	Base(my_team_size) {
}

auto regular_move(MoveCollection const & moves) -> Move const & {
	return *(moves.regular().begin() + static_cast<int>(moves.index()));
}
auto regular_move(MoveCollection & moves) -> Move & {
	return *(moves.regular().begin() + static_cast<int>(moves.index()));
}

auto MoveCollection::add(Moves move, Pp::pp_ups_type pp_ups) -> void {
	auto it = std::find(regular().begin(), regular().end(), move);
	if (it == regular().end()) {
		container.emplace_back(move, pp_ups);
		it = std::prev(regular().end());
	}
	set_index(static_cast<index_type>(it - regular().begin()));
}

auto set_index(MoveCollection & moves, Moves const move) -> void {
	auto it = moves.begin();
	for (; it != moves.end(); ++it) {
		if (static_cast<Moves>(*it) == move) {
			moves.set_index(static_cast<MoveCollection::index_type>(it - moves.begin()));
			break;
		}
	}
	assert(it != moves.end());
}

using IndexResult = bounded::optional<RegularMoveIndex>;
auto index(MoveCollection const & moves, Moves const name) -> IndexResult {
	auto const it = std::find(moves.regular().begin(), moves.regular().end(), name);
	return (it != moves.regular().end()) ? IndexResult(it - moves.regular().begin()) : bounded::none;
}

auto MoveCollection::remove_switch() -> void {
	container.remove_switch();
}

}	// namespace technicalmachine
