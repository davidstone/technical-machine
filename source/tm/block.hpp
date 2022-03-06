// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/ability.hpp>
#include <tm/any_team.hpp>
#include <tm/weather.hpp>

#include <tm/move/category.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/pokemon/any_pokemon.hpp>
#include <tm/pokemon/collection.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/is_empty.hpp>
#include <containers/push_back.hpp>

namespace technicalmachine {

template<Generation generation>
constexpr auto would_switch_to_same_pokemon(PokemonCollection<generation> const & collection, Moves const move) {
	return to_replacement(move) == collection.index();
}

template<any_active_pokemon ActivePokemonType>
constexpr auto is_blocked_from_switching(ActivePokemonType const user, ActivePokemonType const other, Weather const weather) {
	auto const block_attempted = blocks_switching(other.ability(), user, weather) or user.trapped();
	auto const result = block_attempted and !allows_switching(user.item(weather));
	return result;
}

template<Generation generation>
constexpr auto is_illegal_switch(Team<generation> const & user, Moves const move, ActivePokemon<generation> const other, Weather const weather) {
	return is_switch(move) and (
		would_switch_to_same_pokemon(user.all_pokemon(), move) or
		is_blocked_from_switching(user.pokemon(), other, weather)
	);
}

constexpr auto is_healing(Moves const name) {
	switch (name) {
		case Moves::Heal_Order:
		case Moves::Milk_Drink:
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Recover:
		case Moves::Rest:
		case Moves::Roost:
		case Moves::Slack_Off:
		case Moves::Soft_Boiled:
		case Moves::Swallow:
		case Moves::Synthesis:
		case Moves::Wish:
			return true;
		default:
			return false;
	}
}

constexpr auto imprison(Moves const move, any_active_pokemon auto const other) {
	return other.used_imprison() and containers::any_equal(other.regular_moves(), move);
}

// Things that both block selection and block execution in between sleep and confusion
auto block1(any_active_pokemon auto const user, Move const move, any_active_pokemon auto const other) {
	if (!is_regular(move.name())) {
		return false;
	}
	return
		no_pp(move.pp()) or
		(user.is_disabled(move.name())) or
		(user.heal_block_is_active() and (is_healing(move.name()))) or
		(imprison(move.name(), other));
}

inline auto is_blocked_by_taunt(Moves const move) {
	return !is_damaging(move);
}

constexpr auto is_blocked_by_gravity(Moves const move) {
	switch (move) {
		case Moves::Bounce:
		case Moves::Fly:
		case Moves::High_Jump_Kick:
		case Moves::Jump_Kick:
		case Moves::Magnet_Rise:
		case Moves::Splash:
			return true;
		default:
			return false;
	}
}

// Things that both block selection and block execution after flinching
constexpr auto block2(any_active_pokemon auto const user, Moves const move, Weather const weather) {
	return !is_switch(move) and (
		(user.is_taunted() and is_blocked_by_taunt(move)) or
		(weather.gravity() and is_blocked_by_gravity(move))
	);
}

constexpr auto blocked_by_torment(any_active_pokemon auto const user, Moves const move) {
	return user.is_tormented() and user.last_used_move().name() == move and not is_switch(move) and move != Moves::Struggle;
}

constexpr auto is_locked_in(any_active_pokemon auto const user, Weather const weather) {
	return user.is_encored() or user.last_used_move().is_locked_in_by_move() or is_choice_item(user.item(weather));
}

constexpr auto is_locked_in_to_different_move(any_active_pokemon auto const user, Moves const move, Weather const weather) {
	if (not is_locked_in(user, weather)) {
		return false;
	}
	auto const last_move = user.last_used_move().name();
	return not is_switch(last_move) and last_move != move;
}

constexpr auto is_blocked_due_to_lock_in(any_active_pokemon auto const user, Moves const move, Weather const weather) {
	return !is_regular(move) ?
		user.last_used_move().is_locked_in_by_move() :
		is_locked_in_to_different_move(user, move, weather);
}

template<any_team TeamType>
auto is_legal_selection(TeamType const & user, Move const move, TeamType const & other, Weather const weather, bool const found_selectable_move) {
	BOUNDED_ASSERT(move != Moves::Hit_Self);
	if (switch_decision_required(user)) {
		return is_switch(move.name()) and !would_switch_to_same_pokemon(user.all_pokemon(), move.name());
	}
	auto const is_pass = move == Moves::Pass;
	if (switch_decision_required(other)) {
		return is_pass;
	}
	auto const user_pokemon = user.pokemon();
	if (user_pokemon.last_used_move().moved_this_turn()) {
		return is_pass;
	}
	if (move == Moves::Struggle) {
		return !found_selectable_move;
	}
	auto const other_pokemon = other.pokemon();
	return
		!is_pass and
		!is_blocked_due_to_lock_in(user_pokemon, move.name(), weather) and
		!is_illegal_switch(user, move.name(), other_pokemon, weather) and
		!block1(user_pokemon, move, other_pokemon) and
		!block2(user_pokemon, move.name(), weather) and
		!blocked_by_torment(user_pokemon, move.name());
}

template<any_team TeamType>
constexpr auto legal_selections(TeamType const & user, TeamType const & other, Weather const weather) -> LegalSelections {
	auto result = LegalSelections();
	for (auto const move : all_moves(user.pokemon(), user.size())) {
		bool const found_selectable_move = !containers::is_empty(result);
		if (is_legal_selection(user, move, other, weather, found_selectable_move)) {
			containers::push_back(result, move.name());
		}
	}
	BOUNDED_ASSERT(!containers::is_empty(result));
	return result;
}


constexpr bool is_blocked_by_freeze(any_active_pokemon auto const user, Moves const move) {
	return is_frozen(user.status()) and !thaws_user(move);
}

constexpr bool usable_while_sleeping(Moves const move) {
	switch (move) {
		case Moves::Sleep_Talk:
		case Moves::Snore:
			return true;
		default:
			return false;
	}
}

template<any_active_pokemon UserPokemon>
constexpr bool is_blocked_by_sleep(UserPokemon const user, Moves const move, bool const user_was_asleep) {
	return generation_from<UserPokemon> == Generation::one ? user_was_asleep : usable_while_sleeping(move) != is_sleeping(user.status());
}

constexpr auto is_blocked_due_to_status(any_active_pokemon auto const user, Moves const move, bool const user_was_asleep) {
	return is_blocked_by_freeze(user, move) or is_blocked_by_sleep(user, move, user_was_asleep);
}

auto can_attempt_move_execution(any_active_pokemon auto const user, Move const move, any_active_pokemon auto const other, bool const user_was_asleep) -> bool {
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

auto can_execute_move(any_active_pokemon auto const user, Move const move, Weather const weather, bool const is_recharging) -> bool {
	// TODO: handle is_fully_paralyzed
	constexpr auto is_fully_paralyzed = false;
	auto const switching = is_switch(move.name());
	if (switching or move.name() == Moves::Hit_Self) {
		BOUNDED_ASSERT(!is_recharging or (switching and user.hp().current() == 0_bi));
		return true;
	}
	return !user.flinched() and !block2(user, move.name(), weather) and !is_fully_paralyzed and !is_recharging;
}

} // namespace technicalmachine
