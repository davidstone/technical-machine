// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.side_effect_function;

import tm.stat.current_hp;

import tm.any_team;
import tm.environment;
import tm.other_team;

import containers;

namespace technicalmachine {

export template<any_team UserTeam>
using SideEffectFunction = containers::trivial_inplace_function<void(UserTeam & user, OtherTeam<UserTeam> & other, Environment &, CurrentHP) const, 0>;

} // namespace technicalmachine
