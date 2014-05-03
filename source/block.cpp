// Block selection and execution of moves
// Copyright (C) 2014 David Stone
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
#include "move/moves.hpp"
#include "move/is_switch.hpp"

#include "pokemon/pokemon.hpp"
#include "pokemon/active_pokemon.hpp"

namespace technicalmachine {
namespace {

bool is_legal_selection (ActivePokemon const & user, Move const & move, ActivePokemon const & other, Weather const & weather, bool found_selectable_move);
bool is_blocked_by_bide (ActivePokemon const & user, Moves move);
bool is_not_illegal_switch(ActivePokemon const & user, Moves move, ActivePokemon const & other, Weather const & weather);
bool is_blocked_from_switching(ActivePokemon const & user, Pokemon const & other, Weather const & weather);
bool imprison(Moves move, ActivePokemon const & other);
bool blocked_by_torment(ActivePokemon const & user, Moves move);
bool block1 (ActivePokemon const & user, Move const & move, ActivePokemon const & other);
bool block2(ActivePokemon const & user, Moves move, Weather const & weather);
bool is_blocked_due_to_lock_in(ActivePokemon const & user, Moves move);
bool standard_move_lock_in(ActivePokemon const & user, Moves move);
bool is_locked_in (ActivePokemon const & user);
bool is_locked_in_to_different_move(ActivePokemon const & user, Moves move);
bool is_blocked_due_to_status(ActivePokemon & user, Moves move);
bool is_blocked_by_freeze(Pokemon const & user, Moves move);
bool handle_sleep_counter(ActivePokemon & user, Moves move);

}	// unnamed namespace

LegalSelections::LegalSelections(ActivePokemon const & user, ActivePokemon const & other, Weather const & weather):
	m_species(user) {
	for (auto const & move : user.all_moves()) {
		bool const found_selectable_move = !container.empty();
		if (is_legal_selection(user, move, other, weather, found_selectable_move)) {
			container.emplace_back(move);
		}
	}
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

Moves LegalSelections::operator[](size_t const index) const {
	return container[index];
}

namespace {

bool is_legal_selection (ActivePokemon const & user, Move const & move, ActivePokemon const & other, Weather const & weather, bool const found_selectable_move) {
	return !is_blocked_by_bide (user, move) and
			is_not_illegal_switch (user, move, other, weather) and
			(move != Moves::Struggle or !found_selectable_move) and
			!((block1 (user, move, other)) or (block2 (user, move, weather)) or blocked_by_torment(user, move)) and
			!is_blocked_due_to_lock_in(user, move);
}
}	// namespace

bool can_execute_move (ActivePokemon & user, ActivePokemon const & other, Weather const & weather) {
	Move const & move = user.move();
	assert(!is_switch(move) or !user.recharging());
	
	if (is_switch(move)) {
		return true;
	}
	if (get_hp(user) == 0_bi or (other.is_fainted() and false)) {
		return false;
	}

	bool execute = !(is_blocked_due_to_status (user, move) or
			block1 (user, move, other) or
			user.is_loafing());

	if (execute) {
		// Confusion doesn't block execution, it just changes the move that will
		// execute
		user.handle_confusion();
		if (user.flinched()) {
			if (get_ability(user).boosts_speed_when_flinched ())
				boost(user.stage(), StatNames::SPE, 1_bi);
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

bool is_blocked_by_bide(ActivePokemon const & user, Moves const move) {
	return user.is_locked_in_to_bide() and move == Moves::Bide;
}

bool is_not_illegal_switch(ActivePokemon const & user, Moves const move, ActivePokemon const & other, Weather const & weather) {
	return is_switch(move) ?
		!user.is_switching_to_self(move) and !is_blocked_from_switching (user, other, weather) :
		true;
}

bool is_blocked_from_switching(ActivePokemon const & user, Pokemon const & other, Weather const & weather) {
	bool const block_attempted = get_ability(other).blocks_switching(user, weather) or user.trapped();
	bool const result = block_attempted and !get_item(user).allows_switching();
	return result;
}

bool is_healing(Moves const name) {
	switch (name) {
		case Moves::Heal_Order:
		case Moves::Milk_Drink:
		case Moves::Moonlight:
		case Moves::Morning_Sun:
		case Moves::Recover:
		case Moves::Rest:
		case Moves::Roost:
		case Moves::Slack_Off:
		case Moves::Softboiled:
		case Moves::Swallow:
		case Moves::Synthesis:
		case Moves::Wish:
			return true;
		default:
			return false;
	}
}

bool is_regular_move(Moves const move) {
	return !is_switch(move) and move != Moves::Struggle;
}

// Things that both block selection and block execution in between sleep and confusion
bool block1 (ActivePokemon const & user, Move const & move, ActivePokemon const & other) {
	if (!is_regular_move(move)) {
		return false;
	}
	return (move.pp().is_empty())
			or (user.is_disabled(move))
			or (user.heal_block_is_active() and (is_healing(move)))
			or (imprison (move, other));
}

bool imprison(Moves const move, ActivePokemon const & other) {
	auto const & moves = other.all_moves();
	return other.imprisoned() and std::find(moves.regular_begin(), moves.regular_end(), move) != moves.regular_end();
}

bool is_blocked_by_taunt(Moves const move) {
	return !is_damaging(move);
}

bool is_blocked_by_gravity(Moves const move) {
	switch (move) {
		case Moves::Bounce:
		case Moves::Fly:
		case Moves::Hi_Jump_Kick:
		case Moves::Jump_Kick:
		case Moves::Magnet_Rise:
		case Moves::Splash:
			return true;
		default:
			return false;
	}
}

// Things that both block selection and block execution after flinching
bool block2(ActivePokemon const & user, Moves const move, Weather const & weather) {
	return !is_switch(move) and
			((user.is_taunted() and is_blocked_by_taunt(move)) or
			(weather.gravity() and is_blocked_by_gravity(move)));
}

bool is_blocked_due_to_lock_in(ActivePokemon const & user, Moves const move) {
	return !is_regular_move(move) ?
		user.recharging() :
		standard_move_lock_in(user, move);
}

bool standard_move_lock_in(ActivePokemon const & user, Moves const move) {
	return is_locked_in(user) ? is_locked_in_to_different_move(user, move) : false;
}

bool is_locked_in (ActivePokemon const & user) {
	return user.is_encored() or user.recharging() or get_item(user).is_choice_item();
}

bool is_locked_in_to_different_move(ActivePokemon const & user, Moves const move) {
	return user.moved_since_switch() and !user.was_used_last(move);
}

bool blocked_by_torment(ActivePokemon const & user, Moves const move) {
	return user.tormented() and user.was_used_last(move);
}

bool is_blocked_due_to_status(ActivePokemon & user, Moves const move) {
	return is_blocked_by_freeze(user, move) or handle_sleep_counter(user, move);
}

bool is_blocked_by_freeze(Pokemon const & user, Moves const move) {
	return get_status(user).is_frozen() and !is_usable_while_frozen(move);
}

bool is_blocked_by_sleep(Moves const move) {
	switch (move) {
		case Moves::Sleep_Talk:
		case Moves::Snore:
			return false;
		default:
			return true;
	}
}

bool handle_sleep_counter(ActivePokemon & user, Moves const move) {
	if (!get_status(user).is_sleeping())
		return false;
	user.increase_sleep_counter();
	return is_blocked_by_sleep(move);
}

}	// unnamed namespace
}	// namespace technicalmachine
