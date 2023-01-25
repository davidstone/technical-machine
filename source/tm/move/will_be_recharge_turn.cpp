// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.will_be_recharge_turn;

import tm.move.move_name;

import tm.pokemon.any_pokemon;

import tm.ability;
import tm.weather;

namespace technicalmachine {

export auto will_be_recharge_turn(any_active_pokemon auto const user, MoveName const move, Ability const other_ability, Weather const weather) {
	auto const blocks_weather = weather_is_blocked_by_ability(user.ability(), other_ability);
	switch (move) {
		case MoveName::Solar_Beam: return !weather.sun(blocks_weather) and !user.last_used_move().is_charging_up();
		default: return false;
	}
}

} // namespace technicalmachine
