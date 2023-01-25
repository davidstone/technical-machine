// Convert between a switch and the equivalent Pokemon index
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.move.is_switch;

import tm.move.move_name;

import tm.pokemon.max_pokemon_per_team;

import bounded;

namespace technicalmachine {

export constexpr auto is_switch(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Switch0:
		case MoveName::Switch1:
		case MoveName::Switch2:
		case MoveName::Switch3:
		case MoveName::Switch4:
		case MoveName::Switch5:
			return true;
		default:
			return false;
	}
}

export constexpr auto is_delayed_switch(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Flip_Turn:
		case MoveName::U_turn:
			return true;
		default:
			return false;
	}
}

export constexpr auto to_switch(TeamIndex const replacement) -> MoveName {
	return static_cast<MoveName>(replacement + bounded::constant<MoveName::Switch0>);
}

export constexpr auto to_replacement(MoveName const move) -> TeamIndex {
	BOUNDED_ASSERT(is_switch(move));
	return bounded::assume_in_range<TeamIndex>(bounded::integer(move) - bounded::constant<MoveName::Switch0>);
}

} // namespace technicalmachine
