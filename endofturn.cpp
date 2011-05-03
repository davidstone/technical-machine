// End of turn effects
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "endofturn.h"
#include "ability.h"
#include "damage.h"
#include "heal.h"
#include "move.h"
#include "pokemon.h"
#include "statfunction.h"
#include "statusfunction.h"
#include "team.h"
#include "typefunction.h"
#include "weather.h"

namespace technicalmachine {

void endofturn (Team &first, Team &last, Weather &weather) {
	endofturn0 (first);
	endofturn0 (last);
	endofturn1 (first);
	endofturn1 (last);
	endofturn2 (first);
	endofturn2 (last);
	decrement (weather.hail);
	decrement (weather.sun);
	decrement (weather.sand);
	decrement (weather.rain);
	if (first.active->ability != AIR_LOCK and last.active->ability != AIR_LOCK) {
		endofturn3 (first, weather);
		endofturn3 (last, weather);
	}
	decrement (weather.gravity);
	endofturn5 (first, *last.active, weather);
	endofturn5 (last, *first.active, weather);
	endofturn6 (first, weather);
	endofturn6 (last, weather);
	endofturn7 (first);
	endofturn7 (last);
	decrement (weather.trick_room);
}

void endofturn0 (Team &team) {
	team.damaged = false;
	team.flinch = false;
	team.moved = false;
	team.mf = false;
	if (team.loaf)
		team.loaf = false;
	else
		team.loaf = true;
}

void endofturn1 (Team &team) {
	decrement (team.light_screen);
	decrement (team.lucky_chant);
	decrement (team.mist);
	decrement (team.reflect);
	decrement (team.safeguard);
	decrement (team.tailwind);
}

void endofturn2 (Team &team) {
	if (1 == team.wish)
		heal (*team.active, 2);
	decrement (team.wish);
}

void endofturn3 (Team &team, const Weather &weather) {
	if (weather.hail != 0 and !istype (team, ICE))
		heal (*team.active, -16);
	if (weather.sand != 0 and !(istype (team, GROUND) or istype (team, ROCK) or istype (team, STEEL)))
		heal (*team.active, -16);
	if (DRY_SKIN == team.active->ability) {
		if (0 != weather.rain)
			heal (*team.active, 8);
		else if (0 != weather.sun)
			heal (*team.active, -8);
	}
	else if (HYDRATION == team.active->ability and 0 != weather.rain)
		team.active->status = NO_STATUS;
	else if ((ICE_BODY == team.active->ability and 0 != weather.hail) or (RAIN_DISH == team.active->ability and 0 != weather.rain))
		heal (*team.active, 16);
}

void endofturn5 (Team &team, Pokemon &foe, Weather &weather) {
	if (team.ingrain)
		heal (*team.active, 16);
	if (team.aqua_ring)
		heal (*team.active, 16);
	if (SPEED_BOOST == team.active->ability)
		team.active->spe.boost (1);
	else if (team.shed_skin)
		team.active->status = NO_STATUS;
	if (LEFTOVERS == team.active->item)
		heal (*team.active, 16);
	else if (BLACK_SLUDGE == team.active->item) {
		if (istype (team, POISON))
			heal (*team.active, 16);
		else
			heal (*team.active, -16);
	}
	if (team.leech_seed) {
		int n = team.active->hp.stat;
		heal (*team.active, -8);
		if (foe.hp.stat != 0) {
			if (LIQUID_OOZE == team.active->ability) {
				foe.hp.stat -= n - team.active->hp.stat;
				if (foe.hp.stat < 0)
					foe.hp.stat = 0;
			}
			else {
				foe.hp.stat += n - team.active->hp.stat;
				if (foe.hp.stat > foe.hp.max)
					foe.hp.stat = foe.hp.max;
			}
		}
	}
	if (BURN == team.active->status) {
		if (HEATPROOF == team.active->ability)
			heal (*team.active, -16);
		else
			heal (*team.active, -8);
	}
	else if (POISON_NORMAL == team.active->status or POISON_TOXIC == team.active->status) {
		if (POISON_HEAL == team.active->ability)
			heal (*team.active, 8);
		else if (POISON_NORMAL == team.active->status)
			heal (*team.active, -8);
		else {
			if (15 != team.toxic)
				++team.toxic;
			heal (*team.active, -16, team.toxic);
		}
	}
	else if (team.nightmare)
		heal (*team.active, -4);
	if (FLAME_ORB == team.active->item)
		burn (team, team, weather);
	else if (TOXIC_ORB == team.active->item and NO_STATUS == team.active->status)
		poison_toxic (team, team, weather);
	if (team.curse)
		heal (*team.active, -4);
	if (team.partial_trap > 0) {
		heal (*team.active, -16);
		--team.partial_trap;				// No need to use decrement here, as I already know team.partial_trap > 0
	}
	if (BAD_DREAMS == foe.ability and SLEEP == team.active->status)
		heal (*team.active, -8);
	if (0 != team.rampage) {			// Can't use decrement here because I only want to cause confusion when team.rampage becomes 0.
		--team.rampage;
		if (0 == team.rampage)
			team.confused = true;
	}
	else {
		decrement (team.uproar);
		decrement (weather.uproar);
	}
	for (std::vector<Move>::iterator it = team.active->move.set.begin(); it != team.active->move.set.end(); ++it)
		decrement (it->disable);
	decrement (team.encore);
	decrement (team.taunt);
	decrement (team.magnet_rise);
	decrement (team.heal_block);
	decrement (team.embargo);
	if (1 == team.yawn) {
		sleep (*team.active, *team.active, weather);
		team.active->sleep = 0;
	}
	decrement (team.yawn);
	if (STICKY_BARB == team.active->item)
		heal (*team.active, -8);
}

void endofturn6 (Team &target, const Weather &weather) {		// Doom Desire / Future Sight
/*	if (target.counter == 1) {
		defense (target.ddfs, *target.active, weather);
		target.active->hp.stat -= damagecalculator (target.ddfs, target, weather);
	}
	decrement (target.counter);*/
}

void endofturn7 (Team &team)  {
	if (team.perish_song == 1)
		team.active->hp.stat = 0;
	decrement (team.perish_song);
}

void decrement (char &n) {
	if (n > 0)
		--n;
}

}
