// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/end_of_turn_counter.hpp>
#include <tm/generation.hpp>

namespace technicalmachine {

template<Generation generation>
using MagnetRise = EndOfTurnCounter<
	generation >= Generation::four,
	4
>;

} // namespace technicalmachine
