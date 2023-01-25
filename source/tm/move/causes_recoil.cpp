// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.causes_recoil;

import tm.move.move_name;

namespace technicalmachine {

export constexpr auto causes_recoil(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Brave_Bird:
		case MoveName::Double_Edge:
		case MoveName::Flare_Blitz:
		case MoveName::Head_Smash:
		case MoveName::Submission:
		case MoveName::Take_Down:
		case MoveName::Volt_Tackle:
		case MoveName::Wood_Hammer:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine