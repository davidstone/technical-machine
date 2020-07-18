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

#include <tm/ability.hpp>
#include <tm/weather.hpp>

#include <tm/move/category.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

template<Generation>
struct ActivePokemon;
template<Generation>
struct MutableActivePokemon;
template<Generation>
struct PokemonCollection;
template<Generation>
struct Team;

template<Generation generation>
constexpr auto would_switch_to_same_pokemon(PokemonCollection<generation> const & collection, Moves const move) {
	return to_replacement(move) == collection.index();
}

template<Generation generation>
constexpr auto is_blocked_from_switching(ActivePokemon<generation> const user, ActivePokemon<generation> const other, Weather const weather) {
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

template<Generation generation>
constexpr auto imprison(Moves const move, ActivePokemon<generation> const other) {
	return other.used_imprison() and containers::any_equal(other.regular_moves(), move);
}

// Things that both block selection and block execution in between sleep and confusion
template<Generation generation>
constexpr auto block1(ActivePokemon<generation> const user, Move const move, ActivePokemon<generation> const other) {
	if (!is_regular(move.name())) {
		return false;
	}
	return
		(move.pp().is_empty()) or
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
template<Generation generation>
constexpr auto block2(ActivePokemon<generation> const user, Moves const move, Weather const weather) {
	return !is_switch(move) and (
		(user.is_taunted() and is_blocked_by_taunt(move)) or
		(weather.gravity() and is_blocked_by_gravity(move))
	);
}

template<Generation generation>
constexpr auto blocked_by_torment(ActivePokemon<generation> const user, Moves const move) {
	return user.is_tormented() and user.last_used_move().name() == move and not is_switch(move) and move != Moves::Struggle;
}

template<Generation generation>
constexpr auto is_locked_in(ActivePokemon<generation> const user, Weather const weather) {
	return user.is_encored() or user.is_locked_in_by_move() or is_choice_item(user.item(weather));
}

template<Generation generation>
constexpr auto is_locked_in_to_different_move(ActivePokemon<generation> const user, Moves const move, Weather const weather) {
	if (not is_locked_in(user, weather)) {
		return false;
	}
	auto const last_move = user.last_used_move().name();
	return not is_switch(last_move) and last_move != move;
}

template<Generation generation>
constexpr auto is_blocked_due_to_lock_in(ActivePokemon<generation> const user, Moves const move, Weather const weather) {
	return !is_regular(move) ?
		user.is_locked_in_by_move() :
		is_locked_in_to_different_move(user, move, weather);
}

template<Generation generation>
constexpr auto is_legal_selection(Team<generation> const & user, Move const move, Team<generation> const & other, Weather const weather, bool const found_selectable_move) {
	BOUNDED_ASSERT(move != Moves::Hit_Self);
	auto const pokemon = user.pokemon();
	if (user.size() > 1_bi and pokemon.switch_decision_required()) {
		return is_switch(move.name()) and !would_switch_to_same_pokemon(user.all_pokemon(), move.name());
	}
	auto const other_pokemon = other.pokemon();
	auto const is_pass = move == Moves::Pass;
	if (other.size() > 1_bi and other_pokemon.switch_decision_required()) {
		return is_pass;
	}
	if (pokemon.moved()) {
		return is_pass;
	}
	if (move == Moves::Struggle) {
		return !found_selectable_move;
	}
	return
		!is_pass and
		!is_blocked_due_to_lock_in(pokemon, move.name(), weather) and
		!is_illegal_switch(user, move.name(), other_pokemon, weather) and
		!block1(pokemon, move, other_pokemon) and
		!block2(pokemon, move.name(), weather) and
		!blocked_by_torment(pokemon, move.name());
}

template<Generation generation>
constexpr auto legal_selections(Team<generation> const & user, Team<generation> const & other, Weather const weather) -> StaticVectorMove {
	auto result = StaticVectorMove{};
	for (auto const move : all_moves(user.pokemon(), user.size())) {
		bool const found_selectable_move = !empty(result);
		if (is_legal_selection(user, move, other, weather, found_selectable_move)) {
			containers::push_back(result, move.name());
		}
	}
	BOUNDED_ASSERT(!empty(result));
	return result;
}


template<Generation generation>
constexpr bool is_blocked_by_freeze(ActivePokemon<generation> const user, Moves const move) {
	return is_frozen(user.status()) and !is_usable_while_frozen(move);
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

template<Generation generation>
constexpr bool is_blocked_by_sleep(ActivePokemon<generation> const user, Moves const move, bool const user_was_asleep) {
	return generation == Generation::one ? user_was_asleep : usable_while_sleeping(move) != is_sleeping(user.status());
}

template<Generation generation>
constexpr auto is_blocked_due_to_status(ActivePokemon<generation> const user, Moves const move, bool const user_was_asleep) {
	return is_blocked_by_freeze(user, move) or is_blocked_by_sleep(user, move, user_was_asleep);
}

template<Generation generation>
constexpr auto can_attempt_move_execution(ActivePokemon<generation> user, Move const move, ActivePokemon<generation> const other, bool const user_was_asleep) -> bool {
	if (is_switch(move.name())) {
		return true;
	}
	if (user.hp() == 0_bi) {
		return false;
	}

	auto const blocked_due_to_status = is_blocked_due_to_status(user, move.name(), user_was_asleep);
	if (blocked_due_to_status or block1(user, move, other) or user.is_loafing()) {
		return false;
	}
	return true;
}

template<Generation generation>
constexpr auto can_execute_move(ActivePokemon<generation> user, Move const move, Weather const weather, bool const is_recharging) -> bool {
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
