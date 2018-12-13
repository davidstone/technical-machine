// End of turn effects
// Copyright (C) 2016 David Stone
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

#include <tm/endofturn.hpp>

#include <tm/ability.hpp>
#include <tm/damage.hpp>
#include <tm/heal.hpp>
#include <tm/rational.hpp>
#include <tm/status.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/pokemon.hpp>

namespace technicalmachine {
namespace {

void end_of_turn1(Team & team);
void end_of_turn2(Team & team);
void end_of_turn3(MutableActivePokemon pokemon, Weather const weather);
void end_of_turn5(MutableActivePokemon pokemon, MutableActivePokemon foe, Weather & weather, bool shed_skin_activated);
void end_of_turn6(Team & target, Weather const weather);
void end_of_turn7(MutableActivePokemon pokemon);

}	// namespace

void end_of_turn(Team & first, Team & last, Weather & weather, bool const first_shed_skin, bool const last_shed_skin) {
	// TODO: Cure Pokemon of sleep if Uproar is active. Where is this effect ordered?
	first.reset_end_of_turn();
	last.reset_end_of_turn();
	end_of_turn1(first);
	end_of_turn1(last);
	end_of_turn2(first);
	end_of_turn2(last);
	weather.advance_one_turn();
	if (!blocks_weather(get_ability(first.pokemon())) and !blocks_weather(get_ability(last.pokemon()))) {
		end_of_turn3(first.pokemon(), weather);
		end_of_turn3(last.pokemon(), weather);
	}
	end_of_turn5(first.pokemon(), last.pokemon(), weather, first_shed_skin);
	end_of_turn5(last.pokemon(), first.pokemon(), weather, last_shed_skin);
	end_of_turn6(first, weather);
	end_of_turn6(last, weather);
	end_of_turn7(first.pokemon());
	end_of_turn7(last.pokemon());
}

namespace {

void end_of_turn1(Team & team) {
	team.screens.decrement();
}

void end_of_turn2(Team & team) {
	team.wish.decrement(team.pokemon());
}

void end_of_turn3(MutableActivePokemon pokemon, Weather const weather) {
	if (weather.hail() and !is_immune_to_hail(get_type(pokemon)))
		heal(pokemon, rational(-1_bi, 16_bi));
	if (weather.sand() and !is_immune_to_sandstorm(get_type(pokemon))) {
		heal(pokemon, rational(-1_bi, 16_bi));
	}
	weather_healing_ability(pokemon, weather);
}

void end_of_turn5(MutableActivePokemon pokemon, MutableActivePokemon foe, Weather & weather, bool const shed_skin_activated) {
	if (get_hp(pokemon) == 0_bi) {
		return;
	}
	if (ingrained(pokemon)) {
		heal(pokemon, rational(1_bi, 16_bi));
	}
	if (aqua_ring_is_active(pokemon)) {
		heal(pokemon, rational(1_bi, 16_bi));
	}
	if (boosts_speed(get_ability(pokemon))) {
		boost(stage(pokemon), StatNames::SPE, 1_bi);
	} else if (shed_skin_activated) {
		get_status(pokemon) = Status{};
	}
	switch (get_item(pokemon)) {
		case Item::Leftovers:
			heal(pokemon, rational(1_bi, 16_bi));
			break;
		case Item::Black_Sludge: {
			auto const numerator = BOUNDED_CONDITIONAL(is_type(pokemon, Type::Poison, false), 1_bi, -1_bi);
			heal(pokemon, rational(numerator, 16_bi));
			break;
		}
		default:
			break;
	}
	if (leech_seeded(pokemon)) {
		auto const initial = get_hp(pokemon).current();
		heal(pokemon, rational(-1_bi, 8_bi));
		auto & foe_hp = get_hp(foe);
		if (foe_hp != 0_bi) {
			auto const hp_change = initial - get_hp(pokemon).current();
			if (damages_leechers(get_ability(pokemon))) {
				foe_hp -= hp_change;
			} else {
				foe_hp += hp_change;
			}
		}
	}
	switch (get_status(pokemon).name()) {
		case Statuses::burn: {
			auto const denominator = BOUNDED_CONDITIONAL(weakens_burn(get_ability(pokemon)), 16_bi, 8_bi);
			heal(pokemon, rational(-1_bi, denominator));
			break;
		}
		case Statuses::poison: {
			auto const numerator = BOUNDED_CONDITIONAL(absorbs_poison_damage(get_ability(pokemon)), 1_bi, -1_bi);
			heal(pokemon, rational(numerator, 8_bi));
			break;
		}
		case Statuses::poison_toxic:
			pokemon.advance_toxic();
			if (absorbs_poison_damage(get_ability(pokemon))) {
				heal(pokemon, rational(1_bi, 8_bi));
			} else {
				heal(pokemon, toxic_ratio(pokemon));
			}
			break;
		case Statuses::sleep:
			if (is_having_a_nightmare(pokemon)) {
				heal(pokemon, rational(-1_bi, 4_bi));
			}
			if (harms_sleepers(get_ability(foe))) {
				heal(pokemon, rational(-1_bi, 8_bi));
			}
			break;
		default:
			break;
	}
	switch (get_item(pokemon)) {
		case Item::Flame_Orb:
			apply(Statuses::burn, pokemon, weather);
			break;
		case Item::Toxic_Orb:
			apply(Statuses::poison_toxic, pokemon, weather);
			break;
		default:
			break;
	}
	if (is_cursed(pokemon)) {
		heal(pokemon, rational(-1_bi, 4_bi));
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
		heal(pokemon, rational(-1_bi, 8_bi));
	}
}

void end_of_turn6(Team &, Weather const) {
	// TODO: Doom Desire / Future Sight
}

void end_of_turn7(MutableActivePokemon pokemon) {
	pokemon.perish_song_turn();
}

}	// namespace
}	// namespace technicalmachine
