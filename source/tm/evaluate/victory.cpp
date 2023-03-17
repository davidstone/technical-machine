// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.victory;

import tm.evaluate.evaluate;

import tm.generation;
import tm.team;

import bounded;
import numeric_traits;

namespace technicalmachine {
using namespace bounded::literal;

// 100% chance to win
export template<Generation generation>
constexpr auto victory = double(numeric_traits::max_value<decltype(bounded::declval<Evaluate<generation>>()(bounded::declval<Team<generation>>(), bounded::declval<Team<generation>>()))> + 1_bi);

} // namespace technicalmachine
