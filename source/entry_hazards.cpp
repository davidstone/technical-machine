// Copyright (C) 2015 David Stone
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

#include "entry_hazards.hpp"

#include <cstdint>

#include "heal.hpp"
#include "team.hpp"
#include "weather.hpp"

#include "pokemon/active_pokemon.hpp"

#include "type/effectiveness.hpp"

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto removes_toxic_spikes(ActivePokemon const switcher) {
	return is_type(switcher, Type::Poison, is_roosting(switcher));
}

auto apply_toxic_spikes(EntryHazards const & hazards, MutableActivePokemon switcher, Weather const weather) {
	if (hazards.toxic_spikes() == 1_bi) {
		Status::apply<Statuses::poison>(switcher, weather);
	} else {
		Status::apply<Statuses::poison_toxic>(switcher, weather);
	}
}

}	// namespace

auto EntryHazards::add_spikes() -> void {
	++m_spikes;
}

auto EntryHazards::add_toxic_spikes() -> void {
	++m_toxic_spikes;
}

auto EntryHazards::clear_toxic_spikes() -> void {
	m_toxic_spikes = 0_bi;
}

auto EntryHazards::add_stealth_rock() -> void {
	m_stealth_rock = true;
}

auto apply(EntryHazards & hazards, MutableActivePokemon switcher, Weather const weather) -> void {
	if (get_ability(switcher).blocks_secondary_damage())
		return;

	if (grounded(switcher, weather)) {
		if (hazards.toxic_spikes() != 0_bi) {
			if (removes_toxic_spikes(switcher)) {
				hazards.clear_toxic_spikes();
			}
			else {
				apply_toxic_spikes(hazards, switcher, weather);
			}
		}
		if (hazards.spikes() != 0_bi) {
			heal(switcher, make_rational(-hazards.spikes() - 1_bi, 16_bi));
		}
	}
	if (hazards.stealth_rock()) {
		heal(switcher, make_rational(-1_bi, 8_bi) * Effectiveness(Type::Rock, switcher));
	}
}

auto operator== (EntryHazards const lhs, EntryHazards const rhs) -> bool {
	return
		lhs.spikes() == rhs.spikes() and
		lhs.toxic_spikes() == rhs.toxic_spikes() and
		lhs.stealth_rock() == rhs.stealth_rock();
}

}	// namespace technicalmachine
