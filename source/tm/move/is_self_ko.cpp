// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.is_self_ko;

import tm.move.move_name;

namespace technicalmachine {

export constexpr auto is_self_KO(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Explosion:
		case MoveName::Healing_Wish:
		case MoveName::Lunar_Dance:
		case MoveName::Memento:
		case MoveName::Misty_Explosion:
		case MoveName::Self_Destruct:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
