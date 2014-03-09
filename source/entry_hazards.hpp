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

#ifndef ENTRY_HAZARDS_HPP_
#define ENTRY_HAZARDS_HPP_

#include <bounded_integer/bounded_integer.hpp>
#include <cstdint>

namespace technicalmachine {
class ActivePokemon;
class Team;
class Weather;

// Using an entry hazard puts them down on the opponent's side of the field.

class EntryHazards {
public:
	EntryHazards();
	auto spikes() const {
		return bounded_integer::make_bounded<bounded_integer::null_policy>(m_spikes);
	}
	auto stealth_rock() const {
		return m_stealth_rock;
	}
	auto toxic_spikes() const {
		return bounded_integer::make_bounded<bounded_integer::null_policy>(m_toxic_spikes);
	}
	auto add_spikes() -> void;
	auto add_toxic_spikes() -> void;
	auto clear_toxic_spikes() -> void;
	auto add_stealth_rock() -> void;
	typedef uint64_t hash_type;
	auto hash() const -> hash_type;
	static auto max_hash() -> hash_type;
private:
	bounded_integer::clamped_integer<0, 3> m_spikes;
	bounded_integer::clamped_integer<0, 2> m_toxic_spikes;
	bool m_stealth_rock;
};

auto operator==(EntryHazards lhs, EntryHazards rhs) -> bool;
auto operator!=(EntryHazards lhs, EntryHazards rhs) -> bool;

auto apply(EntryHazards & hazards, ActivePokemon & switcher, Weather const & weather) -> void;

}	// namespace technicalmachine
#endif	// ENTRY_HAZARDS_HPP_
