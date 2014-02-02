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
using namespace bounded_integer::literal;

class HP {
public:
	static constexpr unsigned max_value = 714;
	using max_type = bounded_integer::native_integer<1, max_value>;
	using current_type = bounded_integer::native_integer<0, max_value>;
	
	HP(Species species, Level level, EV ev = EV(0_bi));
	template<typename T>
	HP & operator=(T const & value) {
		m_value = value;
		return *this;
	}
	current_type current() const;
	max_type max() const;
	EV ev;
	IV iv;
private:
	bounded_integer::clamped_integer<0, max_value, bounded_integer::bounds::dynamic_max> m_value;
};

template<typename T>
HP & operator+=(HP & lhs, T const & rhs) {
	lhs = lhs.current() + bounded_integer::make_bounded<bounded_integer::null_policy>(rhs);
	return lhs;
}

template<typename T>
HP & operator-=(HP & lhs, T const & rhs) {
	return lhs += -rhs;
}

template<intmax_t min, intmax_t max, typename overflow>
bool operator==(HP const lhs, bounded_integer::bounded_integer<min, max, overflow> const rhs) {
	return lhs.current() == rhs;
}
template<intmax_t min, intmax_t max, typename overflow>
bool operator==(bounded_integer::bounded_integer<min, max, overflow> const lhs, HP const rhs) {
	return rhs == lhs.current();
}

template<intmax_t min, intmax_t max, typename overflow>
bool operator!=(HP const lhs, bounded_integer::bounded_integer<min, max, overflow> const rhs) {
	return lhs.current() != rhs;
}
template<intmax_t min, intmax_t max, typename overflow>
bool operator!=(bounded_integer::bounded_integer<min, max, overflow> const lhs, HP const rhs) {
	return rhs != lhs.current();
}

}	// namespace technicalmachine
#endif	// STAT__HP_HPP_
