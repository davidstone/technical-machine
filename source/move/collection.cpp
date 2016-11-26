// Collection of moves with index indicating current move
// Copyright (C) 2016 David Stone
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

#include <cassert>

namespace technicalmachine {

MoveCollection::MoveCollection(TeamSize const my_team_size):
	detail::Collection<MoveContainer>(my_team_size) {
}

auto regular_move(MoveCollection const & moves) -> Move {
	return *(begin(moves.regular()) + RegularMoveIndex(moves.index()));
}
auto regular_move(MoveCollection & moves) -> Move & {
	return *(begin(moves.regular()) + RegularMoveIndex(moves.index()));
}

auto MoveCollection::add(Moves move, Pp::pp_ups_type pp_ups) -> void {
	auto it = containers::find(begin(regular()), end(regular()), move);
	if (it == end(regular())) {
		emplace_back(move, pp_ups);
		it = containers::prev(end(regular()));
	}
	set_index(static_cast<containers::index_type<MoveCollection>>(it - begin(regular())));
}

auto set_index(MoveCollection & moves, Moves const move) -> void {
	auto const it = containers::find(begin(moves), end(moves), move);
	assert(it != end(moves));
	moves.set_index(static_cast<containers::index_type<MoveCollection>>(it - begin(moves)));
}

using IndexResult = bounded::optional<RegularMoveIndex>;
auto index(MoveCollection const & moves, Moves const name) -> IndexResult {
	auto const it = containers::find(begin(moves.regular()), end(moves.regular()), name);
	return (it != end(moves.regular())) ? IndexResult(it - begin(moves.regular())) : bounded::none;
}

}	// namespace technicalmachine
