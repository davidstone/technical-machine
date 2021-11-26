// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/moves.hpp>

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/weather.hpp>

namespace technicalmachine {

auto will_be_recharge_turn(any_active_pokemon auto const user, Moves const move, Ability const other_ability, Weather const weather) {
	auto const blocks_weather = weather_is_blocked_by_ability(user.ability(), other_ability);
	switch (move) {
		case Moves::Solar_Beam: return !weather.sun(blocks_weather) and !user.last_used_move().is_charging_up();
		default: return false;
	}
}

}	// namespace technicalmachine
