export module tm.team_is_empty;

import tm.any_team;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto team_is_empty(any_team auto const & team) {
	return team.size() == 0_bi or (team.size() == 1_bi and team.pokemon().hp().current() == 0_bi);
};

} // namespace technicalmachine
