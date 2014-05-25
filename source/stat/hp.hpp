// HP class
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

#ifndef STAT__HP_HPP_
#define STAT__HP_HPP_

#include <bounded_integer/bounded_integer.hpp>
#include "ev.hpp"
#include "iv.hpp"
#include "../pokemon/level.hpp"
#include "../pokemon/species_forward.hpp"
#include "../rational.hpp"

namespace technicalmachine {
class Level;
using namespace bounded::literal;

class HP {
public:
	static constexpr unsigned max_value = 714;
	using max_type = bounded::integer<1, max_value>;
	using current_type = bounded::integer<0, max_value>;
	
	HP(Species species, Level level, EV ev = EV(0_bi));
	template<typename T>
	auto operator=(T const & value) -> HP & {
		m_value = value;
		return *this;
	}
	auto current() const -> current_type;
	auto max() const -> max_type;
	EV ev;
	IV iv;
private:
	bounded::dynamic_max_integer<0, max_value, bounded::clamp_policy> m_value;
};

template<typename T>
auto operator+=(HP & lhs, T const & rhs) -> HP & {
	lhs = lhs.current() + bounded::make<bounded::null_policy>(rhs);
	return lhs;
}

template<typename T>
auto operator-=(HP & lhs, T const & rhs) -> HP & {
	return lhs += -rhs;
}

template<intmax_t min, intmax_t max, typename overflow>
auto operator==(HP const lhs, bounded::integer<min, max, overflow> const rhs) -> bool {
	return lhs.current() == rhs;
}
template<intmax_t min, intmax_t max, typename overflow>
auto operator==(bounded::integer<min, max, overflow> const lhs, HP const rhs) -> bool {
	return rhs == lhs.current();
}

template<intmax_t min, intmax_t max, typename overflow>
auto operator!=(HP const lhs, bounded::integer<min, max, overflow> const rhs) -> bool {
	return lhs.current() != rhs;
}
template<intmax_t min, intmax_t max, typename overflow>
auto operator!=(bounded::integer<min, max, overflow> const lhs, HP const rhs) -> bool {
	return rhs != lhs.current();
}

inline auto hash(HP const hp) noexcept {
	return hp.current();
}

inline auto max_hash(HP const hp) noexcept {
	return hp.max();
}

}	// namespace technicalmachine
#endif	// STAT__HP_HPP_
