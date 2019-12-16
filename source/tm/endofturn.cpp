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
void end_of_turn2(Generation, Team & team, Weather);
void end_of_turn3(Generation, MutableActivePokemon pokemon, ActivePokemon other, Weather const weather);
void end_of_turn5(Generation, MutableActivePokemon pokemon, MutableActivePokemon foe, Weather & weather, EndOfTurnFlags flags);
void end_of_turn6(Team & target, Weather const weather);
void end_of_turn7(MutableActivePokemon pokemon);

}	// namespace

void end_of_turn(Generation const generation, Team & first, EndOfTurnFlags const first_flags, Team & last, EndOfTurnFlags const last_flags, Weather & weather) {
	end_of_turn1(first);
	end_of_turn1(last);
	end_of_turn2(generation, first, weather);
	end_of_turn2(generation, last, weather);
	weather.advance_one_turn();
	auto const first_pokemon = first.pokemon();
	auto const last_pokemon = last.pokemon();
	end_of_turn3(generation, first_pokemon, last_pokemon, weather);
	end_of_turn3(generation, last_pokemon, first_pokemon, weather);
	end_of_turn5(generation, first_pokemon, last_pokemon, weather, first_flags);
	end_of_turn5(generation, last_pokemon, first_pokemon, weather, last_flags);
	end_of_turn6(first, weather);
	end_of_turn6(last, weather);
	end_of_turn7(first_pokemon);
	end_of_turn7(last_pokemon);
	first.reset_end_of_turn();
	last.reset_end_of_turn();
}

namespace {

void end_of_turn1(Team & team) {
	team.screens.decrement();
}

void end_of_turn2(Generation const generation, Team & team, Weather const weather) {
	team.wish.decrement(generation, team.pokemon(), weather);
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

void weather_healing_ability(Generation const generation, MutableActivePokemon pokemon, Weather const weather, bool const ability_blocks_weather) {
	switch (get_ability(pokemon)) {
		case Ability::Dry_Skin:
			if (weather.rain(ability_blocks_weather)) {
				heal(generation, pokemon, weather, rational(1_bi, 8_bi));
			} else if (weather.sun(ability_blocks_weather)) {
				heal(generation, pokemon, weather, rational(-1_bi, 8_bi));
			}
			break;
		case Ability::Hydration:
			if (weather.rain(ability_blocks_weather)) {
				pokemon.clear_status();
			}
			break;
		case Ability::Ice_Body:
			if (weather.hail(ability_blocks_weather)) {
				heal(generation, pokemon, weather, rational(1_bi, 16_bi));
			}
			break;
		case Ability::Rain_Dish:
			if (weather.rain(ability_blocks_weather)) {
				heal(generation, pokemon, weather, rational(1_bi, 16_bi));
			}
			break;
		default:
			break;
	}
}

void end_of_turn3(Generation const generation, MutableActivePokemon pokemon, ActivePokemon other, Weather const weather) {
	auto const ability_blocks_weather = weather_is_blocked_by_ability(get_ability(pokemon), get_ability(other));
	if (weather.hail(ability_blocks_weather) and !is_immune_to_hail(pokemon.types()))
		heal(generation, pokemon, weather, rational(-1_bi, 16_bi));
	if (weather.sand(ability_blocks_weather) and !is_immune_to_sandstorm(pokemon.types())) {
		heal(generation, pokemon, weather, rational(-1_bi, 16_bi));
	}
	weather_healing_ability(generation, pokemon, weather, ability_blocks_weather);
}

void end_of_turn5(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon foe, Weather & weather, EndOfTurnFlags const flags) {
	if (get_hp(pokemon) == 0_bi) {
		return;
	}
	if (pokemon.ingrained()) {
		heal(generation, pokemon, weather, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(generation, weather)));
	}
	if (pokemon.aqua_ring_is_active()) {
		heal(generation, pokemon, weather, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(generation, weather)));
	}
	if (boosts_speed(get_ability(pokemon))) {
		pokemon.stage()[StatNames::SPE] += 1_bi;
	} else if (flags.shed_skin) {
		pokemon.clear_status();
	}
	switch (pokemon.item(generation, weather)) {
		case Item::Leftovers:
			heal(generation, pokemon, weather, rational(1_bi, 16_bi));
			break;
		case Item::Black_Sludge:
			heal(generation, pokemon, weather, BOUNDED_CONDITIONAL(is_type(pokemon, Type::Poison), rational(1_bi, 16_bi), rational(-1_bi, 8_bi)));
			break;
		default:
			break;
	}
	if (pokemon.leech_seeded()) {
		auto const initial = get_hp(pokemon).current();
		heal(generation, pokemon, weather, rational(-1_bi, 8_bi));
		if (get_hp(foe) != 0_bi) {
			auto const hp_change = (initial - get_hp(pokemon).current()) * healing_multiplier(pokemon.item(generation, weather));
			if (damages_leechers(get_ability(pokemon))) {
				change_hp(generation, foe, weather, -hp_change);
			} else {
				change_hp(generation, foe, weather, hp_change);
			}
		}
	}
	// TODO: Not sure if this check for Uproar is in the correct place
	auto const uproar = pokemon.is_uproaring() or foe.is_uproaring();
	pokemon.end_of_turn_status(generation, foe, weather, uproar);
	switch (pokemon.item(generation, weather)) {
		case Item::Flame_Orb:
			apply_status_to_self(generation, Statuses::burn, pokemon, weather);
			break;
		case Item::Toxic_Orb:
			apply_status_to_self(generation, Statuses::toxic, pokemon, weather);
			break;
		default:
			break;
	}
	if (pokemon.is_cursed()) {
		heal(generation, pokemon, weather, rational(-1_bi, 4_bi));
	}
	pokemon.partial_trap_damage(generation, weather);
	
	pokemon.advance_lock_in(generation, flags.lock_in_ends, weather);
	
	pokemon.advance_disable();
	pokemon.advance_encore();
	pokemon.advance_taunt();
	pokemon.advance_magnet_rise();
	pokemon.advance_heal_block();
	pokemon.advance_embargo();
	pokemon.try_to_activate_yawn(generation, weather, uproar);
	if (pokemon.item(generation, weather) == Item::Sticky_Barb) {
		heal(generation, pokemon, weather, rational(-1_bi, 8_bi));
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
