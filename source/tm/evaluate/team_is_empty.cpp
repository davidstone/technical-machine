// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.team_is_empty;

import tm.any_team;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto team_is_empty(any_team auto const & team) {
	return team.size() == 0_bi or (team.size() == 1_bi and team.pokemon().hp().current() == 0_bi);
};

} // namespace technicalmachine
