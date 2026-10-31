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
