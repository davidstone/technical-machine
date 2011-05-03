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
#include "statfunction.h"
#include "team.h"
#include "typefunction.h"
#include "weather.h"

namespace technicalmachine {

void blockselection (Team &user, const Team &target, const Weather &weather) {
	user.active->move->selectable = true;
	if (user.bide != 0) {
		if (user.active->move->name != BIDE)
			user.active->move->selectable = false;
	}
	else {
		if (SWITCH0 <= user.active->move->name and user.active->move->name <= SWITCH5) {
			if ((((target.active->ability == SHADOW_TAG and user.active->ability != SHADOW_TAG) or (target.active->ability == ARENA_TRAP and grounded (user, weather)) or (target.active->ability == MAGNET_PULL and istype (user, STEEL)) or user.trapped or user.partial_trap != 0) and user.active->item != SHED_SHELL)
					or (user.active.set [user.active->move->name - SWITCH0].name == user.active->name and user.active.set.size() > 1))		// Can't switch to yourself
				user.active->move->selectable = false;
		}
		else if (user.active->move->name == STRUGGLE) {
			for (std::vector<Move>::const_iterator it = user.active->move.set.begin(); it != user.active->move.set.end(); ++it) {
				if (it->pp_max != -1		// Don't let Struggle or Switch keep Struggle from being selectable
						and it->selectable) {
					user.active->move->selectable = false;
					break;
				}
			}
		}
		else if ((block1 (user, target))
				or (block2 (user, weather))
				or (user.torment and 0 != user.active->move->times_used))
			user.active->move->selectable = false;
		else if (0 != user.encore or CHOICE_BAND == user.active->item or CHOICE_SCARF == user.active->item or CHOICE_SPECS == user.active->item) {
			for (std::vector<Move>::const_iterator it = user.active->move.set.begin(); it != user.active->move.set.end(); ++it) {
				if (it->name != user.active->move->name and it->times_used != 0)
					user.active->move->selectable = false;
			}
		}
	}
}

void blockexecution (Team &user, const Team &target, const Weather &weather) {
	if (user.active->hp.stat == 0 or (target.active->hp.stat == 0 and false))
		user.active->move->execute = false;
	else if (user.active->move->pp_max != -1 or user.active->move->name == STRUGGLE) {
		if (user.active->status == FREEZE and (user.active->move->name != FLAME_WHEEL and user.active->move->name != SACRED_FIRE))
			user.active->move->execute = false;

		else if (user.active->status == SLEEP) {
			if (user.awaken) {
				user.active->sleep = 0;
				user.active->status = NO_STATUS;
			}
			else {
				if (user.active->ability == EARLY_BIRD)
					user.active->sleep += 2;
				else
					++user.active->sleep;
				if (user.active->move->name != SLEEP_TALK and user.active->move->name != SNORE)
					user.active->move->execute = false;
			}
		}

		if (block1 (user, target)
		 or (user.active->ability == TRUANT and user.loaf))
			user.active->move->execute = false;

		if (user.active->move->execute and user.confused != 0) {
			if (user.hitself) {
				// fix
				user.active->move->execute = false;
			}
			else
				--user.confused;
		}
		if (user.active->move->execute and user.flinch) {
			if (user.active->ability == STEADFAST)
				user.active->spe.boost (1);
			user.active->move->execute = false;
		}
	
		if (block2 (user, weather))
			user.active->move->execute = false;
	}
}

bool block1 (const Team &user, const Team &target) {		// Things that both block selection and block execution in between sleep and confusion
	if ((0 == user.active->move->pp)
	 or (0 != user.active->move->disable)
	 or (0 != user.heal_block and (HEAL_ORDER == user.active->move->name or MILK_DRINK == user.active->move->name or MOONLIGHT == user.active->move->name or MORNING_SUN == user.active->move->name or RECOVER == user.active->move->name or REST == user.active->move->name or ROOST == user.active->move->name or SLACK_OFF == user.active->move->name or SOFTBOILED == user.active->move->name or SWALLOW == user.active->move->name or SYNTHESIS == user.active->move->name or WISH == user.active->move->name))
	  or (imprison (*user.active->move, target)))
		return true;
	return false;
}

bool imprison (const Move &move, const Team &target) {
	if (target.imprison) {
		for (std::vector<Move>::const_iterator it = target.active->move.set.begin(); it != target.active->move.set.end(); ++it) {
			if (move.name == it->name)
				return true;
		}
	}
	return false;
}

bool block2 (const Team &user, const Weather &weather) {		// Things that both block selection and block execution after flinching
	if ((0 != user.taunt and 0 == user.active->move->basepower)
	 or (0 < weather.gravity and (BOUNCE == user.active->move->name or FLY == user.active->move->name or HI_JUMP_KICK == user.active->move->name or JUMP_KICK == user.active->move->name or MAGNET_RISE == user.active->move->name or SPLASH == user.active->move->name)))
		return true;
	return false;
}

}
