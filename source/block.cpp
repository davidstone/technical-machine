// Block selection and execution of moves
// Copyright (C) 2012 David Stone
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

#include "block.hpp"

#include <cassert>

#include "ability.hpp"
#include "weather.hpp"

#include "move/move.hpp"

#include "pokemon/pokemon.hpp"
#include "pokemon/active_pokemon.hpp"

namespace technicalmachine {
namespace {

bool is_legal_selection (ActivePokemon const & user, Move const & move, ActivePokemon const & other, Weather const & weather, bool found_selectable_move);
bool is_blocked_by_bide (ActivePokemon const & user, Move const & move);
bool is_not_illegal_switch (ActivePokemon const & user, Move const & move, ActivePokemon const & other, Weather const & weather);
bool is_blocked_from_switching (ActivePokemon const & user, ActivePokemon const & other, Weather const & weather);
bool imprison (Move const & move, ActivePokemon const & other);
bool blocked_by_torment (ActivePokemon const & user, Move const & move);
bool block1 (ActivePokemon const & user, Move const & move, ActivePokemon const & other);
bool block2 (ActivePokemon const & user, Move const & move, Weather const & weather);
bool is_blocked_due_to_lock_in (ActivePokemon const & user, Move const & move);
bool standard_move_lock_in (ActivePokemon const & user, Move const & move);
bool is_locked_in (ActivePokemon const & user);
bool is_locked_in_by_choice_item(ActivePokemon const & user);
bool is_locked_in_to_different_move (Pokemon const & user, Move const & move);
bool is_blocked_due_to_status (ActivePokemon & user, Move const & move);
bool is_blocked_by_freeze (Pokemon const & user, Move const & move);
bool handle_sleep_counter (ActivePokemon & user, Move const & move);

}	// unnamed namespace

LegalSelections::LegalSelections(ActivePokemon const & user, ActivePokemon const & other, Weather const & weather):
	m_species(user.name()) {
	user.all_moves().for_each([&](Move const & move) {
		bool const found_selectable_move = !container.empty();
		if (is_legal_selection(user, move, other, weather, found_selectable_move)) {
			container.emplace_back(&move);
		}
	});
	assert(!container.empty());
}

Species LegalSelections::species() const {
	return m_species;
}

LegalSelections::const_iterator LegalSelections::begin() const {
	return container.begin();
}

LegalSelections::const_iterator LegalSelections::end() const {
	return container.end();
}

size_t LegalSelections::size() const {
	return container.size();
}

Move const * const & LegalSelections::operator[] (size_t const index) const {
	return container[index];
}

namespace {

bool is_legal_selection (ActivePokemon const & user, Move const & move, ActivePokemon const & other, Weather const & weather, bool const found_selectable_move) {
	return !is_blocked_by_bide (user, move) and
			is_not_illegal_switch (user, move, other, weather) and
			(!move.is_struggle() or !found_selectable_move) and
			!((block1 (user, move, other)) or (block2 (user, move, weather)) or blocked_by_torment (user, move)) and
			!is_blocked_due_to_lock_in (user, move);
}
}	// unnamed namespace

bool can_execute_move (ActivePokemon & user, ActivePokemon const & other, Weather const & weather) {
	Move const & move = user.move();
	assert(!move.is_switch() or !user.recharging());
	
	if (move.is_switch())
		return true;
	if (user.stat(Stat::HP).stat == 0 or (other.is_fainted() and false))
		return false;

	bool execute = !(is_blocked_due_to_status (user, move) or
			block1 (user, move, other) or
			user.is_loafing());

	if (execute) {
		// Confusion doesn't block execution, it just changes the move that will
		// execute
		user.handle_confusion();
		if (user.flinched()) {
			if (user.ability().boosts_speed_when_flinched ())
				user.stat_boost(Stat::SPE, 1);
			execute = false;
		}
		else if (block2 (user, move, weather) or user.is_fully_paralyzed()) {
			execute = false;
		}
	}

	if (user.recharge())
		execute = false;
	return execute;
}

namespace {

bool is_blocked_by_bide (ActivePokemon const & user, Move const & move) {
	return user.is_locked_in_to_bide() and move.is_bide();
}

bool is_not_illegal_switch (ActivePokemon const & user, Move const & move, ActivePokemon const & other, Weather const & weather) {
	return move.is_switch() ?
		!user.is_switching_to_self (move) and !is_blocked_from_switching (user, other, weather) :
		true;
}

bool is_blocked_from_switching (ActivePokemon const & user, ActivePokemon const & other, Weather const & weather) {
	bool const block_attempted = other.ability().blocks_switching(user, weather) or user.trapped();
	bool const result = block_attempted and !user.item().allows_switching();
	return result;
}

// Things that both block selection and block execution in between sleep and confusion
bool block1 (ActivePokemon const & user, Move const & move, ActivePokemon const & other) {
	return (move.pp.is_empty())
			or (move.disable)
			or (user.heal_block_is_active() and (move.is_healing()))
			or (imprison (move, other));
}

bool imprison (Move const & move, ActivePokemon const & other) {
	return other.imprisoned() and other.all_moves().regular_move_exists ([& move](Move const & element) {
		return move.name() == element.name();
	});
}

// Things that both block selection and block execution after flinching
bool block2 (ActivePokemon const & user, Move const & move, Weather const & weather) {
	return !move.is_switch() and
			((user.is_taunted() and move.is_blocked_by_taunt()) or
			(weather.gravity() and move.is_blocked_by_gravity()));
}

bool is_blocked_due_to_lock_in (ActivePokemon const & user, Move const & move) {
	return move.is_struggle_or_switch() ? user.recharging() : standard_move_lock_in (user, move);
}

bool standard_move_lock_in (ActivePokemon const & user, Move const & move) {
	return is_locked_in (user) ? is_locked_in_to_different_move (user, move) : false;
}

bool is_locked_in (ActivePokemon const & user) {
	return user.is_encored() or user.recharging() or is_locked_in_by_choice_item(user);
}

bool is_locked_in_by_choice_item(ActivePokemon const & user) {
	return user.item().is_choice_item() and user.moved_since_switch();
}

bool is_locked_in_to_different_move (Pokemon const & user, Move const & move) {
	return user.move.name_of_last_used_move() != move.name();
}

bool blocked_by_torment (ActivePokemon const & user, Move const & move) {
	return user.tormented() and move.was_used_last();
}

bool is_blocked_due_to_status (ActivePokemon & user, Move const & move) {
	return is_blocked_by_freeze(user, move) or handle_sleep_counter(user, move);
}

bool is_blocked_by_freeze (Pokemon const & user, Move const & move) {
	return user.status().is_frozen() and !move.is_usable_while_frozen();
}

bool handle_sleep_counter (ActivePokemon & user, Move const & move) {
	if (!user.status().is_sleeping())
		return false;
	user.increase_sleep_counter();
	return !move.is_usable_while_sleeping();
}

}	// unnamed namespace
}	// namespace technicalmachine
