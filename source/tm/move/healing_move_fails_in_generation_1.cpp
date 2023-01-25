// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.healing_move_fails_in_generation_1;

import tm.stat.hp;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto healing_move_fails_in_generation_1(HP const hp) {
	auto const difference = hp.max() - hp.current();
	return difference % 256_bi == 255_bi;
}

} // namespace technicalmachine