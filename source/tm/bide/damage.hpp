// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/stat/hp.hpp>
#include <tm/operators.hpp>
#include <tm/saturating_add.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct BideDamage {
	constexpr auto add(HP::current_type const damage) -> void {
		saturating_add(m_damage, damage);
	}

	constexpr auto release() {
		auto const output_damage = m_damage * 2_bi;
		m_damage = 0_bi;
		return output_damage;
	}

	friend auto operator==(BideDamage, BideDamage) -> bool = default;
	friend constexpr auto compress(BideDamage const value) {
		return compress(value.m_damage);
	}
private:
	// This is the greatest range that matters since anything more is overkill
	bounded::integer<0, (HP::max_value + 1) / 2> m_damage = 0_bi;
};

}	// namespace technicalmachine
