// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/known_move.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/any_team.hpp>

namespace technicalmachine {

template<any_team UserTeam>
struct ExecutedMove {
	KnownMove move;
	PP pp;
	typename SideEffect<UserTeam>::Function side_effect;
	bool critical_hit;
};

}	// namespace technicalmachine
