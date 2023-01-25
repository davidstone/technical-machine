// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.rampage;

import tm.pokemon.end_of_turn_counter;

namespace technicalmachine {

// Outrage, Petal Dance, and Thrash
// TODO: Change the duration in different generations
export using Rampage = EndOfTurnCounter<
	true,
	3
>;

} // namespace technicalmachine
