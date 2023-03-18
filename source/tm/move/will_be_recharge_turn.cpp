// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.will_be_recharge_turn;

import tm.move.move_name;

import tm.pokemon.any_pokemon;

import tm.ability;
import tm.ability_blocks_weather;
import tm.environment;

namespace technicalmachine {

export auto will_be_recharge_turn(any_active_pokemon auto const user, MoveName const move, Ability const other_ability, Environment const environment) {
	switch (move) {
		case MoveName::Solar_Beam:
			return (!environment.sun() or ability_blocks_weather(user.ability(), other_ability)) and !user.last_used_move().is_charging_up();
		default:
			return false;
	}
}

} // namespace technicalmachine
