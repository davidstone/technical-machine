// Entry hazards
// Copyright (C) 2014 David Stone
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

#include "hash.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;
class MutableActivePokemon;
class Team;
class Weather;

// Using an entry hazard puts them down on the opponent's side of the field.

class EntryHazards {
public:
	constexpr auto spikes() const {
		return bounded::make<bounded::null_policy>(m_spikes);
	}
	constexpr auto stealth_rock() const {
		return m_stealth_rock;
	}
	constexpr auto toxic_spikes() const {
		return bounded::make<bounded::null_policy>(m_toxic_spikes);
	}
	auto add_spikes() -> void;
	auto add_toxic_spikes() -> void;
	auto clear_toxic_spikes() -> void;
	auto add_stealth_rock() -> void;
private:
	bounded::clamped_integer<0, 3> m_spikes = 0_bi;
	bounded::clamped_integer<0, 2> m_toxic_spikes = 0_bi;
	bool m_stealth_rock = false;
};

constexpr auto hash(EntryHazards const hazards) noexcept {
	return hash(hazards.spikes(), hazards.stealth_rock(), hazards.toxic_spikes());
}

auto operator==(EntryHazards lhs, EntryHazards rhs) -> bool;
auto operator!=(EntryHazards lhs, EntryHazards rhs) -> bool;

auto apply(EntryHazards & hazards, MutableActivePokemon switcher, Weather weather) -> void;

}	// namespace technicalmachine
