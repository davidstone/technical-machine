// Switching-related functions
// Copyright (C) 2016 David Stone
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

#include "switch.hpp"

#include "ability.hpp"
#include "entry_hazards.hpp"
#include "team.hpp"
#include "weather.hpp"

#include "pokemon/pokemon.hpp"

#include "stat/stat.hpp"

namespace technicalmachine {

void switch_pokemon(Team & switcher_team, Team & other, Weather & weather, TeamIndex const replacement) {
	switcher_team.reset_switch();

	if (get_hp(switcher_team.pokemon()) != 0_bi) {
		switcher_team.all_pokemon().set_index(replacement);
	} else {
		switcher_team.all_pokemon().remove_active(replacement);
		// If the last Pokemon is fainted; there is nothing left to do.
		if (empty(switcher_team.all_pokemon())) {
			return;
		}
	}

	auto const switcher = switcher_team.pokemon();
	switch_in(switcher);
	apply(switcher_team.entry_hazards, switcher, weather);
	if (get_hp(switcher) != 0_bi) {
		Ability::activate_on_switch(switcher, other.pokemon(), weather);
	}
}

}	// namespace technicalmachine
