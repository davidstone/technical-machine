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

template<Generation generation>
void hail_effect(MutableActivePokemon<generation> pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Ice_Body:
			heal(pokemon, weather, rational(1_bi, 16_bi));
			break;
		case Ability::Snow_Cloak:
			break;
		default:
			if (!is_immune_to_hail(pokemon.types())) {
				heal(pokemon, weather, rational(-1_bi, 16_bi));
			}
			break;
	}
}

template<Generation generation>
void rain_effect(MutableActivePokemon<generation> pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Dry_Skin:
			heal(pokemon, weather, rational(1_bi, 8_bi));
			break;
		case Ability::Hydration:
			pokemon.clear_status();
			break;
		case Ability::Rain_Dish:
			heal(pokemon, weather, rational(1_bi, 16_bi));
			break;
		default:
			break;
	}
}

template<Generation generation>
void sand_effect(MutableActivePokemon<generation> pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Sand_Veil:
			break;
		default:
			if (!is_immune_to_sandstorm(pokemon.types())) {
				heal(pokemon, weather, rational(-1_bi, 16_bi));
			}
	}
}

template<Generation generation>
void sun_effect(MutableActivePokemon<generation> pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Dry_Skin:
			heal(pokemon, weather, rational(1_bi, 8_bi));
			break;
		case Ability::Solar_Power:
			heal(pokemon, weather, rational(-1_bi, 8_bi));
			break;
		default:
			break;
	}
}

template<Generation generation>
void weather_effects(MutableActivePokemon<generation> first, MutableActivePokemon<generation> last, Weather & weather) {
	weather.advance_one_turn();
	auto const ability_blocks_weather = weather_is_blocked_by_ability(first.ability(), last.ability());
	for (auto const pokemon : {first, last}) {
		if (weather.hail(ability_blocks_weather)) {
			hail_effect(pokemon, weather);
		} else if (weather.rain(ability_blocks_weather)) {
			rain_effect(pokemon, weather);
		} else if (weather.sand(ability_blocks_weather)) {
			sand_effect(pokemon, weather);
		} else if (weather.sun(ability_blocks_weather)) {
			sun_effect(pokemon, weather);
		}
	}
}

template<Generation generation>
auto handle_curse(MutableActivePokemon<generation> pokemon, Weather const weather) -> void {
	if (!pokemon.is_cursed()) {
		return;
	}
	heal(pokemon, weather, rational(-1_bi, 4_bi));
}

constexpr auto leftovers_healing() {
	return rational(1_bi, 16_bi);
}

template<Generation generation>
auto other_effects(Team<generation> & team, MutableActivePokemon<generation> foe, Weather & weather, EndOfTurnFlags const flags) -> void {
	auto pokemon = team.pokemon();
	if (pokemon.hp() == 0_bi) {
		return;
	}
	if (pokemon.ingrained()) {
		heal(pokemon, weather, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(weather)));
	}
	if (pokemon.aqua_ring_is_active()) {
		heal(pokemon, weather, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(weather)));
	}
	if (!pokemon.switched_in_this_turn() and boosts_speed(pokemon.ability())) {
		pokemon.stage()[BoostableStat::spe] += 1_bi;
	} else if (flags.shed_skin) {
		pokemon.clear_status();
	}
	switch (pokemon.item(weather)) {
		case Item::Leftovers:
			heal(pokemon, weather, leftovers_healing());
			break;
		case Item::Black_Sludge:
			heal(pokemon, weather, BOUNDED_CONDITIONAL(is_type(as_const(pokemon), Type::Poison), rational(1_bi, 16_bi), rational(-1_bi, 8_bi)));
			break;
		default:
			break;
	}
	// TODO: Not sure if this check for Uproar is in the correct place
	auto const uproar = pokemon.is_uproaring() or foe.is_uproaring();
	pokemon.status_and_leech_seed_effects(foe, weather, uproar);
	auto set_status = [&](Statuses const status) {
		if (non_sleep_status_can_apply(status, as_const(pokemon), as_const(pokemon), weather)) {
			pokemon.set_status(status, weather);
		}
	};
	switch (pokemon.item(weather)) {
		case Item::Flame_Orb:
			set_status(Statuses::burn);
			break;
		case Item::Toxic_Orb:
			set_status(Statuses::toxic);
			break;
		default:
			break;
	}
	handle_curse(pokemon, weather);
	pokemon.partial_trap_damage(weather);
	
	pokemon.advance_lock_in(flags.lock_in_ends, weather);
	
	pokemon.advance_disable();
	pokemon.advance_encore();
	pokemon.advance_taunt();
	pokemon.advance_magnet_rise();
	pokemon.advance_heal_block();
	pokemon.advance_embargo();
	pokemon.try_to_activate_yawn(weather, uproar, team_has_status(team, Statuses::sleep));
	if (pokemon.item(weather) == Item::Sticky_Barb) {
		heal(pokemon, weather, rational(-1_bi, 8_bi));
	}
}

void generation_2_end_of_turn(Team<Generation::two> & first_team, Team<Generation::two> & last_team, Weather & weather) {
	auto const first = first_team.pokemon();
	auto const last = last_team.pokemon();

	// TODO: Future Sight in reverse speed order

	weather.advance_one_turn();
	for (auto const pokemon : {first, last}) {
		if (weather.sand() and !is_immune_to_sandstorm(pokemon.types())) {
			heal(pokemon, weather, rational(-1_bi, 8_bi));
		}
	}

	// TODO: multi-turn attacks

	first.perish_song_turn();
	last.perish_song_turn();

	for (auto const pokemon : {first, last}) {
		if (pokemon.item(weather) == Item::Leftovers) {
			heal(pokemon, weather, leftovers_healing());
		}
	}

	// TODO: Defrost

	first_team.decrement_screens();
	last_team.decrement_screens();

	// TODO: Berry activation?

	first.advance_encore();
	last.advance_encore();
}

template<Generation generation>
void generation_3_plus_end_of_turn(Team<generation> & first_team, EndOfTurnFlags const first_flags, Team<generation> & last_team, EndOfTurnFlags const last_flags, Weather & weather) {
	first_team.decrement_screens();
	last_team.decrement_screens();

	first_team.decrement_wish(weather);
	last_team.decrement_wish(weather);

	auto const first = first_team.pokemon();
	auto const last = last_team.pokemon();

	weather_effects(first, last, weather);

	other_effects(first_team, last, weather, first_flags);
	other_effects(last_team, first, weather, last_flags);

	// TODO: Doom Desire / Future Sight

	first.perish_song_turn();
	last.perish_song_turn();
}

} // namespace

void end_of_attack(Team<Generation::two> & user, Team<Generation::two> & other, Weather const weather) {
	auto const user_pokemon = user.pokemon();
	user_pokemon.status_and_leech_seed_effects(other.pokemon(), weather);
	handle_curse(user_pokemon, weather);
}

template<Generation generation>
void end_of_turn(Team<generation> & first, EndOfTurnFlags const first_flags, Team<generation> & last, EndOfTurnFlags const last_flags, Weather & weather) {
	if constexpr (generation == Generation::one) {
	} else if constexpr (generation == Generation::two) {
		generation_2_end_of_turn(first, last, weather);
	} else {
		generation_3_plus_end_of_turn(first, first_flags, last, last_flags, weather);
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template void end_of_turn<generation>(Team<generation> & first, EndOfTurnFlags const first_flags, Team<generation> & last, EndOfTurnFlags const last_flags, Weather & weather)

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine
