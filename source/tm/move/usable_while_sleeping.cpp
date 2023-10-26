// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.usable_while_sleeping;

import tm.move.move_name;

namespace technicalmachine {

export constexpr bool usable_while_sleeping(MoveName const move) {
	switch (move) {
		case MoveName::Sleep_Talk:
		case MoveName::Snore:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
