// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/move/moves.hpp>

#include <tm/pokemon/active_pokemon.hpp>

#include <tm/weather.hpp>

namespace technicalmachine {

inline auto will_be_recharge_turn(ActivePokemon const user, Moves const move, Ability const other_ability, Weather const weather) {
	auto const blocks_weather = weather_is_blocked_by_ability(user.ability(), other_ability);
	switch (move) {
		case Moves::Solar_Beam: return !weather.sun(blocks_weather) and !user.is_charging_up();
		default: return false;
	}
}

}	// namespace technicalmachine
