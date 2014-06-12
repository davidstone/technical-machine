// End of turn effects
// Copyright (C) 2014 David Stone
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
	if (weather.hail() and !is_immune_to_hail(get_type(pokemon)))
		heal(pokemon, make_rational(-1_bi, 16_bi));
	if (weather.sand() and !is_immune_to_sandstorm(get_type(pokemon))) {
		heal(pokemon, make_rational(-1_bi, 16_bi));
	}
	Ability::weather_healing(pokemon, weather);
}

void endofturn5 (ActivePokemon & pokemon, ActivePokemon & foe, Weather & weather) {
	if (get_hp(pokemon) == 0_bi) {
		return;
	}
	if (pokemon.ingrained()) {
		heal(pokemon, make_rational(1_bi, 16_bi));
	}
	if (pokemon.aqua_ring_is_active()) {
		heal(pokemon, make_rational(1_bi, 16_bi));
	}
	if (get_ability(pokemon).boosts_speed()) {
		boost(pokemon.stage(), StatNames::SPE, 1_bi);
	}
	else if (pokemon.shed_skin_activated()) {
		get_status(pokemon) = Status{};
	}
	switch (get_item(pokemon)) {
		case Item::Leftovers:
			heal(pokemon, make_rational(1_bi, 16_bi));
			break;
		case Item::Black_Sludge: {
			auto const numerator = BOUNDED_CONDITIONAL(is_type(pokemon, Type::Poison), 1_bi, -1_bi);
			heal(pokemon, make_rational(numerator, 16_bi));
			break;
		}
		default:
			break;
	}
	if (pokemon.leech_seeded()) {
		auto const initial = get_hp(pokemon).current();
		heal(pokemon, make_rational(-1_bi, 8_bi));
		if (!foe.is_fainted()) {
			if (get_ability(pokemon).damages_leechers()) {
				get_hp(foe) -= initial - get_hp(pokemon).current();
			}
			else {
				get_hp(foe) += initial - get_hp(pokemon).current();
			}
		}
	}
	switch (get_status(pokemon).name()) {
		case Status::burn: {
			auto const denominator = BOUNDED_CONDITIONAL(get_ability(pokemon).weakens_burn(), 16_bi, 8_bi);
			heal(pokemon, make_rational(-1_bi, denominator));
			break;
		}
		case Status::poison: {
			auto const numerator = BOUNDED_CONDITIONAL(get_ability(pokemon).absorbs_poison_damage(), 1_bi, -1_bi);
			heal(pokemon, make_rational(numerator, 8_bi));
			break;
		}
		case Status::poison_toxic:
			pokemon.advance_toxic();
			if (get_ability(pokemon).absorbs_poison_damage()) {
				heal(pokemon, make_rational(1_bi, 8_bi));
			} else {
				heal(pokemon, pokemon.toxic_ratio());
			}
			break;
		case Status::sleep:
			if (pokemon.is_having_a_nightmare()) {
				heal(pokemon, make_rational(-1_bi, 4_bi));
			}
			if (get_ability(foe).harms_sleepers()) {
				heal(pokemon, make_rational(-1_bi, 8_bi));
			}
			break;
		default:
			break;
	}
	switch (get_item(pokemon)) {
		case Item::Flame_Orb:
			Status::apply<Status::burn>(pokemon, weather);
			break;
		case Item::Toxic_Orb:
			Status::apply<Status::poison_toxic>(pokemon, weather);
			break;
		default:
			break;
	}
	if (pokemon.is_cursed()) {
		heal(pokemon, make_rational(-1_bi, 4_bi));
	}
	pokemon.partial_trap_damage();
	
	pokemon.advance_lock_in();
	
	pokemon.advance_disable();
	pokemon.advance_encore();
	pokemon.advance_taunt();
	pokemon.advance_magnet_rise();
	pokemon.advance_heal_block();
	pokemon.advance_embargo();
	pokemon.try_to_activate_yawn(weather);
	if (get_item(pokemon) == Item::Sticky_Barb) {
		heal(pokemon, make_rational(-1_bi, 8_bi));
	}
}

void endofturn6 (Team & target, Weather const & weather) {
	// TODO: Doom Desire / Future Sight
}

void endofturn7 (ActivePokemon & pokemon) {
	pokemon.perish_song_turn();
}

}	// unnamed namespace
}	// namespace technicalmachine
