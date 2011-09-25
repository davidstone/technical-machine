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

void endofturn (Team & first, Team & last, Weather & weather) {
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
	if (first.pokemon->ability.blocks_weather () or last.pokemon->ability.blocks_weather ()) {
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
	reset_variable (first);
	reset_variable (last);
}

void endofturn0 (Team & team) {
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

void endofturn1 (Team & team) {
	decrement (team.light_screen);
	decrement (team.lucky_chant);
	decrement (team.mist);
	decrement (team.reflect);
	decrement (team.safeguard);
	decrement (team.tailwind);
}

void endofturn2 (Team & team) {
	if (team.wish == 1)
		heal (*team.pokemon, 2);
	decrement (team.wish);
}

void endofturn3 (Team & team, Weather const & weather) {
	if (weather.hail and !is_type (team, ICE))
		heal (*team.pokemon, -16);
	if (weather.sand and !(is_type (team, GROUND) or is_type (team, ROCK) or is_type (team, STEEL)))
		heal (*team.pokemon, -16);
	switch (team.pokemon->ability.name) {
		case Ability::DRY_SKIN:
			if (weather.rain)
				heal (*team.pokemon, 8);
			else if (weather.sun)
				heal (*team.pokemon, -8);
			break;
		case Ability::HYDRATION:
			if (weather.rain)
				team.pokemon->status.clear ();
			break;
		case Ability::ICE_BODY:
			if (weather.hail)
				heal (*team.pokemon, 16);
			break;
		case Ability::RAIN_DISH:
			if (weather.rain)
				heal (*team.pokemon, 16);
			break;
		default:
			break;
	}
}

void endofturn5 (Team & team, Pokemon & foe, Weather & weather) {
	if (team.ingrain)
		heal (*team.pokemon, 16);
	if (team.aqua_ring)
		heal (*team.pokemon, 16);
	if (team.pokemon->ability.name == Ability::SPEED_BOOST)
		team.pokemon->spe.boost (1);
	else if (team.shed_skin)
		team.pokemon->status.clear ();
	switch (team.pokemon->item.name) {
		case Item::LEFTOVERS:
			heal (*team.pokemon, 16);
			break;
		case Item::BLACK_SLUDGE:
			if (is_type (team, POISON))
				heal (*team.pokemon, 16);
			else
				heal (*team.pokemon, -16);
			break;
		default:
			break;
	}
	if (team.leech_seed) {
		unsigned n = team.pokemon->hp.stat;
		heal (*team.pokemon, -8);
		if (foe.hp.stat != 0) {
			if (team.pokemon->ability.name == Ability::LIQUID_OOZE)
				damage_side_effect (foe, n - team.pokemon->hp.stat);
			else {
				foe.hp.stat += n - team.pokemon->hp.stat;
				if (foe.hp.stat > foe.hp.max)
					foe.hp.stat = foe.hp.max;
			}
		}
	}
	switch (team.pokemon->status.name) {
		case Status::BURN:
			if (team.pokemon->ability.name == Ability::HEATPROOF)
				heal (*team.pokemon, -16);
			else
				heal (*team.pokemon, -8);
			break;
		case Status::POISON_NORMAL:
			if (team.pokemon->ability.name == Ability::POISON_HEAL)
				heal (*team.pokemon, 8);
			else
				heal (*team.pokemon, -8);
			break;
		case Status::POISON_TOXIC:
			if (team.pokemon->ability.name == Ability::POISON_HEAL)
				heal (*team.pokemon, 8);
			else {
				if (team.toxic < 15)
					++team.toxic;
				heal (*team.pokemon, -16, team.toxic);
			}
			break;
		case Status::SLEEP:
			if (team.nightmare)
				heal (*team.pokemon, -4);
			if (foe.ability.name == Ability::BAD_DREAMS)
				heal (*team.pokemon, -8);
			break;
		default:
			break;
	}
	switch (team.pokemon->item.name) {
		case Item::FLAME_ORB:
			burn (team, team, weather);
			break;
		case Item::TOXIC_ORB:
			poison_toxic (team, team, weather);
			break;
		default:
			break;
	}
	if (team.curse)
		heal (*team.pokemon, -4);
	if (team.partial_trap > 0) {
		heal (*team.pokemon, -16);
		--team.partial_trap;				// No need to use decrement here, as I already know team.partial_trap > 0
	}
	
	// Can't use decrement here because I only want to cause confusion when team.rampage becomes 0.
	if (team.rampage) {
		--team.rampage;
		if (team.rampage == 0)
			team.confused = true;
	}
	else
		decrement (team.uproar);
	decrement (weather.uproar);

	for (std::vector<Move>::iterator move = team.pokemon->move.set.begin(); move != team.pokemon->move.set.end(); ++move)
		decrement (move->disable);
	decrement (team.encore);
	decrement (team.taunt);
	decrement (team.magnet_rise);
	decrement (team.heal_block);
	decrement (team.embargo);
	if (team.yawn == 1)
		sleep (*team.pokemon, *team.pokemon, weather);
	decrement (team.yawn);
	if (team.pokemon->item.name == Item::STICKY_BARB)
		heal (*team.pokemon, -8);
}

void endofturn6 (Team & target, Weather const & weather) {		// Doom Desire / Future Sight
/*	if (target.counter == 1) {
		defense (target.ddfs, *target.pokemon, weather);
		target.pokemon->hp.stat -= damagecalculator (target.ddfs, target, weather);
	}
	decrement (target.counter);*/
}

void endofturn7 (Team & team) {
	if (team.perish_song == 1)
		team.pokemon->hp.stat = 0;
	decrement (team.perish_song);
}

void reset_variable (Team & team) {
	for (std::vector <Pokemon>::iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon) {
		for (std::vector <Move>::iterator move = pokemon->move.set.begin(); move != pokemon->move.set.end(); ++move) {
			move->variable.index = 0;
		}
	}
}

void decrement (int8_t & n) {
	if (n > 0)
		--n;
}

}
