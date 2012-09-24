// End of turn effects
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

#include "endofturn.hpp"

#include "ability.hpp"
#include "damage.hpp"
#include "heal.hpp"
#include "rational.hpp"
#include "status.hpp"
#include "team.hpp"
#include "weather.hpp"

#include "move/move.hpp"

#include "pokemon/active_pokemon.hpp"
#include "pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {

void endofturn1 (Team & team);
void endofturn2 (Team & team);
void endofturn3 (ActivePokemon & pokemon, Weather const & weather);
void endofturn5 (ActivePokemon & pokemon, Pokemon & foe, Weather & weather);
void endofturn6 (Team & target, Weather const & weather);
void endofturn7 (ActivePokemon & pokemon);
void reset_variable (Team & team);

template<typename Integer>
void decrement (Integer & n) {
	if (n > 0)
		--n;
}

}	// unnamed namespace

void endofturn (Team & first, Team & last, Weather & weather) {
	first.pokemon().reset_end_of_turn();
	last.pokemon().reset_end_of_turn();
	endofturn1 (first);
	endofturn1 (last);
	endofturn2 (first);
	endofturn2 (last);
	weather.decrement();	// The order doesn't matter here.
	if (!first.pokemon().ability().blocks_weather() and !last.pokemon().ability().blocks_weather()) {
		endofturn3 (first.pokemon(), weather);
		endofturn3 (last.pokemon(), weather);
	}
	endofturn5 (first.pokemon(), last.pokemon().get_pokemon(), weather);
	endofturn5 (last.pokemon(), first.pokemon().get_pokemon(), weather);
	endofturn6 (first, weather);
	endofturn6 (last, weather);
	endofturn7 (first.pokemon());
	endofturn7 (last.pokemon());
	reset_variable (first);
	reset_variable (last);
}

namespace {

void endofturn1 (Team & team) {
	team.screens.decrement();
}

void endofturn2 (Team & team) {
	team.wish.decrement(team.pokemon().get_pokemon());
}

void endofturn3 (ActivePokemon & pokemon, Weather const & weather) {
	if (weather.hail() and !pokemon.type().is_immune_to_hail())
		heal (pokemon.get_pokemon(), -16);
	if (weather.sand() and !pokemon.type().is_immune_to_sandstorm())
		heal (pokemon.get_pokemon(), -16);
	switch (pokemon.ability().name) {
		case Ability::DRY_SKIN:
			if (weather.rain())
				heal (pokemon.get_pokemon(), 8);
			else if (weather.sun())
				heal (pokemon.get_pokemon(), -8);
			break;
		case Ability::HYDRATION:
			if (weather.rain())
				pokemon.status().clear ();
			break;
		case Ability::ICE_BODY:
			if (weather.hail())
				heal (pokemon.get_pokemon(), 16);
			break;
		case Ability::RAIN_DISH:
			if (weather.rain())
				heal (pokemon.get_pokemon(), 16);
			break;
		default:
			break;
	}
}

void endofturn5 (ActivePokemon & pokemon, Pokemon & foe, Weather & weather) {
	if (pokemon.ingrained())
		heal (pokemon.get_pokemon(), 16);
	if (pokemon.aqua_ring_is_active())
		heal (pokemon.get_pokemon(), 16);
	if (pokemon.ability().boosts_speed())
		pokemon.stat_boost(Stat::SPE, 1);
	else if (pokemon.shed_skin_activated())
		pokemon.status().clear();
	switch (pokemon.item().name) {
		case Item::LEFTOVERS:
			heal (pokemon.get_pokemon(), 16);
			break;
		case Item::BLACK_SLUDGE:
			heal (pokemon.get_pokemon(), (is_type(pokemon, Type::POISON)) ? 16 : -16);
			break;
		default:
			break;
	}
	if (pokemon.leech_seeded()) {
		unsigned const n = pokemon.hp().stat;
		heal(pokemon.get_pokemon(), -8);
		if (foe.hp.stat != 0) {
			if (pokemon.ability().damages_leechers ())
				damage_side_effect (foe, n - pokemon.hp().stat);
			else {
				foe.hp.stat += n - pokemon.hp().stat;
				if (foe.hp.stat > foe.hp.max)
					foe.hp.stat = foe.hp.max;
			}
		}
	}
	switch (pokemon.status().name()) {
		case Status::BURN:
			heal(pokemon.get_pokemon(), pokemon.ability().weakens_burn() ? -16 : -8);
			break;
		case Status::POISON:
			heal(pokemon.get_pokemon(), pokemon.ability().absorbs_poison_damage() ? 8 : -8);
			break;
		case Status::POISON_TOXIC:
			pokemon.increment_toxic();
			if (pokemon.ability().absorbs_poison_damage())
				heal(pokemon.get_pokemon(), 8);
			else
				drain(pokemon.get_pokemon(), pokemon.toxic_ratio());
			break;
		case Status::SLEEP:
			if (pokemon.nightmare())
				heal(pokemon.get_pokemon(), -4);
			if (foe.ability().harms_sleepers())
				heal(pokemon.get_pokemon(), -8);
			break;
		default:
			break;
	}
	switch (pokemon.item().name) {
		case Item::FLAME_ORB:
			Status::apply<Status::BURN>(pokemon.get_pokemon(), weather);
			break;
		case Item::TOXIC_ORB:
			Status::apply<Status::POISON_TOXIC>(pokemon.get_pokemon(), weather);
			break;
		default:
			break;
	}
	if (pokemon.is_cursed())
		heal(pokemon.get_pokemon(), -4);
	pokemon.partial_trap_damage();
	
	pokemon.decrement_lock_in();
	
	pokemon.get_pokemon().move.for_each_regular_move([](Move & move) {
		move.disable.advance_one_turn();
	});
	pokemon.increment_encore();
	pokemon.increment_taunt();
	pokemon.decrement_magnet_rise();
	pokemon.decrement_heal_block();
	pokemon.decrement_embargo();
	if (pokemon.decrement_yawn())
		Status::apply<Status::SLEEP>(pokemon.get_pokemon(), weather);
	if (pokemon.item().name == Item::STICKY_BARB)
		heal(pokemon.get_pokemon(), -8);
}

void endofturn6 (Team & target, Weather const & weather) {
	// TODO: Doom Desire / Future Sight
}

void endofturn7 (ActivePokemon & pokemon) {
	pokemon.perish_song_turn();
}

void reset_variable (Team & team) {
	team.all_pokemon().for_each([](Pokemon & pokemon) {
		pokemon.move.for_each([](Move & move) {
			move.variable.reset_index();
		});
	});
}

}	// unnamed namespace
}	// namespace technicalmachine
