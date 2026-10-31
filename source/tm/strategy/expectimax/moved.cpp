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
