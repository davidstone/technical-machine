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
#include "simple.h"
#include "statfunction.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void blockselection (Team &user, const Pokemon &target, const Weather &weather) {
	user.active->move->selectable = true;
	if (user.active->bide != 0) {
		if (user.active->move->name != BIDE)
			user.active->move->selectable = false;
	}
	else {
		if (user.active->move->pp_max == -1 and user.active->move->name != STRUGGLE) {
			if ((((target.ability == SHADOW_TAG and user.active->ability != SHADOW_TAG) or (target.ability == ARENA_TRAP and grounded (*user.active, weather)) or (target.ability == MAGNET_PULL and istype (*user.active, STEEL)) or user.active->trapped or user.active->partial_trap != 0) and user.active->item != SHED_SHELL)
			or ((user.active.set.front().name == user.active->name and user.active->move->name == SWITCH1)
			// I'm relying on lazy evaluation for teams smaller than 6 to prevent attempting to read a Pokemon that doesn't exist
			 or (user.active.set.size() > 1 and user.active.set [1].name == user.active->name and user.active->move->name == SWITCH2)
			 or (user.active.set.size() > 2 and user.active.set [2].name == user.active->name and user.active->move->name == SWITCH3)
			 or (user.active.set.size() > 3 and user.active.set [3].name == user.active->name and user.active->move->name == SWITCH4)
			 or (user.active.set.size() > 4 and user.active.set [4].name == user.active->name and user.active->move->name == SWITCH5)
			 or (user.active.set.size() > 5 and user.active.set [5].name == user.active->name and user.active->move->name == SWITCH6)))	// Can't switch to yourself
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
		else if ((block1 (*user.active, target))
		 or (block2 (*user.active, weather))
		 or (user.active->torment and 0 != user.active->move->times_used))
			user.active->move->selectable = false;
		else if (0 != user.active->encore or CHOICE_BAND == user.active->item or CHOICE_SCARF == user.active->item or CHOICE_SPECS == user.active->item) {
			for (std::vector<Move>::const_iterator it = user.active->move.set.begin(); it != user.active->move.set.end(); ++it) {
				if (it->name != user.active->move->name and it->times_used != 0)
					user.active->move->selectable = false;
			}
		}
	}
}

void blockexecution (Pokemon &user, const Pokemon &target, const Weather &weather, bool hitself) {
	if (user.hp.stat == 0 or (target.hp.stat == 0 and false))
		user.move->execute = false;
	else if (user.move->pp_max != -1 or user.move->name == STRUGGLE) {
		if (FREEZE == user.status and (FLAME_WHEEL != user.move->name and SACRED_FIRE != user.move->name))
			user.move->execute = false;

		else if (SLEEP == user.status) {		// Currently working here!
			if (user.awaken) {
				user.sleep = 0;
				user.status = NO_STATUS;
			}
			else {
				if (EARLY_BIRD == user.ability)
					user.sleep += 2;
				else
					++user.sleep;
				if (SLEEP_TALK != user.move->name and SNORE != user.move->name)
					user.move->execute = false;
			}
		}

		if (block1 (user, target)
		 or (TRUANT == user.ability and user.loaf))
			user.move->execute = false;

		if (user.move->execute and 0 != user.confused) {
			if (hitself) {
				// fix
				user.move->execute = false;
			}
			else
				--user.confused;
		}
		if (user.move->execute and user.flinch) {
			if (STEADFAST == user.ability)
				user.spe.boost (1);
			user.move->execute = false;
		}
	
		if (block2 (user, weather))
			user.move->execute = false;
	}
}

bool block1 (const Pokemon &user, const Pokemon &target) {		// Things that both block selection and block execution in between sleep and confusion
	if ((0 == user.move->pp)
	 or (0 != user.move->disable)
	 or (0 != user.heal_block and (HEAL_ORDER == user.move->name or MILK_DRINK == user.move->name or MOONLIGHT == user.move->name or MORNING_SUN == user.move->name or RECOVER == user.move->name or REST == user.move->name or ROOST == user.move->name or SLACK_OFF == user.move->name or SOFTBOILED == user.move->name or SWALLOW == user.move->name or SYNTHESIS == user.move->name or WISH == user.move->name))
	  or (imprison (*user.move, target)))
		return true;
	return false;
}

bool imprison (const Move &move, const Pokemon &target) {
	if (target.imprison) {
		for (std::vector<Move>::const_iterator it = target.move.set.begin(); it != target.move.set.end(); ++it) {
			if (move.name == it->name)
				return true;
		}
	}
	return false;
}

bool block2 (const Pokemon &user, const Weather &weather) {		// Things that both block selection and block execution after flinching
	if ((0 != user.taunt and 0 == user.move->basepower)
	 or (0 < weather.gravity and (BOUNCE == user.move->name or FLY == user.move->name or HI_JUMP_KICK == user.move->name or JUMP_KICK == user.move->name or MAGNET_RISE == user.move->name or SPLASH == user.move->name)))
		return true;
	return false;
}

}
