// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.can_execute_move;

import tm.move.move;
import tm.move.move_name;
import tm.move.usable_while_sleeping;

import tm.pokemon.any_pokemon;

import tm.status.status;

import tm.blocks_selection_and_execution;
import tm.environment;
import tm.generation;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

constexpr bool is_blocked_by_freeze(any_active_pokemon auto const user, MoveName const move) {
	return is_frozen(user.status()) and !thaws_user(move);
}

template<any_active_pokemon UserPokemon>
constexpr bool is_blocked_by_sleep(UserPokemon const user, MoveName const move, bool const user_was_asleep) {
	return generation_from<UserPokemon> == Generation::one ? user_was_asleep : usable_while_sleeping(move) != is_sleeping(user.status());
}

constexpr auto is_blocked_due_to_status(any_active_pokemon auto const user, MoveName const move, bool const user_was_asleep) {
	return is_blocked_by_freeze(user, move) or is_blocked_by_sleep(user, move, user_was_asleep);
}

export constexpr auto can_attempt_move_execution(any_active_pokemon auto const user, Move const move, any_active_pokemon auto const other, bool const user_was_asleep) -> bool {
	if (user.hp().current() == 0_bi) {
		return false;
	}

	auto const blocked_due_to_status = is_blocked_due_to_status(user, move.name(), user_was_asleep);
	if (blocked_due_to_status or blocks_selection_and_execution_attempt(user, move, other) or user.is_loafing()) {
		return false;
	}
	return true;
}

export constexpr auto can_execute_move(any_active_pokemon auto const user, Move const move, Environment const environment, bool const is_recharging, bool const is_fully_paralyzed) -> bool {
	if (move.name() == MoveName::Hit_Self) {
		BOUNDED_ASSERT(!is_recharging);
		return true;
	}
	return !user.flinched() and !blocks_selection_and_execution(user, move.name(), environment) and !is_fully_paralyzed and !is_recharging;
}

} // namespace technicalmachine
