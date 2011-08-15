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

#include <cstdint>
#include "endofturn.h"
#include "ability.h"
#include "damage.h"
#include "heal.h"
#include "move.h"
#include "pokemon.h"
#include "status.h"
#include "team.h"
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
	if (first.pokemon->ability != AIR_LOCK and last.pokemon->ability != AIR_LOCK) {
		endofturn3 (first, weather);
		endofturn3 (last, weather);
	}
	decrement (weather.gravity);
	endofturn5 (first, *last.pokemon, weather);
	endofturn5 (last, *first.pokemon, weather);
	endofturn6 (first, weather);
	endofturn6 (last, weather);
	endofturn7 (first);
	endofturn7 (last);
	decrement (weather.trick_room);
}

void endofturn0 (Team &team) {
	team.damage = 0;
	team.damaged = false;
	team.endure = false;
	team.flinch = false;
	team.moved = false;
	team.mf = false;
	if (team.loaf)
		team.loaf = false;
	else
		team.loaf = true;
	team.protect = false;
	team.replacing = false;
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
		heal (*team.pokemon, 2);
	decrement (team.wish);
}

void endofturn3 (Team &team, Weather const &weather) {
	if (weather.hail != 0 and !istype (team, ICE))
		heal (*team.pokemon, -16);
	if (weather.sand != 0 and !(istype (team, GROUND) or istype (team, ROCK) or istype (team, STEEL)))
		heal (*team.pokemon, -16);
	if (DRY_SKIN == team.pokemon->ability) {
		if (0 != weather.rain)
			heal (*team.pokemon, 8);
		else if (0 != weather.sun)
			heal (*team.pokemon, -8);
	}
	else if (HYDRATION == team.pokemon->ability and 0 != weather.rain)
		team.pokemon->status = NO_STATUS;
	else if ((ICE_BODY == team.pokemon->ability and 0 != weather.hail) or (RAIN_DISH == team.pokemon->ability and 0 != weather.rain))
		heal (*team.pokemon, 16);
}

void endofturn5 (Team &team, Pokemon &foe, Weather &weather) {
	if (team.ingrain)
		heal (*team.pokemon, 16);
	if (team.aqua_ring)
		heal (*team.pokemon, 16);
	if (SPEED_BOOST == team.pokemon->ability)
		team.pokemon->spe.boost (1);
	else if (team.shed_skin)
		team.pokemon->status = NO_STATUS;
	if (LEFTOVERS == team.pokemon->item)
		heal (*team.pokemon, 16);
	else if (BLACK_SLUDGE == team.pokemon->item) {
		if (istype (team, POISON))
			heal (*team.pokemon, 16);
		else
			heal (*team.pokemon, -16);
	}
	if (team.leech_seed) {
		unsigned n = team.pokemon->hp.stat;
		heal (*team.pokemon, -8);
		if (foe.hp.stat != 0) {
			if (LIQUID_OOZE == team.pokemon->ability)
				damage_side_effect (foe, n - team.pokemon->hp.stat);
			else {
				foe.hp.stat += n - team.pokemon->hp.stat;
				if (foe.hp.stat > foe.hp.max)
					foe.hp.stat = foe.hp.max;
			}
		}
	}
	if (BURN == team.pokemon->status) {
		if (HEATPROOF == team.pokemon->ability)
			heal (*team.pokemon, -16);
		else
			heal (*team.pokemon, -8);
	}
	else if (POISON_NORMAL == team.pokemon->status or POISON_TOXIC == team.pokemon->status) {
		if (POISON_HEAL == team.pokemon->ability)
			heal (*team.pokemon, 8);
		else if (POISON_NORMAL == team.pokemon->status)
			heal (*team.pokemon, -8);
		else {
			if (15 != team.toxic)
				++team.toxic;
			heal (*team.pokemon, -16, team.toxic);
		}
	}
	else if (team.nightmare)
		heal (*team.pokemon, -4);
	if (FLAME_ORB == team.pokemon->item)
		burn (team, team, weather);
	else if (TOXIC_ORB == team.pokemon->item and NO_STATUS == team.pokemon->status)
		poison_toxic (team, team, weather);
	if (team.curse)
		heal (*team.pokemon, -4);
	if (team.partial_trap > 0) {
		heal (*team.pokemon, -16);
		--team.partial_trap;				// No need to use decrement here, as I already know team.partial_trap > 0
	}
	if (BAD_DREAMS == foe.ability and SLEEP == team.pokemon->status)
		heal (*team.pokemon, -8);
	if (0 != team.rampage) {			// Can't use decrement here because I only want to cause confusion when team.rampage becomes 0.
		--team.rampage;
		if (0 == team.rampage)
			team.confused = true;
	}
	else {
		decrement (team.uproar);
		decrement (weather.uproar);
	}
	for (std::vector<Move>::iterator it = team.pokemon->move.set.begin(); it != team.pokemon->move.set.end(); ++it)
		decrement (it->disable);
	decrement (team.encore);
	decrement (team.taunt);
	decrement (team.magnet_rise);
	decrement (team.heal_block);
	decrement (team.embargo);
	if (team.yawn == 1)
		sleep (*team.pokemon, *team.pokemon, weather);
	decrement (team.yawn);
	if (STICKY_BARB == team.pokemon->item)
		heal (*team.pokemon, -8);
}

void endofturn6 (Team &target, Weather const &weather) {		// Doom Desire / Future Sight
/*	if (target.counter == 1) {
		defense (target.ddfs, *target.pokemon, weather);
		target.pokemon->hp.stat -= damagecalculator (target.ddfs, target, weather);
	}
	decrement (target.counter);*/
}

void endofturn7 (Team &team) {
	if (team.perish_song == 1)
		team.pokemon->hp.stat = 0;
	decrement (team.perish_song);
}

void decrement (int8_t &n) {
	if (n > 0)
		--n;
}

}
