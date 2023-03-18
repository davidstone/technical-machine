// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ability_blocks_weather;

import tm.ability;

namespace technicalmachine {

constexpr auto ability_blocks_weather(Ability const ability) {
	switch (ability) {
		case Ability::Air_Lock:
		case Ability::Cloud_Nine:
			return true;
		default:
			return false;
	}
}
export constexpr auto ability_blocks_weather(Ability const ability1, Ability const ability2) {
	return ability_blocks_weather(ability1) or ability_blocks_weather(ability2);
}

} // namespace technicalmachine
