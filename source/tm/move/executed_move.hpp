// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/known_move.hpp>
#include <tm/move/side_effects.hpp>

namespace technicalmachine {

template<Generation generation>
struct ExecutedMove {
	KnownMove move;
	PP pp;
	typename SideEffect<Team<generation>>::Function side_effect;
	bool critical_hit;
};

}	// namespace technicalmachine
