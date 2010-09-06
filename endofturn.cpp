// End of turn effects
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ability.h"
#include "damage.cpp"
#include "pokemon.h"
#include "simple.h"
#include "team.h"
#include "weather.h"

void endofturn0 (teams &team);
void endofturn1 (teams &team);
void endofturn2 (teams &team);
void endofturn4 (teams &team, const weathers &weather);
void endofturn6 (teams &first, teams &last, weathers &weather, int length_first, bool shed_skin_first);
void endofturn7 (teams &target, const weathers &weather);
void endofturn8 (teams &team);
void endofturn10 (teams &team, const pokemon &replacement, const weathers &weather);

void endofturn (teams &first, teams &last, weathers &weather, int length_first, int length_last, bool shed_skin_first, bool shed_skin_last) {
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
	endofturn4 (first, weather);
	endofturn4 (last, weather);
	decrement (weather.gravity);
	endofturn6 (first, last, weather, length_first, shed_skin_first);
	endofturn6 (last, first, weather, length_last, shed_skin_last);
	endofturn7 (first, weather);
	endofturn7 (last, weather);
	endofturn8 (first);
	endofturn8 (last);
	decrement (weather.trick_room);
//	if (first.active->hp.stat == 0)
//		first.member.erase (first.active);
//	endofturn10 (first);
//	if (last.active->hp.stat == 0)
//		last.member.erase (last.active);
//	endofturn10 (last);
}

void endofturn0 (teams &team) {
	team.active->damaged = false;
	team.active->flinch = false;
	team.active->moved = false;
	team.active->mf = false;
	if (team.active->loaf)
		team.active->loaf = false;
	else
		team.active->loaf = true;
}

void endofturn1 (teams &team) {
	decrement (team.light_screen);
	decrement (team.lucky_chant);
	decrement (team.mist);
	decrement (team.reflect);
	decrement (team.safeguard);
	decrement (team.tailwind);
}

void endofturn2 (teams &team) {
	if (1 == team.wish)
		heal (*team.active, 2);
	decrement (team.wish);
}

void endofturn4 (teams &team, const weathers &weather) {
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

void endofturn6 (teams &first, teams &last, weathers &weather, int length, bool shed_skin) {
	if (first.active->ingrain)
		heal (*first.active, 16);
	if (first.active->aqua_ring)
		heal (*first.active, 16);
	if (SPEED_BOOST == first.active->ability)
		statboost (first.active->spe, 1);
	else if (SHED_SKIN == first.active->ability and shed_skin)
		first.active->status = NO_STATUS;
	if (LEFTOVERS == first.active->item)
		heal (*first.active, 16);
	else if (BLACK_SLUDGE == first.active->item) {
		if (istype (*first.active, POISON))
			heal (*first.active, 16);
		else
			heal (*first.active, -16);
	}
	if (first.active->leech_seed) {
		int n = first.active->hp.stat;
		heal (*first.active, -8);
		if (last.active->hp.stat != 0) {
			if (LIQUID_OOZE == first.active->ability) {
				last.active->hp.stat -= n - first.active->hp.stat;
				if (last.active->hp.stat < 0)
					last.active->hp.stat = 0;
			}
			else {
				last.active->hp.stat += n - first.active->hp.stat;
				if (last.active->hp.stat > last.active->hp.max)
					last.active->hp.stat = last.active->hp.max;
			}
		}
	}
	if (BURN == first.active->status) {
		if (HEATPROOF == first.active->ability)
			heal (*first.active, -16);
		else
			heal (*first.active, -8);
	}
	else if (POISON_NORMAL == first.active->status or POISON_TOXIC == first.active->status) {
		if (POISON_HEAL == first.active->ability)
			heal (*first.active, 8);
		else if (POISON_NORMAL == first.active->status)
			heal (*first.active, -8);
		else {
			if (15 != first.active->toxic)
				++first.active->toxic;
			heal (*first.active, -16, first.active->toxic);
		}
	}
	else if (first.active->nightmare)
		heal (*first.active, -4);
	if (FLAME_ORB == first.active->item and NO_STATUS == first.active->status)
		first.active->status = BURN;
	else if (TOXIC_ORB == first.active->item and NO_STATUS == first.active->status)
		first.active->status = POISON_TOXIC;
	if (first.active->curse)
		heal (*first.active, -4);
	if (0 < first.active->partial_trap) {
		heal (*first.active, -16);
		--first.active->partial_trap;				// No need to use decrement here, as I already know first.active->partial_trap > 0
	}
	if (BAD_DREAMS == last.active->ability and SLEEP == first.active->status)
		heal (*first.active, -8);
	if (0 != first.active->rampage) {			// Can't use decrement here because I only want to cause confusion when first.active->rampage becomes 0.
		--first.active->rampage;
		if (0 == first.active->rampage)
			first.active->confused = true;
	}
	else {
		decrement (first.active->uproar);
		decrement (weather.uproar);
	}
	for (std::vector<moves>::iterator it = first.active->moveset.begin(); it != first.active->moveset.end(); ++it)
		decrement (it->disable);
	decrement (first.active->encore);
	decrement (first.active->taunt);
	decrement (first.active->magnet_rise);
	decrement (first.active->heal_block);
	decrement (first.active->embargo);
	if (1 == first.active->yawn and NO_STATUS == first.active->status) {
		first.active->status = SLEEP;
		first.active->sleep = length;
	}
	decrement (first.active->yawn);
	if (STICKY_BARB == first.active->item)
		heal (*first.active, -8);
}

void endofturn7 (teams &target, const weathers &weather) {		// Doom Desire / Future Sight
	if (1 == target.counter) {
		defense (target.ddfs, *target.active, weather);
		target.active->hp.stat -= damagecalculator (target.ddfs, target, weather);
	}
	decrement (target.counter);
}

void endofturn8 (teams &team)  {
	if (1 == team.active->perish_song)
		team.active->hp.stat = 0;
	decrement (team.active->perish_song);
}

void endofturn10 (teams &team, const pokemon &replacement, const weathers &weather) {
	if (0 == team.active->hp.stat)
		switchpokemon (team, replacement, weather);
}
