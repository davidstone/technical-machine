// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.block;

import tm.move.category;
import tm.move.max_moves_per_pokemon;
import tm.move.move;
import tm.move.move_name;
import tm.move.pp;
import tm.move.is_switch;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.pokemon_collection;
import tm.pokemon.pokemon;
import tm.pokemon.potentially_selectable_moves;

import tm.status.status;

import tm.ability;
import tm.ability_blocks_switching;
import tm.any_team;
import tm.environment;
import tm.generation;
import tm.item;
import tm.switch_decision_required;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

template<typename PokemonType>
constexpr auto would_switch_to_same_pokemon(PokemonCollection<PokemonType> const & collection, MoveName const move) {
	return to_replacement(move) == collection.index();
}

template<any_active_pokemon ActivePokemonType>
constexpr auto is_blocked_from_switching(ActivePokemonType const user, ActivePokemonType const other, Environment const environment) {
	auto const block_attempted = ability_blocks_switching(other.ability(), user, environment) or user.trapped();
	auto const result = block_attempted and !allows_switching(user.item(environment));
	return result;
}

template<Generation generation>
constexpr auto is_illegal_switch(Team<generation> const & user, MoveName const move, ActivePokemon<generation> const other, Environment const environment) {
	return is_switch(move) and (
		would_switch_to_same_pokemon(user.all_pokemon(), move) or
		is_blocked_from_switching(user.pokemon(), other, environment)
	);
}

constexpr auto is_healing(MoveName const name) {
	switch (name) {
		case MoveName::Heal_Order:
		case MoveName::Milk_Drink:
		case MoveName::Moonlight:
		case MoveName::Morning_Sun:
		case MoveName::Recover:
		case MoveName::Rest:
		case MoveName::Roost:
		case MoveName::Slack_Off:
		case MoveName::Soft_Boiled:
		case MoveName::Swallow:
		case MoveName::Synthesis:
		case MoveName::Wish:
			return true;
		default:
			return false;
	}
}

constexpr auto imprison(MoveName const move, any_active_pokemon auto const other) {
	return other.used_imprison() and containers::any_equal(other.regular_moves(), move);
}

// Things that both block selection and block execution in between sleep and confusion
constexpr auto block1(any_active_pokemon auto const user, Move const move, any_active_pokemon auto const other) {
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

constexpr auto is_blocked_by_gravity(MoveName const move) {
	switch (move) {
		case MoveName::Bounce:
		case MoveName::Fly:
		case MoveName::High_Jump_Kick:
		case MoveName::Jump_Kick:
		case MoveName::Magnet_Rise:
		case MoveName::Splash:
			return true;
		default:
			return false;
	}
}

// Things that both block selection and block execution after flinching
constexpr auto block2(any_active_pokemon auto const user, MoveName const move, Environment const environment) {
	return !is_switch(move) and (
		(user.is_taunted() and is_blocked_by_taunt(move)) or
		(environment.gravity() and is_blocked_by_gravity(move))
	);
}

constexpr auto blocked_by_torment(any_active_pokemon auto const user, MoveName const move) {
	return user.is_tormented() and user.last_used_move().name() == move and not is_switch(move) and move != MoveName::Struggle;
}

constexpr auto is_locked_in(any_active_pokemon auto const user, Environment const environment) {
	return user.is_encored() or user.last_used_move().is_locked_in_by_move() or is_choice_item(user.item(environment));
}

constexpr auto is_locked_in_to_different_move(any_active_pokemon auto const user, MoveName const move, Environment const environment) {
	if (not is_locked_in(user, environment)) {
		return false;
	}
	auto const last_move = user.last_used_move().name();
	return not is_switch(last_move) and last_move != move;
}

constexpr auto is_blocked_due_to_lock_in(any_active_pokemon auto const user, MoveName const move, Environment const environment) {
	return !is_regular(move) ?
		user.last_used_move().is_locked_in_by_move() :
		is_locked_in_to_different_move(user, move, environment);
}

template<any_team TeamType>
constexpr auto is_legal_selection(TeamType const & user, Move const move, TeamType const & other, Environment const environment, bool const found_selectable_move) {
	BOUNDED_ASSERT(move != MoveName::Hit_Self);
	if (switch_decision_required(user)) {
		return is_switch(move.name()) and !would_switch_to_same_pokemon(user.all_pokemon(), move.name());
	}
	auto const is_pass = move == MoveName::Pass;
	if (switch_decision_required(other)) {
		return is_pass;
	}
	auto const user_pokemon = user.pokemon();
	if (user_pokemon.last_used_move().moved_this_turn()) {
		return is_pass;
	}
	if (move == MoveName::Struggle) {
		return !found_selectable_move;
	}
	auto const other_pokemon = other.pokemon();
	return
		!is_pass and
		!is_blocked_due_to_lock_in(user_pokemon, move.name(), environment) and
		!is_illegal_switch(user, move.name(), other_pokemon, environment) and
		!block1(user_pokemon, move, other_pokemon) and
		!block2(user_pokemon, move.name(), environment) and
		!blocked_by_torment(user_pokemon, move.name());
}

export template<any_team TeamType>
constexpr auto legal_selections(TeamType const & user, TeamType const & other, Environment const environment) -> LegalSelections {
	auto result = LegalSelections();
	for (auto const move : potentially_selectable_moves(user)) {
		bool const found_selectable_move = !containers::is_empty(result);
		if (is_legal_selection(user, move, other, environment, found_selectable_move)) {
			containers::unsafe_push_back(result, move.name());
		}
	}
	BOUNDED_ASSERT(!containers::is_empty(result));
	return result;
}


constexpr bool is_blocked_by_freeze(any_active_pokemon auto const user, MoveName const move) {
	return is_frozen(user.status()) and !thaws_user(move);
}

export constexpr bool usable_while_sleeping(MoveName const move) {
	switch (move) {
		case MoveName::Sleep_Talk:
		case MoveName::Snore:
			return true;
		default:
			return false;
	}
}

template<any_active_pokemon UserPokemon>
constexpr bool is_blocked_by_sleep(UserPokemon const user, MoveName const move, bool const user_was_asleep) {
	return generation_from<UserPokemon> == Generation::one ? user_was_asleep : usable_while_sleeping(move) != is_sleeping(user.status());
}

constexpr auto is_blocked_due_to_status(any_active_pokemon auto const user, MoveName const move, bool const user_was_asleep) {
	return is_blocked_by_freeze(user, move) or is_blocked_by_sleep(user, move, user_was_asleep);
}

export constexpr auto can_attempt_move_execution(any_active_pokemon auto const user, Move const move, any_active_pokemon auto const other, bool const user_was_asleep) -> bool {
	if (is_switch(move.name())) {
		return true;
	}
	if (user.hp().current() == 0_bi) {
		return false;
	}

	auto const blocked_due_to_status = is_blocked_due_to_status(user, move.name(), user_was_asleep);
	if (blocked_due_to_status or block1(user, move, other) or user.is_loafing()) {
		return false;
	}
	return true;
}

export constexpr auto can_execute_move(any_active_pokemon auto const user, Move const move, Environment const environment, bool const is_recharging, bool const is_fully_paralyzed) -> bool {
	auto const switching = is_switch(move.name());
	if (switching or move.name() == MoveName::Hit_Self) {
		BOUNDED_ASSERT(!is_recharging or (switching and user.hp().current() == 0_bi));
		return true;
	}
	return !user.flinched() and !block2(user, move.name(), environment) and !is_fully_paralyzed and !is_recharging;
}

} // namespace technicalmachine
