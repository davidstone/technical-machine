// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/evaluate/best_move.hpp>
#include <tm/evaluate/depth.hpp>
#include <tm/evaluate/evaluate.hpp>

#include <tm/for_each_generation.hpp>
#include <tm/generation.hpp>
#include <tm/weather.hpp>

#include <random>

namespace technicalmachine {

template<Generation generation>
auto expectiminimax(Team<generation> const & ai, Team<generation> const & foe, Weather, Evaluate<generation>, Depth, std::mt19937 & random_engine) -> BestMove;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto expectiminimax(Team<generation> const & ai, Team<generation> const & foe, Weather, Evaluate<generation>, Depth, std::mt19937 & random_engine) -> BestMove

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXTERN_INSTANTIATION);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

} // namespace technicalmachine
