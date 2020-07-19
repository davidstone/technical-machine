// Copyright (C) 2020 David Stone
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

#include <tm/type/effectiveness.hpp>

#include <tm/compress.hpp>
#include <tm/entry_hazards.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/operators.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

template<Generation>
struct ActivePokemon;
template<Generation>
struct MutableActivePokemon;

template<Generation generation>
constexpr auto removes_toxic_spikes(ActivePokemon<generation> const switcher) {
	return is_type(switcher, Type::Poison);
}

template<Generation generation>
constexpr auto apply_toxic_spikes(EntryHazards<generation> const & hazards, MutableActivePokemon<generation> switcher, Weather const weather) {
	auto const status = hazards.toxic_spikes() == 1_bi ? Statuses::poison : Statuses::toxic;
	apply_status_to_self(status, switcher, weather);
}

template<Generation generation>
constexpr auto apply(EntryHazards<generation> & hazards, MutableActivePokemon<generation> switcher, Weather const weather) -> void {
	if (switcher.item(weather) == Item::Heavy_Duty_Boots) {
		return;
	}

	if (grounded(as_const(switcher), weather)) {
		if (hazards.toxic_spikes() != 0_bi) {
			if (removes_toxic_spikes(as_const(switcher))) {
				hazards.clear_toxic_spikes();
			} else if (generation >= Generation::five or !blocks_secondary_damage(switcher.ability())) {
				apply_toxic_spikes(hazards, switcher, weather);
			}
		}
		if (hazards.spikes() != 0_bi) {
			heal(switcher, weather, rational(-hazards.spikes() - 1_bi, 16_bi));
		}
	}
	if (hazards.stealth_rock()) {
		heal(switcher, weather, rational(-1_bi, 8_bi) * Effectiveness(generation, Type::Rock, switcher.types()));
	}
}

} // namespace technicalmachine
