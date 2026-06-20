// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.expectimax.moved;

import tm.pokemon.any_pokemon;

import tm.any_team;

namespace technicalmachine {

export constexpr auto moved(any_active_pokemon auto const pokemon) -> bool {
	return pokemon.last_used_move().moved_this_turn();
}
export constexpr auto moved(any_team auto const & team) -> bool {
	return moved(team.pokemon());
}

} // namespace technicalmachine
