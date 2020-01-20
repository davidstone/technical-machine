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

#include <tm/end_of_turn.hpp>

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

// https://web.archive.org/web/20130530163614/http://www.upokecenter.com/games/gs/guides/timing.php

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

void hail_effect(Generation const generation, MutableActivePokemon pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Ice_Body:
			heal(generation, pokemon, weather, rational(1_bi, 16_bi));
			break;
		case Ability::Snow_Cloak:
			break;
		default:
			if (!is_immune_to_hail(pokemon.types())) {
				heal(generation, pokemon, weather, rational(-1_bi, 16_bi));
			}
			break;
	}
}

void rain_effect(Generation const generation, MutableActivePokemon pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Dry_Skin:
			heal(generation, pokemon, weather, rational(1_bi, 8_bi));
			break;
		case Ability::Hydration:
			pokemon.clear_status();
			break;
		case Ability::Rain_Dish:
			heal(generation, pokemon, weather, rational(1_bi, 16_bi));
			break;
		default:
			break;
	}
}

void sand_effect(Generation const generation, MutableActivePokemon pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Sand_Veil:
			break;
		default:
			if (!is_immune_to_sandstorm(pokemon.types())) {
				heal(generation, pokemon, weather, rational(-1_bi, 16_bi));
			}
	}
}

void sun_effect(Generation const generation, MutableActivePokemon pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Dry_Skin:
			heal(generation, pokemon, weather, rational(1_bi, 8_bi));
			break;
		case Ability::Solar_Power:
			heal(generation, pokemon, weather, rational(-1_bi, 8_bi));
			break;
		default:
			break;
	}
}

void weather_effects(Generation const generation, MutableActivePokemon first, MutableActivePokemon last, Weather & weather) {
	weather.advance_one_turn();
	auto const ability_blocks_weather = weather_is_blocked_by_ability(first.ability(), last.ability());
	for (auto const pokemon : {first, last}) {
		if (weather.hail(ability_blocks_weather)) {
			hail_effect(generation, pokemon, weather);
		} else if (weather.rain(ability_blocks_weather)) {
			rain_effect(generation, pokemon, weather);
		} else if (weather.sand(ability_blocks_weather)) {
			sand_effect(generation, pokemon, weather);
		} else if (weather.sun(ability_blocks_weather)) {
			sun_effect(generation, pokemon, weather);
		}
	}
}

auto handle_curse(Generation const generation, MutableActivePokemon pokemon, Weather const weather) -> void {
	if (!pokemon.is_cursed()) {
		return;
	}
	heal(generation, pokemon, weather, rational(-1_bi, 4_bi));
}

constexpr auto leftovers_healing() {
	return rational(1_bi, 16_bi);
}

auto other_effects(Generation const generation, MutableActivePokemon pokemon, MutableActivePokemon foe, Weather & weather, EndOfTurnFlags const flags) -> void {
	if (get_hp(pokemon) == 0_bi) {
		return;
	}
	if (pokemon.ingrained()) {
		heal(generation, pokemon, weather, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(generation, weather)));
	}
	if (pokemon.aqua_ring_is_active()) {
		heal(generation, pokemon, weather, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(generation, weather)));
	}
	if (!pokemon.switched_in_this_turn() and boosts_speed(pokemon.ability())) {
		pokemon.stage()[StatNames::SPE] += 1_bi;
	} else if (flags.shed_skin) {
		pokemon.clear_status();
	}
	switch (pokemon.item(generation, weather)) {
		case Item::Leftovers:
			heal(generation, pokemon, weather, leftovers_healing());
			break;
		case Item::Black_Sludge:
			heal(generation, pokemon, weather, BOUNDED_CONDITIONAL(is_type(pokemon, Type::Poison), rational(1_bi, 16_bi), rational(-1_bi, 8_bi)));
			break;
		default:
			break;
	}
	// TODO: Not sure if this check for Uproar is in the correct place
	auto const uproar = pokemon.is_uproaring() or foe.is_uproaring();
	pokemon.status_and_leech_seed_effects(generation, foe, weather, uproar);
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
	handle_curse(generation, pokemon, weather);
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

void generation_2_end_of_attack(Team & user, Team & other, Weather const weather) {
	auto const user_pokemon = user.pokemon();
	user_pokemon.status_and_leech_seed_effects(Generation::two, other.pokemon(), weather);
	handle_curse(Generation::two, user_pokemon, weather);
}

void generation_2_end_of_turn(Team & first_team, Team & last_team, Weather & weather) {
	auto const first = first_team.pokemon();
	auto const last = last_team.pokemon();

	// TODO: Future Sight in reverse speed order

	weather.advance_one_turn();
	for (auto const pokemon : {first, last}) {
		if (weather.sand() and !is_immune_to_sandstorm(pokemon.types())) {
			heal(Generation::two, pokemon, weather, rational(-1_bi, 8_bi));
		}
	}

	// TODO: multi-turn attacks

	first.perish_song_turn();
	last.perish_song_turn();

	for (auto const pokemon : {first, last}) {
		if (pokemon.item(Generation::two, weather) == Item::Leftovers) {
			heal(Generation::two, pokemon, weather, leftovers_healing());
		}
	}

	// TODO: Defrost

	first_team.decrement_screens();
	last_team.decrement_screens();

	// TODO: Berry activation?

	first.advance_encore();
	last.advance_encore();
}

void generation_3_plus_end_of_turn(Generation const generation, Team & first_team, EndOfTurnFlags const first_flags, Team & last_team, EndOfTurnFlags const last_flags, Weather & weather) {
	first_team.decrement_screens();
	last_team.decrement_screens();

	first_team.decrement_wish(generation, weather);
	last_team.decrement_wish(generation, weather);

	auto const first = first_team.pokemon();
	auto const last = last_team.pokemon();

	weather_effects(generation, first, last, weather);

	other_effects(generation, first, last, weather, first_flags);
	other_effects(generation, last, first, weather, last_flags);

	// TODO: Doom Desire / Future Sight

	first.perish_song_turn();
	last.perish_song_turn();
}

} // namespace

void end_of_attack(Generation const generation, Team & user, Team & other, Weather const weather) {
	switch (generation) {
		case Generation::one: return;
		case Generation::two: return generation_2_end_of_attack(user, other, weather);
		default: return;
	}
}

void end_of_turn(Generation const generation, Team & first, EndOfTurnFlags const first_flags, Team & last, EndOfTurnFlags const last_flags, Weather & weather) {
	switch (generation) {
		case Generation::one: return;
		case Generation::two: return generation_2_end_of_turn(first, last, weather);
		default: return generation_3_plus_end_of_turn(generation, first, first_flags, last, last_flags, weather);
	}
}

} // namespace technicalmachine
