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

#pragma once

#include "ev.hpp"
#include "iv.hpp"

#include "../operators.hpp"
#include "../pokemon/level.hpp"
#include "../pokemon/species_forward.hpp"
#include "../rational.hpp"

#include <bounded/integer.hpp>

namespace technicalmachine {
struct Level;
struct Pokemon;
using namespace bounded::literal;

struct HP {
	static constexpr auto max_value = 714U;
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
	return lhs = lhs.current() + bounded::make<bounded::null_policy>(rhs);
}

template<typename T>
auto operator-=(HP & lhs, T const & rhs) -> HP & {
	return lhs += -rhs;
}

template<intmax_t min, intmax_t max, typename overflow>
auto operator==(HP const lhs, bounded::integer<min, max, overflow> const rhs) {
	return lhs.current() == rhs;
}
template<intmax_t min, intmax_t max, typename overflow>
auto operator==(bounded::integer<min, max, overflow> const lhs, HP const rhs) {
	return rhs == lhs.current();
}

inline auto hash(HP const hp) noexcept {
	return std::make_pair(hp.current(), hp.max());
}

}	// namespace technicalmachine
