// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.switch_decision_required;

import tm.any_team;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto switch_decision_required(any_team auto const & team) {
	if (team.size() == 1_bi) {
		return false;
	}
	auto const pokemon = team.pokemon();
	return pokemon.hp().current() == 0_bi or pokemon.last_used_move().is_delayed_switching();
}

} // namespace technicalmachine
