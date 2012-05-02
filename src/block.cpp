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
#include "move.hpp"
#include "pokemon.hpp"
#include "team.hpp"
#include "type.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {

bool is_legal_selection (Team const & user, Move const & move, Team const & other, Weather const & weather);
bool is_blocked_by_bide (Team const & user, Move const & move);
bool is_not_illegal_switch (Team const & user, Move const & move, Team const & other, Weather const & weather);
bool is_blocked_from_switching (Team const & user, Team const & other, Weather const & weather);
bool not_illegal_struggle (Pokemon const & user, Move const & move);
bool imprison (Move const & move, Team const & other);
bool blocked_by_torment (Team const & user, Move const & move);
bool block1 (Team const & user, Move const & move, Team const & other);
bool block2 (Team const & user, Move const & move, Weather const & weather);
bool is_blocked_due_to_lock_in (Team const & user, Move const & move);
bool standard_move_lock_in (Team const & user, Move const & move);
bool is_locked_in (Team const & user);
bool is_locked_in_to_different_move (Pokemon const & user, Move const & move);
bool is_blocked_due_to_status (Team & user, Move const & move);
bool is_blocked_by_freeze (Pokemon const & user, Move const & move);
bool handle_sleep_counter (Team & user, Move const & move);
void increase_sleep_counter (Team & user);
bool handle_confusion (Team & user);

}	// unnamed namespace

void determine_all_legal_selections (Team & user, Team const & other, Weather const & weather) {
	user.pokemon().move.for_each([& user, & other, & weather](Move & move) {
		move.selectable = is_legal_selection(user, move, other, weather);
	});
}

namespace {

bool is_legal_selection (Team const & user, Move const & move, Team const & other, Weather const & weather) {
	return !is_blocked_by_bide (user, move) and
			is_not_illegal_switch (user, move, other, weather) and
			not_illegal_struggle (user.pokemon(), move) and
			!((block1 (user, move, other)) or (block2 (user, move, weather)) or blocked_by_torment (user, move)) and
			!is_blocked_due_to_lock_in (user, move);
}
}	// unnamed namespace

bool can_execute_move (Team & user, Team const & other, Weather const & weather) {
	Move const & move = user.pokemon().move();
	assert (!move.is_switch() or !user.recharging);
	
	if (move.is_switch())
		return true;
	if (user.pokemon().hp.stat == 0 or (other.pokemon().hp.stat == 0 and false))
		return false;

	bool execute = !(is_blocked_due_to_status (user, move) or
			block1 (user, move, other) or
			user.pokemon().ability.is_loafing (user.loaf));

	if (execute and user.confused)
		execute = handle_confusion (user);

	if (execute) {
		if (user.flinch) {
			if (user.pokemon().ability.boosts_speed_when_flinched ())
				Stat::boost (user.stage [Stat::SPE], 1);
			execute = false;
		}
		else if (block2 (user, move, weather) or user.fully_paralyzed) {
			execute = false;
		}
	}

	if (user.recharging) {
		user.recharging = false;
		execute = false;
	}
	return execute;
}

namespace {

bool is_blocked_by_bide (Team const & user, Move const & move) {
	return user.bide and move.name != Move::BIDE;
}

bool is_not_illegal_switch (Team const & user, Move const & move, Team const & other, Weather const & weather) {
	return move.is_switch() ?
		!is_blocked_from_switching (user, other, weather) and !user.pokemon.is_switching_to_self (move) :
		true;
}

bool is_blocked_from_switching (Team const & user, Team const & other, Weather const & weather) {
	return (other.pokemon().ability.blocks_switching (user, weather) or user.ingrain or user.trapped or user.partial_trap) and !user.pokemon().item.allows_switching();
}

bool not_illegal_struggle (Pokemon const & user, Move const & move) {
	return move.name != Move::STRUGGLE or !user.move.a_regular_move_is_selectable();
}

// Things that both block selection and block execution in between sleep and confusion
bool block1 (Team const & user, Move const & move, Team const & other) {
	return (move.is_out_of_pp())
			or (move.disable)
			or (user.heal_block and (move.is_healing()))
			or (imprison (move, other));
}

bool imprison (Move const & move, Team const & other) {
	return other.imprison and other.pokemon().move.regular_move_exists ([& move](Move const & element) {
		return move.name == element.name;
	});
}

// Things that both block selection and block execution after flinching
bool block2 (Team const & user, Move const & move, Weather const & weather) {
	return !move.is_switch() and
			((user.taunt and move.basepower == 0) or
			(weather.gravity() and move.is_blocked_by_gravity()));
}

bool is_blocked_due_to_lock_in (Team const & user, Move const & move) {
	return move.is_struggle_or_switch() ? user.recharging : standard_move_lock_in (user, move);
}

bool standard_move_lock_in (Team const & user, Move const & move) {
	return is_locked_in (user) ? is_locked_in_to_different_move (user.pokemon(), move) : false;
}

bool is_locked_in (Team const & user) {
	return user.encore or user.recharging or user.pokemon().item.is_choice_item();
}

bool is_locked_in_to_different_move (Pokemon const & user, Move const & move) {
	return user.move.name_of_last_used_move() == move.name;
}

bool blocked_by_torment (Team const & user, Move const & move) {
	return user.torment and move.was_used_last();
}

bool is_blocked_due_to_status (Team & user, Move const & move) {
	return is_blocked_by_freeze(user.pokemon(), move) or handle_sleep_counter(user, move);
}

bool is_blocked_by_freeze (Pokemon const & user, Move const & move) {
	return user.status.name == Status::FREEZE and !move.is_usable_while_frozen();
}

bool handle_sleep_counter (Team & user, Move const & move) {
	if (!user.pokemon().status.is_sleeping())
		return false;
	increase_sleep_counter(user);
	return !move.is_usable_while_sleeping();
}

void increase_sleep_counter (Team & user) {
	if (user.awaken) {
		user.pokemon().sleep = 0;
		user.pokemon().status.clear();
	}
	else {
		user.pokemon().sleep += user.pokemon().ability.wakes_up_early() ? 2 : 1;
	}
}

bool handle_confusion (Team & user) {
	if (user.hitself) {
		// TODO: make user hit self in confusion
		return false;
	}
	else {
		--user.confused;
		return true;
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
