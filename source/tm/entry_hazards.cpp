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

#include <tm/entry_hazards.hpp>

#include <cstdint>

#include <tm/heal.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/pokemon/active_pokemon.hpp>

#include <tm/type/effectiveness.hpp>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto removes_toxic_spikes(ActivePokemon const switcher) {
	return is_type(switcher, Type::Poison);
}

auto apply_toxic_spikes(Generation const generation, EntryHazards const & hazards, MutableActivePokemon switcher, Weather const weather) {
	auto const status = hazards.toxic_spikes() == 1_bi ? Statuses::poison : Statuses::toxic;
	apply_status_to_self(generation, status, switcher, weather);
}

}	// namespace

auto apply(Generation const generation, EntryHazards & hazards, MutableActivePokemon switcher, Weather const weather) -> void {
	if (switcher.item(generation, weather) == Item::Heavy_Duty_Boots) {
		return;
	}

	if (grounded(generation, switcher, weather)) {
		if (hazards.toxic_spikes() != 0_bi) {
			if (removes_toxic_spikes(switcher)) {
				hazards.clear_toxic_spikes();
			} else if (generation >= Generation::five or !blocks_secondary_damage(switcher.ability())) {
				apply_toxic_spikes(generation, hazards, switcher, weather);
			}
		}
		if (hazards.spikes() != 0_bi) {
			heal(generation, switcher, weather, rational(-hazards.spikes() - 1_bi, 16_bi));
		}
	}
	if (hazards.stealth_rock()) {
		heal(generation, switcher, weather, rational(-1_bi, 8_bi) * Effectiveness(generation, Type::Rock, switcher.types()));
	}
}

}	// namespace technicalmachine
