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

#pragma once

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/operators.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
struct Level;
struct Pokemon;
using namespace bounded::literal;

struct HP {
	static constexpr auto max_value = 714;
	using max_type = bounded::integer<1, max_value>;
	using current_type = bounded::integer<0, max_value>;
	
	HP(Species species, Level level, EV ev_ = EV(0_bi), IV iv_ = IV(31_bi));
	template<typename T>
	auto & operator=(T const & value) {
		m_current = bounded::min(value, m_max);
		return *this;
	}
	auto current() const -> current_type;
	auto max() const -> max_type;
	auto ev() const -> EV;
	auto iv() const -> IV;
private:
	EV m_ev;
	IV m_iv;
	max_type m_max;
	bounded::clamped_integer<0, max_value> m_current;
};

auto set_hp_ev(Pokemon & pokemon, EV ev) -> void;
auto set_hp_ev(Pokemon & pokemon, EV ev, IV iv) -> void;

template<typename T>
auto operator+=(HP & lhs, T const & rhs) -> HP & {
	return lhs = lhs.current() + rhs;
}

template<typename T>
auto operator-=(HP & lhs, T const & rhs) -> HP & {
	return lhs += -rhs;
}

template<auto min, auto max, typename overflow>
auto compare(HP const lhs, bounded::integer<min, max, overflow> const rhs) {
	return compare(lhs.current(), rhs);
}
template<auto min, auto max, typename overflow>
auto compare(bounded::integer<min, max, overflow> const lhs, HP const rhs) {
	return compare(rhs, lhs.current());
}

template<auto min, auto max, typename overflow>
auto operator==(HP const lhs, bounded::integer<min, max, overflow> const rhs) {
	return lhs.current() == rhs;
}
template<auto min, auto max, typename overflow>
auto operator==(bounded::integer<min, max, overflow> const lhs, HP const rhs) {
	return rhs == lhs.current();
}

}	// namespace technicalmachine
