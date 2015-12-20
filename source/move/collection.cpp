// Collection of moves with index indicating current move
// Copyright (C) 2015 David Stone
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

#include <bounded/integer_range.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/algorithms/iterator.hpp>

#include <cassert>
#include <vector>

namespace technicalmachine {

MoveCollection::MoveCollection(TeamSize const my_team_size):
	detail::Collection<MoveContainer>(my_team_size) {
}

auto regular_move(MoveCollection const & moves) -> Move {
	return *(moves.regular().begin() + RegularMoveIndex(moves.index()));
}
auto regular_move(MoveCollection & moves) -> Move & {
	return *(moves.regular().begin() + RegularMoveIndex(moves.index()));
}

auto MoveCollection::add(Moves move, Pp::pp_ups_type pp_ups) -> void {
	auto it = containers::find(regular().begin(), regular().end(), move);
	if (it == regular().end()) {
		emplace_back(move, pp_ups);
		it = containers::prev(regular().end());
	}
	set_index(static_cast<containers::index_type<MoveCollection>>(it - regular().begin()));
}

auto set_index(MoveCollection & moves, Moves const move) -> void {
	auto const it = containers::find(moves.begin(), moves.end(), move);
	assert(it != moves.end());
	moves.set_index(static_cast<containers::index_type<MoveCollection>>(it - moves.begin()));
}

using IndexResult = bounded::optional<RegularMoveIndex>;
auto index(MoveCollection const & moves, Moves const name) -> IndexResult {
	auto const it = containers::find(moves.regular().begin(), moves.regular().end(), name);
	return (it != moves.regular().end()) ? IndexResult(it - moves.regular().begin()) : bounded::none;
}

}	// namespace technicalmachine
