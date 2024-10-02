// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.will_be_recharge_turn;

import tm.move.move_name;

import tm.pokemon.last_used_move;

import tm.ability;
import tm.weather;

namespace technicalmachine {

export auto will_be_recharge_turn(LastUsedMove const last_used_move, MoveName const move, Weather const weather) {
	switch (move) {
		case MoveName::Solar_Beam:
			return weather != Weather::sun and !last_used_move.is_charging_up();
		default:
			return false;
	}
}

} // namespace technicalmachine
