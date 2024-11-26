// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.encore;

import tm.pokemon.end_of_turn_counter;
import tm.generation;

namespace technicalmachine {

// TODO:
// Ends if PP runs out
// Gen 2,3: 2-6
// Gen 4: 4-8
// Gen 5+: 3
export template<Generation generation>
using EncoreCounter = EndOfTurnCounter<
	generation >= Generation::two,
	8
>;

} // namespace technicalmachine
