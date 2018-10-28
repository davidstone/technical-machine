// Copyright (C) 2018 David Stone
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

#include <tm/operators.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;
struct MutableActivePokemon;
struct Team;
struct Weather;

// Using an entry hazard puts them down on the opponent's side of the field.

struct EntryHazards {
	constexpr auto spikes() const {
		return m_spikes;
	}
	constexpr auto stealth_rock() const {
		return m_stealth_rock;
	}
	constexpr auto toxic_spikes() const {
		return m_toxic_spikes;
	}
	constexpr auto add_spikes() {
		++m_spikes;
	}
	constexpr auto add_toxic_spikes() {
		++m_toxic_spikes;
	}
	constexpr auto clear_toxic_spikes() {
		m_toxic_spikes = 0_bi;
	}
	constexpr auto add_stealth_rock() {
		m_stealth_rock = true;
	}
private:
	bounded::clamped_integer<0, 3> m_spikes = 0_bi;
	bounded::clamped_integer<0, 2> m_toxic_spikes = 0_bi;
	bool m_stealth_rock = false;
};

constexpr auto operator==(EntryHazards const lhs, EntryHazards const rhs) {
	return
		lhs.spikes() == rhs.spikes() and
		lhs.toxic_spikes() == rhs.toxic_spikes() and
		lhs.stealth_rock() == rhs.stealth_rock();
}

auto apply(EntryHazards & hazards, MutableActivePokemon switcher, Weather weather) -> void;

}	// namespace technicalmachine
