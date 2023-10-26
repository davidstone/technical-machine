// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.is_healing;

import tm.move.move_name;

namespace technicalmachine {

export constexpr auto is_healing(MoveName const name) {
	switch (name) {
		case MoveName::Heal_Order:
		case MoveName::Milk_Drink:
		case MoveName::Moonlight:
		case MoveName::Morning_Sun:
		case MoveName::Recover:
		case MoveName::Rest:
		case MoveName::Roost:
		case MoveName::Slack_Off:
		case MoveName::Soft_Boiled:
		case MoveName::Swallow:
		case MoveName::Synthesis:
		case MoveName::Wish:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
