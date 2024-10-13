// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.is_delayed_switch;

import tm.move.move_name;

namespace technicalmachine {

export constexpr auto is_delayed_switch(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Baton_Pass:
		case MoveName::Flip_Turn:
		case MoveName::U_turn:
		case MoveName::Volt_Switch:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
