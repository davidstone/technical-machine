// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/base_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>

#include <tm/operators.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <bounded/clamp.hpp>
#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct HP {
	static constexpr auto max_value = 714;
	using max_type = bounded::integer<1, max_value>;
	using current_type = bounded::integer<0, max_value>;
	
	HP(BaseStats, Level level, IV iv, EV ev);

	auto & operator=(auto const & value) & {
		m_current = bounded::clamp(value, 0_bi, m_max);
		return *this;
	}
	auto current() const -> current_type {
		return m_current;
	}

	auto max() const -> max_type {
		return m_max;
	}

	friend auto operator==(HP const &, HP const &) -> bool = default;

private:
	max_type m_max;
	current_type m_current;
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
