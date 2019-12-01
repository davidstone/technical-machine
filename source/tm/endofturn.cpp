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
#include <tm/heal.hpp>
#include <tm/rational.hpp>
#include <tm/status.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {
namespace {

void end_of_turn1(Team & team);
void end_of_turn2(Team & team);
void end_of_turn3(MutableActivePokemon pokemon, Weather const weather);
void end_of_turn5(MutableActivePokemon pokemon, MutableActivePokemon foe, Weather & weather, EndOfTurnFlags flags);
void end_of_turn6(Team & target, Weather const weather);
void end_of_turn7(MutableActivePokemon pokemon);

}	// namespace

void end_of_turn(Generation, Team & first, EndOfTurnFlags const first_flags, Team & last, EndOfTurnFlags const last_flags, Weather & weather) {
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
	end_of_turn5(first.pokemon(), last.pokemon(), weather, first_flags);
	end_of_turn5(last.pokemon(), first.pokemon(), weather, last_flags);
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

auto is_immune_to_hail(PokemonTypes const types) -> bool {
	return containers::any(types, [](Type const type) { return type == Type::Ice; });
}

auto is_immune_to_sandstorm(PokemonTypes const types) -> bool {
	return containers::any(types, [](Type const type) {
		switch (type) {
			case Type::Ground:
			case Type::Rock:
			case Type::Steel:
				return true;
			default:
				return false;
		}
	});
}

void end_of_turn3(MutableActivePokemon pokemon, Weather const weather) {
	if (weather.hail() and !is_immune_to_hail(pokemon.types()))
		heal(pokemon, rational(-1_bi, 16_bi));
	if (weather.sand() and !is_immune_to_sandstorm(pokemon.types())) {
		heal(pokemon, rational(-1_bi, 16_bi));
	}
	weather_healing_ability(pokemon, weather);
}

void end_of_turn5(MutableActivePokemon pokemon, MutableActivePokemon foe, Weather & weather, EndOfTurnFlags const flags) {
	if (get_hp(pokemon) == 0_bi) {
		return;
	}
	if (pokemon.ingrained()) {
		heal(pokemon, rational(1_bi, 16_bi));
	}
	if (pokemon.aqua_ring_is_active()) {
		heal(pokemon, rational(1_bi, 16_bi));
	}
	if (boosts_speed(get_ability(pokemon))) {
		boost(pokemon.stage(), StatNames::SPE, 1_bi);
	} else if (flags.shed_skin) {
		clear_status(pokemon);
	}
	switch (get_item(pokemon)) {
		case Item::Leftovers:
			heal(pokemon, rational(1_bi, 16_bi));
			break;
		case Item::Black_Sludge: {
			auto const numerator = BOUNDED_CONDITIONAL(is_type(pokemon, Type::Poison), 1_bi, -1_bi);
			heal(pokemon, rational(numerator, 16_bi));
			break;
		}
		default:
			break;
	}
	if (pokemon.leech_seeded()) {
		auto const initial = get_hp(pokemon).current();
		heal(pokemon, rational(-1_bi, 8_bi));
		if (get_hp(foe) != 0_bi) {
			auto const hp_change = initial - get_hp(pokemon).current();
			if (damages_leechers(get_ability(pokemon))) {
				change_hp(foe, -hp_change);
			} else {
				change_hp(foe, hp_change);
			}
		}
	}
	// TODO: Not sure if this effect is in the correct order
	auto const uproar = pokemon.is_uproaring() or foe.is_uproaring();
	advance_status_end_of_turn(pokemon, pokemon.is_having_a_nightmare(), foe, uproar);
	switch (get_item(pokemon)) {
		case Item::Flame_Orb:
			apply_status_to_self(Statuses::burn, pokemon, weather, uproar);
			break;
		case Item::Toxic_Orb:
			apply_status_to_self(Statuses::toxic, pokemon, weather, uproar);
			break;
		default:
			break;
	}
	if (pokemon.is_cursed()) {
		heal(pokemon, rational(-1_bi, 4_bi));
	}
	pokemon.partial_trap_damage();
	
	pokemon.advance_lock_in(flags.lock_in_ends);
	
	pokemon.advance_disable();
	pokemon.advance_encore();
	pokemon.advance_taunt();
	pokemon.advance_magnet_rise();
	pokemon.advance_heal_block();
	pokemon.advance_embargo();
	pokemon.try_to_activate_yawn(weather, uproar);
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
