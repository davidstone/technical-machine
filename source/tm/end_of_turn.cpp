// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.end_of_turn;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.indirect_status_can_apply;
import tm.pokemon.is_type;

import tm.stat.stat_names;

import tm.status.status_name;
import tm.status.team_has_status;

import tm.type.pokemon_types;
import tm.type.type;

import tm.ability;
import tm.any_team;
import tm.end_of_turn_flags;
import tm.environment;
import tm.generation;
import tm.handle_curse;
import tm.heal;
import tm.item;
import tm.rational;
import tm.other_team;
import tm.weather;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

// https://web.archive.org/web/20130530163614/http://www.upokecenter.com/games/gs/guides/timing.php

auto is_immune_to_hail(PokemonTypes const types) -> bool {
	return containers::any_equal(types, Type::Ice);
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

void hail_effect(any_mutable_active_pokemon auto const pokemon, Environment const environment) {
	switch (pokemon.ability()) {
		case Ability::Ice_Body:
			heal(pokemon, environment, rational(1_bi, 16_bi));
			break;
		case Ability::Snow_Cloak:
			break;
		default:
			if (!is_immune_to_hail(pokemon.types())) {
				heal(pokemon, environment, rational(-1_bi, 16_bi));
			}
			break;
	}
}

void rain_effect(any_mutable_active_pokemon auto const pokemon, Environment const environment) {
	switch (pokemon.ability()) {
		case Ability::Dry_Skin:
			heal(pokemon, environment, rational(1_bi, 8_bi));
			break;
		case Ability::Hydration:
			pokemon.clear_status();
			break;
		case Ability::Rain_Dish:
			heal(pokemon, environment, rational(1_bi, 16_bi));
			break;
		default:
			break;
	}
}

void sand_effect(any_mutable_active_pokemon auto const pokemon, Environment const environment) {
	switch (pokemon.ability()) {
		case Ability::Sand_Veil:
			break;
		default:
			if (!is_immune_to_sandstorm(pokemon.types())) {
				heal(pokemon, environment, rational(-1_bi, 16_bi));
			}
	}
}

void sun_effect(any_mutable_active_pokemon auto const pokemon, Environment const environment) {
	switch (pokemon.ability()) {
		case Ability::Dry_Skin:
			heal(pokemon, environment, rational(1_bi, 8_bi));
			break;
		case Ability::Solar_Power:
			heal(pokemon, environment, rational(-1_bi, 8_bi));
			break;
		default:
			break;
	}
}

template<any_mutable_active_pokemon PokemonType>
void environment_effects(PokemonType const first, OtherMutableActivePokemon<PokemonType> const last, Environment & environment) {
	environment.advance_one_turn();
	auto const weather = environment.effective_weather(first.ability(), last.ability());
	auto apply_weather = [&](auto const pokemon) {
		switch (weather) {
			case Weather::clear:
				break;
			case Weather::hail:
				hail_effect(pokemon, environment);
				break;
			case Weather::sand:
				sand_effect(pokemon, environment);
				break;
			case Weather::sun:
				sun_effect(pokemon, environment);
				break;
			case Weather::rain:
				rain_effect(pokemon, environment);
				break;
		}
	};
	apply_weather(first);
	apply_weather(last);
}

constexpr auto leftovers_healing() {
	return rational(1_bi, 16_bi);
}

template<typename TeamType>
using ActivePokemonFromTeam = decltype(bounded::declval<TeamType>().pokemon());

template<any_team TeamType>
auto other_effects(TeamType & team, ActivePokemonFromTeam<OtherTeam<TeamType>> const foe, Environment & environment, EndOfTurnFlags const flags) -> void {
	auto pokemon = team.pokemon();
	if (pokemon.hp().current() == 0_bi) {
		return;
	}
	if (pokemon.ingrained()) {
		heal(pokemon, environment, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(environment)));
	}
	if (pokemon.aqua_ring_is_active()) {
		heal(pokemon, environment, rational(1_bi, 16_bi) * healing_multiplier(pokemon.item(environment)));
	}
	if (!pokemon.last_used_move().switched_in_this_turn() and boosts_speed(pokemon.ability())) {
		pokemon.stages()[BoostableStat::spe] += 1_bi;
	} else if (flags.shed_skin) {
		pokemon.clear_status();
	}
	switch (pokemon.item(environment)) {
		case Item::Leftovers:
			heal(pokemon, environment, leftovers_healing());
			break;
		case Item::Black_Sludge:
			heal(pokemon, environment, BOUNDED_CONDITIONAL(is_type(pokemon.as_const(), Type::Poison), rational(1_bi, 16_bi), rational(-1_bi, 8_bi)));
			break;
		default:
			break;
	}
	// TODO: Not sure if this check for Uproar is in the correct place
	auto const uproar = pokemon.last_used_move().is_uproaring() or foe.last_used_move().is_uproaring();
	pokemon.status_and_leech_seed_effects(foe, environment, uproar);
	auto const weather = environment.effective_weather(pokemon.ability(), foe.ability());
	auto set_status = [&](StatusName const status) {
		if (indirect_status_can_apply(status, pokemon.as_const(), weather)) {
			pokemon.set_status(status, environment);
		}
	};
	switch (pokemon.item(environment)) {
		case Item::Flame_Orb:
			set_status(StatusName::burn);
			break;
		case Item::Toxic_Orb:
			set_status(StatusName::toxic);
			break;
		default:
			break;
	}
	handle_curse(pokemon, environment);
	pokemon.partial_trap_damage(environment);

	pokemon.advance_lock_in(flags.lock_in_ends, environment);

	pokemon.advance_disable();
	pokemon.advance_encore();
	pokemon.advance_taunt();
	pokemon.advance_magnet_rise();
	pokemon.advance_heal_block();
	pokemon.advance_embargo();
	pokemon.try_to_activate_yawn(environment, weather, uproar, team_has_status(team, StatusName::sleep));
	if (pokemon.item(environment) == Item::Sticky_Barb) {
		heal(pokemon, environment, rational(-1_bi, 8_bi));
	}
}

