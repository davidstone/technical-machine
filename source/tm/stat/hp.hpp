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

#include <tm/stat/base_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/operators.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct HP {
	static constexpr auto max_value = 714;
	using max_type = bounded::integer<1, max_value>;
	using current_type = bounded::integer<0, max_value>;
	
	HP(BaseStats, Level level, IV iv_, EV ev_);

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

	friend auto operator==(HP const &, HP const &) -> bool = default;

private:
	IV m_iv;
	EV m_ev;
	max_type m_max;
	bounded::clamped_integer<0, max_value> m_current;
};

auto operator<=>(HP const lhs, bounded::bounded_integer auto const rhs) {
	return lhs.current() <=> rhs;
}

auto operator==(HP const lhs, bounded::bounded_integer auto const rhs) -> bool {
	return lhs.current() == rhs;
}

inline auto healing_move_fails_in_generation_1(HP const hp) {
	auto const difference = hp.max() - hp.current();
	return difference % 256_bi == 255_bi;
}

}	// namespace technicalmachine
