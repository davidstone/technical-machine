// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.embargo;

import tm.pokemon.end_of_turn_counter;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
using EmbargoCounter = EndOfTurnCounter<generation >= Generation::four, 5>;

static_assert(!EmbargoCounter<Generation::one>().is_active());
static_assert(!EmbargoCounter<Generation::five>().is_active());

} // namespace technicalmachine
