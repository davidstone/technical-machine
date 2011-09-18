// Block selection and execution of moves
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "block.h"
#include "move.h"
#include "pokemon.h"
#include "team.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

void blockselection (Team & user, Team const & other, Weather const & weather) {
	user.pokemon->move->selectable = true;
	if (user.bide and user.pokemon->move->name != Move::BIDE)
			user.pokemon->move->selectable = false;
	else if (user.pokemon->move->is_switch()) {
		if (((other.pokemon->ability.blocks_switching (user, weather) or user.ingrain or user.trapped or user.partial_trap) and user.pokemon->item != SHED_SHELL)
				or (user.pokemon.set [user.pokemon->move->name - Move::SWITCH0].name == user.pokemon->name))		// Can't switch to yourself
			user.pokemon->move->selectable = false;
	}
	else if (user.pokemon->move->name == Move::STRUGGLE) {
		for (std::vector<Move>::const_iterator it = user.pokemon->move.set.begin(); it != user.pokemon->move.set.end(); ++it) {
			if (it->pp_max != -1		// Don't let Struggle or Switch keep Struggle from being selectable
					and it->selectable) {
				user.pokemon->move->selectable = false;
				break;
			}
		}
	}
	else if ((block1 (user, other))
			or (block2 (user, weather))
			or (user.torment and user.pokemon->move->times_used != 0))
		user.pokemon->move->selectable = false;
	else if (user.encore or user.recharging or CHOICE_BAND == user.pokemon->item or CHOICE_SCARF == user.pokemon->item or CHOICE_SPECS == user.pokemon->item) {
		for (std::vector<Move>::const_iterator it = user.pokemon->move.set.begin(); it != user.pokemon->move.set.end(); ++it) {
			if (it->name != user.pokemon->move->name and it->times_used != 0) {
				user.pokemon->move->selectable = false;
				break;
			}
		}
	}
}

void blockexecution (Team & user, Team const & other, Weather const & weather) {
	if (!user.pokemon->move->is_switch()) {
		if (user.pokemon->hp.stat == 0 or (other.pokemon->hp.stat == 0 and false))
			user.pokemon->move->execute = false;
		else {
			if (user.pokemon->status == FREEZE and (!user.pokemon->move->is_usable_while_frozen ()))
				user.pokemon->move->execute = false;

			else if (user.pokemon->status == SLEEP) {
				if (user.awaken) {
					user.pokemon->sleep = 0;
					user.pokemon->status = NO_STATUS;
				}
				else {
					if (user.pokemon->ability.name == Ability::EARLY_BIRD)
						user.pokemon->sleep += 2;
					else
						++user.pokemon->sleep;
					if (!user.pokemon->move->is_usable_while_sleeping ())
						user.pokemon->move->execute = false;
				}
			}

			if (block1 (user, other)
					or (user.pokemon->ability.name == Ability::TRUANT and user.loaf))
				user.pokemon->move->execute = false;

			if (user.pokemon->move->execute and user.confused != 0) {
				if (user.hitself) {
					// fix
					user.pokemon->move->execute = false;
				}
				else
					--user.confused;
			}
			if (user.pokemon->move->execute and user.flinch) {
				if (user.pokemon->ability.name == Ability::STEADFAST)
					user.pokemon->spe.boost (1);
				user.pokemon->move->execute = false;
			}
	
			if (block2 (user, weather) or user.fully_paralyzed)
				user.pokemon->move->execute = false;
		
			if (user.recharging) {
				user.pokemon->move->execute = false;
				user.recharging = false;
			}
		}
	}
}

// Things that both block selection and block execution in between sleep and confusion
bool block1 (Team const & user, Team const & other) {
	return (user.pokemon->move->pp == 0)
			or (user.pokemon->move->disable)
			or (user.heal_block and (user.pokemon->move->is_healing ()))
			or (imprison (*user.pokemon->move, other));
}

bool imprison (Move const & move, Team const & other) {
	if (other.imprison) {
		for (std::vector<Move>::const_iterator it = other.pokemon->move.set.begin(); it != other.pokemon->move.set.end(); ++it) {
			if (move.name == it->name)
				return true;
		}
	}
	return false;
}

// Things that both block selection and block execution after flinching
bool block2 (Team const & user, Weather const & weather) {
	return (user.taunt and user.pokemon->move->basepower == 0)
			or (weather.gravity and (user.pokemon->move->is_blocked_by_gravity ()));
}

}
