// Copyright David Stone 2025.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.hp_bucket;

import tm.stat.hp;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

// HPBucket divides HP into equally-sized ranges. So if HPBucket were {0, 1},
// then bucket 0 would be (0%, 50%] and bucket 1 would be (50%, 100%].
export using HPBucket = bounded::integer<0, 6>;

export constexpr auto to_hp_bucket(HP const hp) -> HPBucket {
	return bounded::assume_in_range<HPBucket>(
		bounded::number_of<HPBucket> * (bounded::max(hp.current(), 1_bi) - 1_bi) / hp.max()
	);
}

} // namespace technicalmachine
