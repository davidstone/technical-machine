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

void switch_pokemon(Team & switcher, Team & other, Weather & weather, TeamIndex const replacement) {
	switcher.reset_switch();

	if (get_hp(switcher.pokemon()) != 0_bi) {
		if (get_ability(switcher.pokemon()).clears_status_on_switch()) {
			get_status(switcher.pokemon()) = Status{};
		}
		switcher.all_pokemon().set_index(replacement);
	}
	else {
		switcher.all_pokemon().remove_active(replacement);
		// If the last Pokemon is fainted; there is nothing left to do.
		if (empty(switcher.all_pokemon())) {
			return;
		}
	}
	apply(switcher.entry_hazards, switcher.pokemon(), weather);
	if (get_hp(switcher.pokemon()) != 0_bi) {
		Ability::activate_on_switch(switcher.pokemon(), other.pokemon(), weather);
	}
	switch_in(switcher.pokemon());
}

}	// namespace technicalmachine
