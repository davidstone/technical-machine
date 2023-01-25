// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.turn_count;

import bounded;

namespace technicalmachine {

export using TurnCount = bounded::integer<0, 1'000'000'000>;

} // namespace technicalmachine
