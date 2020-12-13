// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/evaluate/best_move.hpp>
#include <tm/evaluate/depth.hpp>
#include <tm/evaluate/evaluate.hpp>
#include <tm/weather.hpp>

#include <tm/generation.hpp>

#include <ostream>

namespace technicalmachine {

template<Generation generation>
auto expectiminimax(Team<generation> const & ai, Team<generation> const & foe, Weather, Evaluate<generation>, Depth, std::ostream & log) -> BestMove;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto expectiminimax<generation>(Team<generation> const & ai, Team<generation> const & foe, Weather, Evaluate<generation>, Depth, std::ostream & log) -> BestMove

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

}	// namespace technicalmachine
