// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.find_required_move_index;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.find_required_index;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto find_required_move_index(RegularMoves const moves, MoveName const move_name) -> containers::index_type<RegularMoves> {
	if (move_name == MoveName::Struggle) {
		return 0_bi;
	}
	return find_required_index(containers::transform(moves, &Move::name), move_name);
}

} // namespace technicalmachine