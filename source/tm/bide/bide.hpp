// Handles bide damage and when it activates
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/bide/damage.hpp>
#include <tm/bide/duration.hpp>

#include <tm/compress.hpp>
#include <tm/operators.hpp>
#include <tm/stat/hp.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {

struct Bide {
	constexpr auto add_damage(HP::current_type const damage) {
		m_damage.add(damage);
	}
	[[nodiscard]] constexpr auto advance_one_turn() {
		return BOUNDED_CONDITIONAL(m_duration.advance_one_turn(), m_damage.release(), bounded::none);
	}

	friend auto operator==(Bide, Bide) -> bool = default;
	friend constexpr auto compress(Bide const value) {
		return compress_combine(value.m_damage, value.m_duration);
	}
private:
	BideDamage m_damage;
	BideDuration m_duration;
};

}	// namespace technicalmachine
