// Switching-related functions
// Copyright (C) 2012 David Stone
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

#include "pokemon/pokemon.hpp"

#include "stat/stat.hpp"

namespace technicalmachine {
namespace {

void replace_fainted_pokemon (Team & switcher, Team & other);
void remove_fainted_from_phazing_moves (Team const & switcher, Team & other);

}	// unnamed namespace

void switchpokemon (Team & switcher, Team & other, Weather & weather) {
	switcher.reset_switch();

	if (switcher.pokemon().hp.stat > 0) {
		switcher.pokemon().switch_out();
		// Change the active Pokemon to the one switching in.
		switcher.pokemon.to_replacement();
	}
	else {
		replace_fainted_pokemon (switcher, other);
		// If the last Pokemon is fainted; there is nothing left to do.
		if (switcher.pokemon.is_empty())
			return;
	}
	EntryHazards::apply(switcher, weather);
	if (switcher.pokemon().hp.stat > 0)
		Ability::activate_on_switch (switcher, other, weather);
	switcher.pokemon().switch_in();
}

namespace {

void replace_fainted_pokemon (Team & switcher, Team & other) {
	switcher.remove_pokemon();
	if (switcher.pokemon.is_empty())
		return;
	remove_fainted_from_phazing_moves(switcher, other);
}

void remove_fainted_from_phazing_moves (Team const & switcher, Team & other) {
	other.pokemon.for_each([& switcher](Pokemon & pokemon) {
		pokemon.move.for_each([& switcher](Move & move) {
			if (move.is_phaze())
				move.variable.remove_phazing(switcher.pokemon.real_size());
		});
	});
}

}	// unnamed namespace
}	// namespace technicalmachine
