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
#include "expectiminimax.h"
#include "pokemon.h"
#include "simple.h"
#include "statfunction.h"
#include "statusfunction.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void endofturn (Team &first, Team &last, Weather &weather, const Random &random) {
	endofturn0 (*first.active);
	endofturn0 (*last.active);
	endofturn1 (first);
	endofturn1 (last);
	endofturn2 (first);
	endofturn2 (last);
	decrement (weather.hail);
	decrement (weather.sun);
	decrement (weather.sand);
	decrement (weather.rain);
	if (first.active->ability != AIR_LOCK and last.active->ability != AIR_LOCK) {
		endofturn3 (*first.active, weather);
		endofturn3 (*last.active, weather);
	}
	decrement (weather.gravity);
	endofturn5 (*first.active, *last.active, weather, random.first);
	endofturn5 (*last.active, *first.active, weather, random.last);
	endofturn6 (first, weather);
	endofturn6 (last, weather);
	endofturn7 (*first.active);
	endofturn7 (*last.active);
	decrement (weather.trick_room);
}

void endofturn0 (Pokemon &member) {
	member.damaged = false;
	member.flinch = false;
	member.moved = false;
	member.mf = false;
	if (member.loaf)
		member.loaf = false;
	else
		member.loaf = true;
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

void endofturn3 (Pokemon &member, const Weather &weather) {
	if (weather.hail != 0 and !istype (member,ICE))
		heal (member, -16);
	if (weather.sand != 0 and !(istype (member, GROUND) or istype (member, ROCK) or istype (member, STEEL)))
		heal (member, -16);
	if (DRY_SKIN == member.ability) {
		if (0 != weather.rain)
			heal (member, 8);
		else if (0 != weather.sun)
			heal (member, -8);
	}
	else if (HYDRATION == member.ability and 0 != weather.rain)
		member.status = NO_STATUS;
	else if ((ICE_BODY == member.ability and 0 != weather.hail) or (RAIN_DISH == member.ability and 0 != weather.rain))
		heal (member, 16);
}

void endofturn5 (Pokemon &member, Pokemon &foe, Weather &weather, const random_team &random) {
	if (member.ingrain)
		heal (member, 16);
	if (member.aqua_ring)
		heal (member, 16);
	if (SPEED_BOOST == member.ability)
		statboost (member.spe.stage, 1);
	else if (SHED_SKIN == member.ability and random.shed_skin)
		member.status = NO_STATUS;
	if (LEFTOVERS == member.item)
		heal (member, 16);
	else if (BLACK_SLUDGE == member.item) {
		if (istype (member, POISON))
			heal (member, 16);
		else
			heal (member, -16);
	}
	if (member.leech_seed) {
		int n = member.hp.stat;
		heal (member, -8);
		if (foe.hp.stat != 0) {
			if (LIQUID_OOZE == member.ability) {
				foe.hp.stat -= n - member.hp.stat;
				if (foe.hp.stat < 0)
					foe.hp.stat = 0;
			}
			else {
				foe.hp.stat += n - member.hp.stat;
				if (foe.hp.stat > foe.hp.max)
					foe.hp.stat = foe.hp.max;
			}
		}
	}
	if (BURN == member.status) {
		if (HEATPROOF == member.ability)
			heal (member, -16);
		else
			heal (member, -8);
	}
	else if (POISON_NORMAL == member.status or POISON_TOXIC == member.status) {
		if (POISON_HEAL == member.ability)
			heal (member, 8);
		else if (POISON_NORMAL == member.status)
			heal (member, -8);
		else {
			if (15 != member.toxic)
				++member.toxic;
			heal (member, -16, member.toxic);
		}
	}
	else if (member.nightmare)
		heal (member, -4);
	if (FLAME_ORB == member.item)
		burn (member, member, weather);
	else if (TOXIC_ORB == member.item and NO_STATUS == member.status)
		poison_toxic (member, member, weather);
	if (member.curse)
		heal (member, -4);
	if (0 < member.partial_trap) {
		heal (member, -16);
		--member.partial_trap;				// No need to use decrement here, as I already know member.partial_trap > 0
	}
	if (BAD_DREAMS == foe.ability and SLEEP == member.status)
		heal (member, -8);
	if (0 != member.rampage) {			// Can't use decrement here because I only want to cause confusion when member.rampage becomes 0.
		--member.rampage;
		if (0 == member.rampage)
			member.confused = true;
	}
	else {
		decrement (member.uproar);
		decrement (weather.uproar);
	}
	for (std::vector<moves>::iterator it = member.moveset.begin(); it != member.moveset.end(); ++it)
		decrement (it->disable);
	decrement (member.encore);
	decrement (member.taunt);
	decrement (member.magnet_rise);
	decrement (member.heal_block);
	decrement (member.embargo);
	if (1 == member.yawn) {
		sleep (member, member, weather);
		member.sleep = random.length;
	}
	decrement (member.yawn);
	if (STICKY_BARB == member.item)
		heal (member, -8);
}

void endofturn6 (Team &target, const Weather &weather) {		// Doom Desire / Future Sight
/*	if (target.counter == 1) {
		defense (target.ddfs, *target.active, weather);
		target.active->hp.stat -= damagecalculator (target.ddfs, target, weather);
	}
	decrement (target.counter);*/
}

void endofturn7 (Pokemon &member)  {
	if (member.perish_song == 1)
		member.hp.stat = 0;
	decrement (member.perish_song);
}

}