template<any_team TeamType> requires(generation_from<TeamType> == Generation::two)
void generation_2_end_of_turn(TeamType & first_team, EndOfTurnFlags const first_flags, OtherTeam<TeamType> & last_team, EndOfTurnFlags const last_flags, Environment & environment) {
	auto const first = first_team.pokemon();
	auto const last = last_team.pokemon();

	// TODO: Future Sight in reverse speed order

	environment.advance_one_turn();
	auto const weather = environment.effective_weather(first.ability(), last.ability());
	auto apply_environment = [&](auto const pokemon) {
		if (weather == Weather::sand and !is_immune_to_sandstorm(pokemon.types())) {
			heal(pokemon, environment, rational(-1_bi, 8_bi));
		}
	};
	apply_environment(first);
	apply_environment(last);

	// TODO: multi-turn attacks

	first.perish_song_turn();
	last.perish_song_turn();

	auto apply_leftovers = [&](auto const pokemon) {
		if (pokemon.item(environment) == Item::Leftovers) {
			heal(pokemon, environment, leftovers_healing());
		}
	};
	apply_leftovers(first);
	apply_leftovers(last);

	if (first_flags.thaws) {
		first_team.pokemon().clear_status();
	}
	if (last_flags.thaws) {
		last_team.pokemon().clear_status();
	}

	first_team.decrement_screens();
	last_team.decrement_screens();

	// TODO: Berry activation?

	first.advance_encore();
	last.advance_encore();
}

template<any_team TeamType>
void generation_3_plus_end_of_turn(TeamType & first_team, EndOfTurnFlags const first_flags, OtherTeam<TeamType> & last_team, EndOfTurnFlags const last_flags, Environment & environment) {
	first_team.decrement_screens();
	last_team.decrement_screens();

	first_team.decrement_wish(environment);
	last_team.decrement_wish(environment);

	auto const first = first_team.pokemon();
	auto const last = last_team.pokemon();

	environment_effects(first, last, environment);

	other_effects(first_team, last, environment, first_flags);
	other_effects(last_team, first, environment, last_flags);

	// TODO: Doom Desire / Future Sight

	first.perish_song_turn();
	last.perish_song_turn();
}

export template<any_team TeamType>
void end_of_turn(TeamType & first, EndOfTurnFlags const first_flags, OtherTeam<TeamType> & last, EndOfTurnFlags const last_flags, Environment & environment) {
	constexpr auto generation = generation_from<TeamType>;
	if constexpr (generation == Generation::one) {
	} else if constexpr (generation == Generation::two) {
		generation_2_end_of_turn(first, first_flags, last, last_flags, environment);
	} else {
		generation_3_plus_end_of_turn(first, first_flags, last, last_flags, environment);
	}
	first.reset_end_of_turn();
	last.reset_end_of_turn();
}

} // namespace technicalmachine
