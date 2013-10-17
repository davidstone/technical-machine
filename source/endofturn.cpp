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

#include "pokemon/active_pokemon.hpp"
#include "pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {

void endofturn1 (Team & team);
void endofturn2 (Team & team);
void endofturn3 (ActivePokemon & pokemon, Weather const & weather);
void endofturn5 (ActivePokemon & pokemon, ActivePokemon & foe, Weather & weather);
void endofturn6 (Team & target, Weather const & weather);
void endofturn7 (ActivePokemon & pokemon);

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
	if (!get_ability(first.pokemon()).blocks_weather() and !get_ability(last.pokemon()).blocks_weather()) {
		endofturn3 (first.pokemon(), weather);
		endofturn3 (last.pokemon(), weather);
	}
	endofturn5(first.pokemon(), last.pokemon(), weather);
	endofturn5(last.pokemon(), first.pokemon(), weather);
	endofturn6 (first, weather);
	endofturn6 (last, weather);
	endofturn7 (first.pokemon());
	endofturn7 (last.pokemon());
}

namespace {

void endofturn1 (Team & team) {
	team.screens.decrement();
}

void endofturn2 (Team & team) {
	team.wish.decrement(team.pokemon());
}

void endofturn3 (ActivePokemon & pokemon, Weather const & weather) {
	if (weather.hail() and !get_type(pokemon).is_immune_to_hail())
		drain(pokemon, Rational(1, 16));
	if (weather.sand() and !get_type(pokemon).is_immune_to_sandstorm()) {
		drain(pokemon, Rational(1, 16));
	}
	Ability::weather_healing(pokemon, weather);
}

void endofturn5 (ActivePokemon & pokemon, ActivePokemon & foe, Weather & weather) {
	if (get_stat(pokemon, Stat::HP).stat == 0) {
		return;
	}
	if (pokemon.ingrained())
		heal(pokemon, Rational(1, 16));
	if (pokemon.aqua_ring_is_active())
		heal(pokemon, Rational(1, 16));
	if (get_ability(pokemon).boosts_speed())
		pokemon.stat_boost(Stat::SPE, 1);
	else if (pokemon.shed_skin_activated())
		get_status(pokemon).clear();
	switch (get_item(pokemon).name) {
		case Item::LEFTOVERS:
			heal(pokemon, Rational(1, 16));
			break;
		case Item::BLACK_SLUDGE:
			if (is_type(pokemon, Type::Poison))
				heal(pokemon, Rational(1, 16));
			else
				drain(pokemon, Rational(1, 16));
			break;
		default:
			break;
	}
	if (pokemon.leech_seeded()) {
		unsigned const n = get_stat(pokemon, Stat::HP).stat;
		drain(pokemon, Rational(1, 8));
		if (!foe.is_fainted()) {
			if (get_ability(pokemon).damages_leechers()) {
				apply_damage(foe, n - get_stat(pokemon, Stat::HP).stat);
			}
			else {
				apply_healing(foe, n - get_stat(pokemon, Stat::HP).stat);
			}
		}
	}
	switch (get_status(pokemon).name()) {
		case Status::BURN:
			drain(pokemon, Rational(1, get_ability(pokemon).weakens_burn() ? 16 : 8));
			break;
		case Status::POISON:
			if (get_ability(pokemon).absorbs_poison_damage())
				heal(pokemon, Rational(1, 8));
			else
				drain(pokemon, Rational(1, 8));
			break;
		case Status::POISON_TOXIC:
			pokemon.increment_toxic();
			if (get_ability(pokemon).absorbs_poison_damage())
				heal(pokemon, Rational(1, 8));
			else
				drain(pokemon, pokemon.toxic_ratio());
			break;
		case Status::SLEEP:
			if (pokemon.nightmare())
				drain(pokemon, Rational(1, 4));
			if (get_ability(foe).harms_sleepers())
				drain(pokemon, Rational(1, 8));
			break;
		default:
			break;
	}
	switch (get_item(pokemon).name) {
		case Item::FLAME_ORB:
			Status::apply<Status::BURN>(pokemon, weather);
			break;
		case Item::TOXIC_ORB:
			Status::apply<Status::POISON_TOXIC>(pokemon, weather);
			break;
		default:
			break;
	}
	if (pokemon.is_cursed())
		drain(pokemon, Rational(1, 4));
	pokemon.partial_trap_damage();
	
	pokemon.decrement_lock_in();
	
	pokemon.advance_disable();
	pokemon.increment_encore();
	pokemon.increment_taunt();
	pokemon.decrement_magnet_rise();
	pokemon.decrement_heal_block();
	pokemon.decrement_embargo();
	if (pokemon.decrement_yawn())
		Status::apply<Status::SLEEP>(pokemon, weather);
	if (get_item(pokemon).name == Item::STICKY_BARB)
		drain(pokemon, Rational(1, 8));
}

void endofturn6 (Team & target, Weather const & weather) {
	// TODO: Doom Desire / Future Sight
}

void endofturn7 (ActivePokemon & pokemon) {
	pokemon.perish_song_turn();
}

}	// unnamed namespace
}	// namespace technicalmachine
