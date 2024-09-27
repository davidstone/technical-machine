// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.statistical;

import tm.strategy.strategy;

namespace technicalmachine {

// This models a player that uses moves based on how often the move was used in
// that match-up in the historical logs. So if Tauros uses Body Slam against
// Starmie 26% of the time, then Body Slam is given a weight of 0.26 when up
// against a Starmie.
export auto make_statistical() -> Strategy;

} // namespace technicalmachine
