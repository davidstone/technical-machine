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

auto apply_toxic_spikes(EntryHazards const & hazards, MutableActivePokemon switcher, Weather const weather) {
	// Irrelevant
	constexpr auto uproar = false;
	apply_status(hazards.toxic_spikes() == 1_bi ? Statuses::poison : Statuses::toxic, switcher, weather, uproar);
}

}	// namespace

auto apply(Generation const generation, EntryHazards & hazards, MutableActivePokemon switcher, Weather const weather) -> void {
	if (blocks_secondary_damage(get_ability(switcher)))
		return;

	if (grounded(switcher, weather)) {
		if (hazards.toxic_spikes() != 0_bi) {
			if (removes_toxic_spikes(switcher)) {
				hazards.clear_toxic_spikes();
			} else {
				apply_toxic_spikes(hazards, switcher, weather);
			}
		}
		if (hazards.spikes() != 0_bi) {
			heal(switcher, rational(-hazards.spikes() - 1_bi, 16_bi));
		}
	}
	if (hazards.stealth_rock()) {
		heal(switcher, rational(-1_bi, 8_bi) * Effectiveness(generation, Type::Rock, get_type(switcher)));
	}
}

}	// namespace technicalmachine
