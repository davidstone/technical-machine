// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.magnet_rise;

import tm.pokemon.end_of_turn_counter;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
using MagnetRise = EndOfTurnCounter<
	generation >= Generation::four,
	5
>;

} // namespace technicalmachine
