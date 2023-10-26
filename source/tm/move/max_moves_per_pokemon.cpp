// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.max_moves_per_pokemon;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto max_moves_per_pokemon = 4_bi;

} // namespace technicalmachine
