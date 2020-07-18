// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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
