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
	user.pokemon->move->selectable = true;
	if (user.bide != 0 and user.pokemon->move->name != BIDE)
			user.pokemon->move->selectable = false;
	else if (SWITCH0 <= user.pokemon->move->name and user.pokemon->move->name <= SWITCH5) {
		if ((((target.pokemon->ability == SHADOW_TAG and user.pokemon->ability != SHADOW_TAG) or (target.pokemon->ability == ARENA_TRAP and grounded (user, weather)) or (target.pokemon->ability == MAGNET_PULL and istype (user, STEEL)) or user.trapped or user.partial_trap != 0) and user.pokemon->item != SHED_SHELL)
				or (user.pokemon.set [user.pokemon->move->name - SWITCH0].name == user.pokemon->name and user.pokemon.set.size() > 1))		// Can't switch to yourself
			user.pokemon->move->selectable = false;
	}
	else if (user.pokemon->move->name == STRUGGLE) {
		for (std::vector<Move>::const_iterator it = user.pokemon->move.set.begin(); it != user.pokemon->move.set.end(); ++it) {
			if (it->pp_max != -1		// Don't let Struggle or Switch keep Struggle from being selectable
					and it->selectable) {
				user.pokemon->move->selectable = false;
				break;
			}
		}
	}
	else if ((block1 (user, target))
			or (block2 (user, weather))
			or (user.torment and user.pokemon->move->times_used != 0))
		user.pokemon->move->selectable = false;
	else if (user.encore != 0 or user.recharging or CHOICE_BAND == user.pokemon->item or CHOICE_SCARF == user.pokemon->item or CHOICE_SPECS == user.pokemon->item) {
		for (std::vector<Move>::const_iterator it = user.pokemon->move.set.begin(); it != user.pokemon->move.set.end(); ++it) {
			if (it->name != user.pokemon->move->name and it->times_used != 0) {
				user.pokemon->move->selectable = false;
				break;
			}
		}
	}
}

void blockexecution (Team &user, const Team &target, const Weather &weather) {
	if (user.pokemon->hp.stat == 0 or (target.pokemon->hp.stat == 0 and false))
		user.pokemon->move->execute = false;
	else if (SWITCH0 <= user.pokemon->move->name and user.pokemon->move->name <= SWITCH5) {
		if (user.pokemon->status == FREEZE and (user.pokemon->move->name != FLAME_WHEEL and user.pokemon->move->name != SACRED_FIRE))
			user.pokemon->move->execute = false;

		else if (user.pokemon->status == SLEEP) {
			if (user.awaken) {
				user.pokemon->sleep = 0;
				user.pokemon->status = NO_STATUS;
			}
			else {
				if (user.pokemon->ability == EARLY_BIRD)
					user.pokemon->sleep += 2;
				else
					++user.pokemon->sleep;
				if (user.pokemon->move->name != SLEEP_TALK and user.pokemon->move->name != SNORE)
					user.pokemon->move->execute = false;
			}
		}

		if (block1 (user, target)
		 or (user.pokemon->ability == TRUANT and user.loaf))
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
			if (user.pokemon->ability == STEADFAST)
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

bool block1 (const Team &user, const Team &target) {		// Things that both block selection and block execution in between sleep and confusion
	if ((0 == user.pokemon->move->pp)
	 or (0 != user.pokemon->move->disable)
	 or (0 != user.heal_block and (HEAL_ORDER == user.pokemon->move->name or MILK_DRINK == user.pokemon->move->name or MOONLIGHT == user.pokemon->move->name or MORNING_SUN == user.pokemon->move->name or RECOVER == user.pokemon->move->name or REST == user.pokemon->move->name or ROOST == user.pokemon->move->name or SLACK_OFF == user.pokemon->move->name or SOFTBOILED == user.pokemon->move->name or SWALLOW == user.pokemon->move->name or SYNTHESIS == user.pokemon->move->name or WISH == user.pokemon->move->name))
	  or (imprison (*user.pokemon->move, target)))
		return true;
	return false;
}

bool imprison (const Move &move, const Team &target) {
	if (target.imprison) {
		for (std::vector<Move>::const_iterator it = target.pokemon->move.set.begin(); it != target.pokemon->move.set.end(); ++it) {
			if (move.name == it->name)
				return true;
		}
	}
	return false;
}

bool block2 (const Team &user, const Weather &weather) {		// Things that both block selection and block execution after flinching
	if ((0 != user.taunt and 0 == user.pokemon->move->basepower)
	 or (0 < weather.gravity and (BOUNCE == user.pokemon->move->name or FLY == user.pokemon->move->name or HI_JUMP_KICK == user.pokemon->move->name or JUMP_KICK == user.pokemon->move->name or MAGNET_RISE == user.pokemon->move->name or SPLASH == user.pokemon->move->name)))
		return true;
	return false;
}

}
