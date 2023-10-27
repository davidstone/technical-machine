// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.blocks_selection_and_execution;

import tm.move.category;
import tm.move.is_blocked_by_gravity;
import tm.move.is_healing;
import tm.move.is_switch;
import tm.move.move;
import tm.move.move_name;
import tm.move.pp;

import tm.pokemon.any_pokemon;

import tm.environment;

import containers;

namespace technicalmachine {

constexpr auto imprison(MoveName const move, any_active_pokemon auto const other) {
	return other.used_imprison() and containers::any_equal(other.regular_moves(), move);
}

export constexpr auto blocks_selection_and_execution_attempt(any_active_pokemon auto const user, Move const move, any_active_pokemon auto const other) {
	if (!is_regular(move.name())) {
		return false;
	}
	return
		no_pp(move.pp()) or
		(user.is_disabled(move.name())) or
		(user.heal_block_is_active() and (is_healing(move.name()))) or
		(imprison(move.name(), other));
}

constexpr  auto is_blocked_by_taunt(MoveName const move) {
	return !is_damaging(move);
}

export constexpr auto blocks_selection_and_execution(any_active_pokemon auto const user, MoveName const move, Environment const environment) {
	return !is_switch(move) and (
		(user.is_taunted() and is_blocked_by_taunt(move)) or
		(environment.gravity() and is_blocked_by_gravity(move))
	);
}

} // namespace technicalmachine
