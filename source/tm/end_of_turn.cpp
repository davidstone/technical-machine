// End of turn effects
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/end_of_turn.hpp>

#include <tm/ability.hpp>
#include <tm/heal.hpp>
#include <tm/known_team.hpp>
#include <tm/rational.hpp>
#include <tm/seen_team.hpp>
#include <tm/status_name.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/pokemon/active_pokemon.hpp>
#include <tm/pokemon/any_pokemon.hpp>
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

void hail_effect(any_mutable_active_pokemon auto const pokemon, Weather const weather) {
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

void rain_effect(any_mutable_active_pokemon auto const pokemon, Weather const weather) {
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

void sand_effect(any_mutable_active_pokemon auto const pokemon, Weather const weather) {
	switch (pokemon.ability()) {
		case Ability::Sand_Veil:
			break;
		default:
			if (!is_immune_to_sandstorm(pokemon.types())) {
				heal(pokemon, weather, rational(-1_bi, 16_bi));
			}
	}
}

void sun_effect(any_mutable_active_pokemon auto const pokemon, Weather const weather) {
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

template<any_mutable_active_pokemon PokemonType>
void weather_effects(PokemonType const first, OtherMutableActivePokemon<PokemonType> const last, Weather & weather) {
	weather.advance_one_turn();
	auto const ability_blocks_weather = weather_is_blocked_by_ability(first.ability(), last.ability());
	auto apply_weather = [&](auto const pokemon) {
		if (weather.hail(ability_blocks_weather)) {
			hail_effect(pokemon, weather);
		} else if (weather.rain(ability_blocks_weather)) {
			rain_effect(pokemon, weather);
		} else if (weather.sand(ability_blocks_weather)) {
			sand_effect(pokemon, weather);
		} else if (weather.sun(ability_blocks_weather)) {
			sun_effect(pokemon, weather);
		}
	};
	apply_weather(first);
	apply_weather(last);
}

constexpr auto leftovers_healing() {
	return rational(1_bi, 16_bi);
}

template<any_team TeamType>
auto other_effects(TeamType & team, OtherMutableActivePokemon<TeamType> const foe, Weather & weather, EndOfTurnFlags const flags) -> void {
	auto pokemon = team.pokemon();
	if (pokemon.hp().current() == 0_bi) {
		return;
	}
	if (pokemon.ingrained()) {
		heal(pokemon, weather, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(weather)));
	}
	if (pokemon.aqua_ring_is_active()) {
		heal(pokemon, weather, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(weather)));
	}
	if (!pokemon.last_used_move().switched_in_this_turn() and boosts_speed(pokemon.ability())) {
		pokemon.stages()[BoostableStat::spe] += 1_bi;
	} else if (flags.shed_skin) {
		pokemon.clear_status();
	}
	switch (pokemon.item(weather)) {
		case Item::Leftovers:
			heal(pokemon, weather, leftovers_healing());
			break;
		case Item::Black_Sludge:
			heal(pokemon, weather, BOUNDED_CONDITIONAL(is_type(pokemon.as_const(), Type::Poison), rational(1_bi, 16_bi), rational(-1_bi, 8_bi)));
			break;
		default:
			break;
	}
	// TODO: Not sure if this check for Uproar is in the correct place
	auto const uproar = pokemon.last_used_move().is_uproaring() or foe.last_used_move().is_uproaring();
	pokemon.status_and_leech_seed_effects(foe, weather, uproar);
	auto set_status = [&](StatusName const status) {
		if (indirect_status_can_apply(status, pokemon.as_const(), weather)) {
			pokemon.set_status(status, weather);
		}
	};
	switch (pokemon.item(weather)) {
		case Item::Flame_Orb:
			set_status(StatusName::burn);
			break;
		case Item::Toxic_Orb:
			set_status(StatusName::toxic);
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
	pokemon.try_to_activate_yawn(weather, uproar, team_has_status(team, StatusName::sleep));
	if (pokemon.item(weather) == Item::Sticky_Barb) {
		heal(pokemon, weather, rational(-1_bi, 8_bi));
	}
}

template<any_team TeamType> requires(generation_from<TeamType> == Generation::two)
void generation_2_end_of_turn(TeamType & first_team, OtherTeam<TeamType> & last_team, Weather & weather) {
	auto const first = first_team.pokemon();
	auto const last = last_team.pokemon();

	// TODO: Future Sight in reverse speed order

	weather.advance_one_turn();
	auto apply_weather = [&](auto const pokemon) {
		if (weather.sand() and !is_immune_to_sandstorm(pokemon.types())) {
			heal(pokemon, weather, rational(-1_bi, 8_bi));
		}
	};
	apply_weather(first);
	apply_weather(last);

	// TODO: multi-turn attacks

	first.perish_song_turn();
	last.perish_song_turn();

	auto apply_leftovers = [&](auto const pokemon) {
		if (pokemon.item(weather) == Item::Leftovers) {
			heal(pokemon, weather, leftovers_healing());
		}
	};
	apply_leftovers(first);
	apply_leftovers(last);

	// TODO: Defrost

	first_team.decrement_screens();
	last_team.decrement_screens();

	// TODO: Berry activation?

	first.advance_encore();
	last.advance_encore();
}

template<any_team TeamType>
void generation_3_plus_end_of_turn(TeamType & first_team, EndOfTurnFlags const first_flags, OtherTeam<TeamType> & last_team, EndOfTurnFlags const last_flags, Weather & weather) {
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

template<any_team TeamType>
void end_of_turn(TeamType & first, EndOfTurnFlags const first_flags, OtherTeam<TeamType> & last, EndOfTurnFlags const last_flags, Weather & weather) {
	constexpr auto generation = generation_from<TeamType>;
	if constexpr (generation == Generation::one) {
	} else if constexpr (generation == Generation::two) {
		generation_2_end_of_turn(first, last, weather);
	} else {
		generation_3_plus_end_of_turn(first, first_flags, last, last_flags, weather);
	}
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(TeamType) \
	template void end_of_turn(TeamType & first, EndOfTurnFlags first_flags, OtherTeam<TeamType> & last, EndOfTurnFlags last_flags, Weather &)

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(Team<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(KnownTeam<generation>); \
	TECHNICALMACHINE_EXPLICIT_INSTANTIATION_IMPL(SeenTeam<generation>)

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

} // namespace technicalmachine
