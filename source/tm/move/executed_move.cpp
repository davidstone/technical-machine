// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.executed_move;

import tm.move.known_move;
import tm.move.pp;
import tm.move.side_effect_function;

import tm.any_team;
import tm.contact_ability_effect;

namespace technicalmachine {

export template<any_team UserTeam>
struct ExecutedMove {
	KnownMove move;
	PP pp;
	SideEffectFunction<UserTeam> side_effect;
	bool critical_hit;
	ContactAbilityEffect contact_ability_effect;
	bool action_ends;
};

} // namespace technicalmachine
