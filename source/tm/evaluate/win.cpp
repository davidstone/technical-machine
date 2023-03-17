// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.evaluate.win;

import tm.evaluate.victory;

import tm.any_team;
import tm.generation;

import bounded;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

// Returns victory if the battle is won. Returns -victory if the battle is
// lost. Returns 0 otherwise.
export template<any_team TeamType>
constexpr auto win(TeamType const & team1, TeamType const & team2) -> tv::optional<double> {
	constexpr auto generation = generation_from<TeamType>;
	auto single_team_win = [](TeamType const & team) {
		BOUNDED_ASSERT(team.size() != 0_bi);
		return team.size() == 1_bi and team.pokemon().hp().current() == 0_bi ?
			team.is_me() ? -victory<generation> : victory<generation> :
			0.0;
	};
	auto const win1 = single_team_win(team1);
	auto const win2 = single_team_win(team2);
	if (win1 != 0.0 or win2 != 0.0) {
		return win1 + win2;
	}
	return tv::none;
}

} // namespace technicalmachine
