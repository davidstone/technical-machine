// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.move.calculate_damage;

import tm.move.affects_target;
import tm.move.category;
import tm.move.damage_type;
import tm.move.executed_move;
import tm.move.is_self_ko;
import tm.move.known_move;
import tm.move.move_name;
import tm.move.other_action;
import tm.move.power;

import tm.pokemon.any_pokemon;
import tm.pokemon.is_type;

import tm.stat.calculate;

import tm.status.status_name;

import tm.type.effectiveness;
import tm.type.type;

import tm.ability;
import tm.any_team;
import tm.associated_team;
import tm.environment;
import tm.generation;
import tm.item;
import tm.rational;
import tm.weather;

import bounded;

namespace technicalmachine {

using namespace bounded::literal;

template<any_team DefenderTeam>
constexpr auto reflect_is_active(KnownMove const move, DefenderTeam const & defender) {
	return defender.reflect() and is_physical(generation_from<DefenderTeam>, move);
}

template<any_team DefenderTeam>
constexpr auto light_screen_is_active(KnownMove const move, DefenderTeam const & defender) {
	return defender.light_screen() and is_special(generation_from<DefenderTeam>, move);
}

template<any_team AttackerTeam, any_team DefenderTeam>
constexpr auto screen_is_active(ExecutedMove<AttackerTeam> const executed, DefenderTeam const & defender) {
	return !executed.critical_hit and (reflect_is_active(executed.move, defender) or light_screen_is_active(executed.move, defender));
}

using WeatherModifier = rational<
	bounded::integer<1, 3>,
	bounded::constant_t<2>
>;
constexpr auto calculate_weather_modifier(Type const type, Weather const weather) -> WeatherModifier {
	constexpr auto boost = WeatherModifier(3_bi, 2_bi);
	constexpr auto neutral = WeatherModifier(2_bi, 2_bi);
	constexpr auto weaken = WeatherModifier(1_bi, 2_bi);
	switch (weather) {
		case Weather::clear:
		case Weather::hail:
		case Weather::sand:
			return neutral;
		case Weather::sun:
			switch (type) {
				case Type::Fire: return boost;
				case Type::Water: return weaken;
				default: return neutral;
			}
		case Weather::rain:
			switch (type) {
				case Type::Fire: return weaken;
				case Type::Water: return boost;
				default: return neutral;
			}
	}
}

constexpr auto is_boosted_by_flash_fire(Type const type) {
	return type == Type::Fire;
}

constexpr auto calculate_flash_fire_modifier(any_active_pokemon auto const attacker, Type const move_type) {
	return BOUNDED_CONDITIONAL(attacker.flash_fire_is_active() and is_boosted_by_flash_fire(move_type),
		rational(3_bi, 2_bi),
		rational(1_bi, 1_bi)
	);
}

using ItemModifier = rational<bounded::integer<10, 20>, bounded::integer<10, 10>>;
template<any_active_pokemon PokemonType>
constexpr auto calculate_item_modifier(PokemonType const attacker, Environment const environment) -> ItemModifier {
	switch (attacker.item(environment)) {
		case Item::Life_Orb:
			return rational(13_bi, 10_bi);
		case Item::Metronome:
			return attacker.last_used_move().metronome_boost(generation_from<PokemonType>);
		default:
			return rational(10_bi, 10_bi);
	}
}

constexpr auto calculate_me_first_modifier(any_active_pokemon auto const attacker) {
	return BOUNDED_CONDITIONAL(attacker.me_first_is_active(),
		rational(3_bi, 2_bi),
		rational(1_bi, 1_bi)
	);
}


constexpr auto calculate_stab_boost(Ability const ability) {
	return BOUNDED_CONDITIONAL(boosts_stab(ability),
		rational(2_bi, 1_bi),
		rational(3_bi, 2_bi)
	);
}

constexpr auto calculate_stab_modifier(any_active_pokemon auto const attacker, Type const move_type) {
	return BOUNDED_CONDITIONAL(is_type(attacker, move_type) and move_type != Type::Typeless,
		calculate_stab_boost(attacker.ability()),
		rational(1_bi, 1_bi)
	);
}

constexpr auto calculate_ability_effectiveness_modifier(Ability const ability, Effectiveness const & effectiveness) {
	return BOUNDED_CONDITIONAL(weakens_se_attacks(ability) and effectiveness.is_super_effective(),
		rational(3_bi, 4_bi),
		rational(1_bi, 1_bi)
	);
}

constexpr auto calculate_expert_belt_modifier(Item const item, Effectiveness const & effectiveness) {
	return BOUNDED_CONDITIONAL(boosts_super_effective_moves(item) and effectiveness.is_super_effective(),
		rational(6_bi, 5_bi),
		rational(1_bi, 1_bi)
	);
}


constexpr auto level_multiplier(any_active_pokemon auto const attacker) {
	return attacker.level()() * 2_bi / 5_bi;
}

constexpr auto weakening_from_status(any_active_pokemon auto const attacker) {
	return BOUNDED_CONDITIONAL(
		attacker.status().name() == StatusName::burn and blocks_burn_damage_penalty(attacker.ability()),
		2_bi,
		1_bi
	);
}

template<any_active_pokemon UserPokemon>
constexpr auto physical_vs_special_modifier(UserPokemon const attacker, ExecutedMove<AssociatedTeam<UserPokemon>> const executed, any_active_pokemon auto const defender, Environment const environment) {
	constexpr auto generation = generation_from<UserPokemon>;
	auto const attacker_ability = attacker.ability();
	auto const defender_ability = defender.ability();
	// For all integers a, b, and c:
	// (a / b) / c == a / (b * c)
	// See: http://math.stackexchange.com/questions/147771/rewriting-repeated-integer-division-with-multiplication
	return BOUNDED_CONDITIONAL(is_physical(generation, executed.move),
		rational(
			calculate_attack(attacker, executed.move.type, defender_ability, environment, executed.critical_hit),
			50_bi * calculate_defense(defender, environment, is_self_KO(executed.move.name), executed.critical_hit) * weakening_from_status(attacker)
		),
		rational(
			calculate_special_attack(attacker, executed.move.type, defender_ability, environment, executed.critical_hit),
			50_bi * calculate_special_defense(defender, attacker_ability, environment, executed.critical_hit)
		)
	);
}

template<any_team AttackerTeam, any_team DefenderTeam>
constexpr auto screen_divisor(ExecutedMove<AttackerTeam> const move, DefenderTeam const & defender) {
	return BOUNDED_CONDITIONAL(screen_is_active(move, defender), 2_bi, 1_bi);
}

constexpr auto critical_hit_multiplier(Generation const generation, Ability const ability, bool const critical_hit) {
	auto const boosted = ability == Ability::Sniper;
	return BOUNDED_CONDITIONAL(!critical_hit,
		rational(1_bi, 1_bi),
		BOUNDED_CONDITIONAL(generation >= Generation::six,
			BOUNDED_CONDITIONAL(boosted, rational(9_bi, 4_bi), rational(3_bi, 2_bi)),
			BOUNDED_CONDITIONAL(boosted, rational(3_bi, 1_bi), rational(2_bi, 1_bi))
		)
	);
}

constexpr auto tinted_lens_multiplier(Ability const ability, Effectiveness const & effectiveness) {
	return BOUNDED_CONDITIONAL(
		strengthens_nve_attacks(ability) and effectiveness.is_not_very_effective(),
		2_bi,
		1_bi
	);
}

constexpr auto resistance_berry_divisor(bool const move_weakened_from_item) {
	return BOUNDED_CONDITIONAL(move_weakened_from_item, 2_bi, 1_bi);
}

template<any_team UserTeam, any_team DefenderTeam>
constexpr auto regular_damage(UserTeam const & attacker_team, ExecutedMove<UserTeam> const executed, bool const move_weakened_from_item, DefenderTeam const & defender_team, Environment const environment) {
	constexpr auto generation = generation_from<UserTeam>;
	auto const attacker = attacker_team.pokemon();
	auto const attacker_ability = attacker.ability();
	auto const defender = defender_team.pokemon();
	auto const defender_ability = defender.ability();
	auto const effectiveness = Effectiveness(generation, executed.move.type, defender.types());
	auto const weather = environment.effective_weather(attacker_ability, defender_ability);

	auto const temp =
		(level_multiplier(attacker) + 2_bi) *
		move_power(attacker_team, executed, defender_team, environment) *
		physical_vs_special_modifier(attacker, executed, defender, environment) /
		screen_divisor(executed, defender_team) *
		calculate_weather_modifier(executed.move.type, weather) *
		calculate_flash_fire_modifier(attacker, executed.move.type) +
		2_bi;

	return bounded::max(1_bi, temp *
		critical_hit_multiplier(generation, attacker_ability, executed.critical_hit) *
		calculate_item_modifier(attacker, environment) *
		calculate_me_first_modifier(attacker) *
		// random_damage *
		calculate_stab_modifier(attacker, executed.move.type) *
		effectiveness *
		calculate_ability_effectiveness_modifier(defender_ability, effectiveness) *
		calculate_expert_belt_modifier(attacker.item(environment), effectiveness) *
		tinted_lens_multiplier(attacker_ability, effectiveness) /
		resistance_berry_divisor(move_weakened_from_item)
	);
}

template<any_team UserTeam, any_team OtherTeamType>
constexpr auto raw_damage(UserTeam const & attacker_team, ExecutedMove<UserTeam> const executed, bool const move_weakened_from_item, OtherTeamType const & defender_team, OtherAction const defender_action, Environment const environment) -> damage_type {
	constexpr auto generation = generation_from<UserTeam>;
	auto const attacker = attacker_team.pokemon();
	auto const defender = defender_team.pokemon();
	auto regular = [&] -> damage_type {
		return bounded::assume_in_range<damage_type>(regular_damage(
			attacker_team,
			executed,
			move_weakened_from_item,
			defender_team,
			environment
		));
	};
	switch (executed.move.name) {
		case MoveName::Bide:
			// TODO: Determine the damage here
			return 0_bi;
		case MoveName::Bind:
		case MoveName::Clamp:
		case MoveName::Fire_Spin:
		case MoveName::Wrap:
			return generation == Generation::one and attacker.last_used_move().is_immobilizing() ?
				defender.direct_damage_received() :
				regular();
		case MoveName::Counter:
			return defender_action.is_counterable(generation) ? attacker.direct_damage_received() * 2_bi : 0_bi;
		case MoveName::Dragon_Rage:
			return 40_bi;
		case MoveName::Endeavor:
			return bounded::max(defender.hp().current() - attacker.hp().current(), 0_bi);
		case MoveName::Final_Gambit:
			return attacker.hp().current();
		case MoveName::Fissure:
		case MoveName::Guillotine:
		case MoveName::Horn_Drill:
		case MoveName::Sheer_Cold:
			return BOUNDED_CONDITIONAL(defender.ability() == Ability::Sturdy, 0_bi, defender.hp().max());
		case MoveName::Metal_Burst:
			return attacker.direct_damage_received() * 3_bi / 2_bi;
		case MoveName::Mirror_Coat:
			return defender_action.is_mirror_coatable(generation) ? attacker.direct_damage_received() * 2_bi : 0_bi;
		case MoveName::Night_Shade:
		case MoveName::Seismic_Toss:
			return attacker.level()();
		case MoveName::Psywave:
			return attacker.level()();
		case MoveName::Sonic_Boom:
			return 20_bi;
		case MoveName::Super_Fang:
			return defender.hp().current() / 2_bi;
		default:
			return regular();
	}
}

export template<any_team UserTeam, any_team OtherTeamType>
constexpr auto calculate_damage(UserTeam const & attacker, ExecutedMove<UserTeam> const executed, bool const move_weakened_from_item, OtherTeamType const & defender, OtherAction const defender_action, Environment const environment) -> damage_type {
	return affects_target(executed.move, defender.pokemon(), environment) ?
		raw_damage(attacker, executed, move_weakened_from_item, defender, defender_action, environment) :
		0_bi;
}

} // namespace technicalmachine
