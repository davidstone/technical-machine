// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.taunt;

import tm.pokemon.end_of_turn_counter;

import tm.generation;

namespace technicalmachine {

// TODO:
// Gen 3: 2
// Gen 4: 3-5
// Gen 5: 4 target actions
export template<Generation generation>
using TauntCounter = EndOfTurnCounter<
	generation >= Generation::three,
	5
>;

} // namespace technicalmachine
