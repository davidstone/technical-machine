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
bool is_blocked_from_switching (Team const & user, Team const & other, Weather const & weather);
bool imprison (Move const & move, Team const & other);
bool blocked_by_torment (Team const & user);
bool block1 (Team const & user, Team const & other);
bool block2 (Team const & user, Weather const & weather);
bool is_locked_in (Team const & user);
void handle_sleep_counter (Team & user);
void handle_confusion (Team & user);

}	// unnamed namespace

void block_selection (Team & user, Team const & other, Weather const & weather) {
	user.pokemon().move().selectable = true;
	if (is_blocked_by_bide (user))
		user.pokemon().move().selectable = false;
	else if (user.pokemon().move().is_switch()) {
		if (is_blocked_from_switching (user, other, weather) or user.is_switching_to_self ())
			user.pokemon().move().selectable = false;
	}
	else if (user.pokemon().move().name == Move::STRUGGLE) {
		for (auto it = user.pokemon().move.set.cbegin(); it->name != Move::STRUGGLE; ++it) {
			if (it->selectable) {
				// Struggle is only selectable if no other move is selectable
				user.pokemon().move().selectable = false;
				break;
			}
		}
	}
	else if ((block1 (user, other)) or (block2 (user, weather)) or blocked_by_torment (user))
		user.pokemon().move().selectable = false;
	else if (is_locked_in (user)) {
		for (auto it = user.pokemon().move.set.cbegin(); it->name != Move::STRUGGLE; ++it) {
			if (it->name != user.pokemon().move().name and it->times_used != 0) {
				user.pokemon().move().selectable = false;
				break;
			}
		}
	}
}

void block_execution (Team & user, Team const & other, Weather const & weather) {
	if (user.pokemon().move().is_switch())
		return;
	if (user.pokemon().hp.stat == 0 or (other.pokemon().hp.stat == 0 and false))
		user.pokemon().move().execute = false;
	else {
		if (user.pokemon().status.name == Status::FREEZE and !user.pokemon().move().is_usable_while_frozen ())
			user.pokemon().move().execute = false;
		else if (user.pokemon().status.is_sleeping ())
			handle_sleep_counter (user);

		if (block1 (user, other) or (user.pokemon().ability.name == Ability::TRUANT and user.loaf))
			user.pokemon().move().execute = false;

		if (user.pokemon().move().execute and user.confused)
			handle_confusion (user);

		if (user.pokemon().move().execute) {
			if (user.flinch) {
				if (user.pokemon().ability.name == Ability::STEADFAST)
					Stat::boost (user.stage [Stat::SPE], 1);
				user.pokemon().move().execute = false;
			}
			else if (block2 (user, weather) or user.fully_paralyzed) {
				user.pokemon().move().execute = false;
			}
		}
	
		if (user.recharging) {
			user.pokemon().move().execute = false;
			user.recharging = false;
		}
	}
}

namespace {

bool is_blocked_by_bide (Team const & user) {
	return user.bide and user.pokemon().move().name != Move::BIDE;
}

bool is_blocked_from_switching (Team const & user, Team const & other, Weather const & weather) {
	return (other.pokemon().ability.blocks_switching (user, weather) or user.ingrain or user.trapped or user.partial_trap) and user.pokemon().item.name != Item::SHED_SHELL;
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
	return (user.taunt and user.pokemon().move().basepower == 0)
			or (weather.gravity and user.pokemon().move().is_blocked_by_gravity ());
}

bool is_locked_in (Team const & user) {
	return user.encore or user.recharging or user.pokemon().item.is_choice_item ();
}

bool blocked_by_torment (Team const & user) {
	return user.torment and user.pokemon().move().times_used != 0;
}

void handle_sleep_counter (Team & user) {
	if (user.awaken) {
		user.pokemon().sleep = 0;
		user.pokemon().status.clear ();
	}
	else {
		if (user.pokemon().ability.name == Ability::EARLY_BIRD)
			user.pokemon().sleep += 2;
		else
			++user.pokemon().sleep;
		if (!user.pokemon().move().is_usable_while_sleeping ())
			user.pokemon().move().execute = false;
	}
}

void handle_confusion (Team & user) {
	if (user.hitself) {
		// TODO: make user hit self in confusion
		user.pokemon().move().execute = false;
	}
	else {
		--user.confused;
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
