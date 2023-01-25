// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.uproar;

import tm.pokemon.end_of_turn_counter;

namespace technicalmachine {

// TODO: Change behavior in different generations
export using UproarCounter = EndOfTurnCounter<
	true,
	4
>;

} // namespace technicalmachine
