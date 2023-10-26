// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.is_blocked_by_gravity;

import tm.move.move_name;

namespace technicalmachine {

export constexpr auto is_blocked_by_gravity(MoveName const move) {
	switch (move) {
		case MoveName::Bounce:
		case MoveName::Fly:
		case MoveName::High_Jump_Kick:
		case MoveName::Jump_Kick:
		case MoveName::Magnet_Rise:
		case MoveName::Splash:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
