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
#include "ability.hpp"
#include "move.hpp"
#include "pokemon.hpp"
#include "team.hpp"
#include "type.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {

bool is_blocked_by_bide (Team const & user);
bool is_not_illegal_switch (Team const & user, Team const & other, Weather const & weather);
bool is_blocked_from_switching (Team const & user, Team const & other, Weather const & weather);
bool not_illegal_struggle (Pokemon const & user);
bool imprison (Move const & move, Team const & other);
bool blocked_by_torment (Team const & user);
bool block1 (Team const & user, Team const & other);
bool block2 (Team const & user, Weather const & weather);
bool is_blocked_due_to_lock_in (Team const & user);
bool is_locked_in (Team const & user);
void increase_sleep_counter (Team & user);
bool handle_sleep_counter (Pokemon const & user);
bool handle_confusion (Team & user);

}	// unnamed namespace

bool is_legal_selection (Team const & user, Team const & other, Weather const & weather) {
	return !is_blocked_by_bide (user) and
			is_not_illegal_switch (user, other, weather) and
			not_illegal_struggle (user.pokemon()) and
			!((block1 (user, other)) or (block2 (user, weather)) or blocked_by_torment (user)) and
			!is_blocked_due_to_lock_in (user);
}

bool can_execute_move (Team & user, Team const & other, Weather const & weather) {
	if (user.pokemon().move().is_switch())
		return true;
	if (user.pokemon().hp.stat == 0 or (other.pokemon().hp.stat == 0 and false))
		return false;

	bool execute = true;
	if (user.pokemon().status.name == Status::FREEZE and !user.pokemon().move().is_usable_while_frozen ())
		execute = false;
	else if (user.pokemon().status.is_sleeping ()) {
		increase_sleep_counter (user);
		execute = handle_sleep_counter (user.pokemon());
	}

	if (block1 (user, other) or (user.pokemon().ability.name == Ability::TRUANT and user.loaf))
		execute = false;

	if (execute and user.confused)
		execute = handle_confusion (user);

	if (execute) {
		if (user.flinch) {
			if (user.pokemon().ability.name == Ability::STEADFAST)
				Stat::boost (user.stage [Stat::SPE], 1);
			execute = false;
		}
		else if (block2 (user, weather) or user.fully_paralyzed) {
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

bool is_blocked_by_bide (Team const & user) {
	return user.bide and user.pokemon().move().name != Move::BIDE;
}

bool is_not_illegal_switch (Team const & user, Team const & other, Weather const & weather) {
	if (user.pokemon().move().is_switch())
		return !is_blocked_from_switching (user, other, weather) and !user.is_switching_to_self ();
	return true;
}

bool is_blocked_from_switching (Team const & user, Team const & other, Weather const & weather) {
	return (other.pokemon().ability.blocks_switching (user, weather) or user.ingrain or user.trapped or user.partial_trap) and user.pokemon().item.name != Item::SHED_SHELL;
}

bool not_illegal_struggle (Pokemon const & user) {
	if (user.move().name == Move::STRUGGLE) {
		for (auto it = user.move.set.cbegin(); it->name != Move::STRUGGLE; ++it) {
			// Struggle is only selectable if no other move is selectable
			if (it->selectable)
				return false;
		}
	}
	return true;
}

// Things that both block selection and block execution in between sleep and confusion
bool block1 (Team const & user, Team const & other) {
	return (user.pokemon().move().pp == 0)
			or (user.pokemon().move().disable)
			or (user.heal_block and (user.pokemon().move().is_healing ()))
			or (imprison (user.pokemon().move(), other));
}

bool imprison (Move const & move, Team const & other) {
	if (other.imprison) {
		for (auto it = other.pokemon().move.set.cbegin (); it->name != Move::STRUGGLE; ++it) {
			if (move.name == it->name)
				return true;
		}
	}
	return false;
}

// Things that both block selection and block execution after flinching
bool block2 (Team const & user, Weather const & weather) {
	return !user.pokemon().move().is_switch () and
			((user.taunt and user.pokemon().move().basepower == 0) or
			(weather.gravity and user.pokemon().move().is_blocked_by_gravity ()));
}

bool is_blocked_due_to_lock_in (Team const & user) {
	if (user.pokemon().move().is_struggle_or_switch() and !user.recharging)
		return false;
	if (is_locked_in (user)) {
		for (auto it = user.pokemon().move.set.cbegin(); it->name != Move::STRUGGLE; ++it) {
			if (it->name != user.pokemon().move().name and it->times_used != 0)
				return true;
		}
	}
	return false;
}

bool is_locked_in (Team const & user) {
	return user.encore or user.recharging or user.pokemon().item.is_choice_item ();
}

bool blocked_by_torment (Team const & user) {
	return user.torment and user.pokemon().move().times_used != 0;
}

void increase_sleep_counter (Team & user) {
	if (user.awaken) {
		user.pokemon().sleep = 0;
		user.pokemon().status.clear ();
	}
	else {
		if (user.pokemon().ability.name == Ability::EARLY_BIRD)
			user.pokemon().sleep += 2;
		else
			++user.pokemon().sleep;
	}
}

bool handle_sleep_counter (Pokemon const & user) {
	return !user.sleep or user.move().is_usable_while_sleeping ();
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
