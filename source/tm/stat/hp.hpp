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
enum class Generation : std::uint8_t;
struct Level;
using namespace bounded::literal;

struct HP {
	static constexpr auto max_value = 714;
	using max_type = bounded::integer<1, max_value>;
	using current_type = bounded::integer<0, max_value>;
	
	HP(Generation generation, Species species, Level level, EV ev_ = EV(0_bi), IV iv_ = IV(31_bi));
	auto & operator=(auto const & value) & {
		m_current = bounded::min(value, m_max);
		return *this;
	}
	auto current() const -> current_type {
		return m_current;
	}

	auto max() const -> max_type {
		return m_max;
	}

	auto ev() const -> EV {
		return m_ev;
	}

	auto iv() const -> IV {
		return m_iv;
	}

private:
	EV m_ev;
	IV m_iv;
	max_type m_max;
	bounded::clamped_integer<0, max_value> m_current;
};

auto operator<=>(HP const lhs, bounded::bounded_integer auto const rhs) {
	return lhs.current() <=> rhs;
}
auto operator<=>(bounded::bounded_integer auto const lhs, HP const rhs) {
	return rhs <=> lhs.current();
}

auto operator==(HP const lhs, bounded::bounded_integer auto const rhs) {
	return lhs.current() == rhs;
}
auto operator==(bounded::bounded_integer auto const lhs, HP const rhs) {
	return rhs == lhs.current();
}

}	// namespace technicalmachine
