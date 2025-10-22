// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.bide.damage;

import tm.stat.current_hp;

import tm.compress;
import tm.saturating_add;

import bounded;
import numeric_traits;

namespace technicalmachine {
using namespace bounded::literal;

export struct BideDamage {
	constexpr auto add(CurrentHP const damage) -> void {
		saturating_add(m_damage, damage);
	}

	constexpr auto released() const -> CurrentHP {
		return m_damage * 2_bi;
	}

	friend auto operator==(BideDamage, BideDamage) -> bool = default;
	friend constexpr auto compress(BideDamage const value) {
		return compress(value.m_damage);
	}

private:
	// This is the greatest range that matters since anything more is overkill
	bounded::integer<0, bounded::normalize<(numeric_traits::max_value<CurrentHP> + 1_bi) / 2_bi>> m_damage = 0_bi;
};

} // namespace technicalmachine
