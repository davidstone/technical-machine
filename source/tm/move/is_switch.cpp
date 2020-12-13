// Convert between a switch and the equivalent Pokemon index
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/is_switch.hpp>

#include <tm/move/moves.hpp>

#include <bounded/assert.hpp>

namespace technicalmachine {

auto is_switch(Moves const move) -> bool {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return true;
		default:
			return false;
	}
}

auto to_switch(TeamIndex const replacement) -> Moves {
	return static_cast<Moves>(replacement + bounded::constant<static_cast<int>(Moves::Switch0)>);
}

auto to_replacement(Moves const move) -> TeamIndex {
	BOUNDED_ASSERT(is_switch(move));
	return static_cast<TeamIndex>(bounded::integer(move) - bounded::constant<static_cast<int>(Moves::Switch0)>);
}

}	// namespace technicalmachine
