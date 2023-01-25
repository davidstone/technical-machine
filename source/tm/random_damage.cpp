// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.random_damage;

import tm.rational;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export struct RandomDamage {
	constexpr auto operator()() const {
		return rational(m_r, 100_bi);
	}

private:
	bounded::integer<85, 100> m_r = 100_bi;
};

} // namespace technicalmachine
