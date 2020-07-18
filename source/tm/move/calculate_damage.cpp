// Copyright (C) 2019 David Stone
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

#include <tm/move/calculate_damage.hpp>

#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/item.hpp>
#include <tm/status.hpp>
#include <tm/random_damage.hpp>
#include <tm/rational.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/move/category.hpp>
#include <tm/move/executed_move.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/power.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/calculate.hpp>

#include <tm/type/effectiveness.hpp>

namespace technicalmachine {

using namespace bounded::literal;

namespace {

template<Generation generation>
auto reflect_is_active(KnownMove const move, Team<generation> const & defender) {
	return defender.reflect() and is_physical(generation, move);
}

template<Generation generation>
auto light_screen_is_active(KnownMove const move, Team<generation> const & defender) {
	return defender.light_screen() and is_special(generation, move);
}

template<Generation generation>
auto screen_is_active(ExecutedMove const executed, Team<generation> const & defender) {
	return !executed.critical_hit and (reflect_is_active(executed.move, defender) or light_screen_is_active(executed.move, defender));
}

constexpr auto is_strengthened_by_weather(Type const type, Weather const weather, bool const ability_blocks_weather) -> bool {
	return
		(weather.rain(ability_blocks_weather) and type == Type::Water) or
		(weather.sun(ability_blocks_weather) and type == Type::Fire);
}

constexpr auto is_weakened_by_weather(Type const type, Weather const weather, bool const ability_blocks_weather) -> bool {
	return
		(weather.rain(ability_blocks_weather) and type == Type::Fire) or
		(weather.sun(ability_blocks_weather) and type == Type::Water);
}


auto calculate_weather_modifier(Type const type, Weather const weather, bool const ability_blocks_weather) {
	return
		BOUNDED_CONDITIONAL(is_strengthened_by_weather(type, weather, ability_blocks_weather), rational(3_bi, 2_bi),
		BOUNDED_CONDITIONAL(is_weakened_by_weather(type, weather, ability_blocks_weather), rational(1_bi, 2_bi),
		rational(1_bi, 1_bi)
	));
}

template<Generation generation>
auto calculate_flash_fire_modifier(ActivePokemon<generation> const attacker, Type const move_type) {
	return BOUNDED_CONDITIONAL(attacker.flash_fire_is_active() and is_boosted_by_flash_fire(move_type),
		rational(3_bi, 2_bi),
		rational(1_bi, 1_bi)
	);
}

using ItemModifier = rational<bounded::integer<10, 20>, bounded::integer<10, 10>>;
template<Generation generation>
auto calculate_item_modifier(ActivePokemon<generation> const attacker, Weather const weather) -> ItemModifier {
	switch (attacker.item(weather)) {
		case Item::Life_Orb:
			return rational(13_bi, 10_bi);
		case Item::Metronome:
			return attacker.last_used_move().metronome_boost(generation);
		default:
			return rational(10_bi, 10_bi);
	}
}

template<Generation generation>
auto calculate_me_first_modifier(ActivePokemon<generation> const attacker) {
	return BOUNDED_CONDITIONAL(attacker.me_first_is_active(),
		rational(3_bi, 2_bi),
		rational(1_bi, 1_bi)
	);
}


auto calculate_stab_boost(Ability const ability) {
	return BOUNDED_CONDITIONAL(boosts_stab(ability),
		rational(2_bi, 1_bi),
		rational(3_bi, 2_bi)
	);
}

template<Generation generation>
auto calculate_stab_modifier(ActivePokemon<generation> const attacker, Type const move_type) {
	return BOUNDED_CONDITIONAL(is_type(attacker, move_type) and move_type != Type::Typeless,
		calculate_stab_boost(attacker.ability()),
		rational(1_bi, 1_bi)
	);
}

auto calculate_ability_effectiveness_modifier(Ability const ability, Effectiveness const & effectiveness) {
	return BOUNDED_CONDITIONAL(weakens_se_attacks(ability) and effectiveness.is_super_effective(),
		rational(3_bi, 4_bi),
		rational(1_bi, 1_bi)
	);
}

auto calculate_expert_belt_modifier(Item const item, Effectiveness const & effectiveness) {
	return BOUNDED_CONDITIONAL(boosts_super_effective_moves(item) and effectiveness.is_super_effective(),
		rational(6_bi, 5_bi),
		rational(1_bi, 1_bi)
	);
}


template<Generation generation>
auto level_multiplier(ActivePokemon<generation> const attacker) {
	return attacker.level()() * 2_bi / 5_bi;
}

template<Generation generation>
auto weakening_from_status(ActivePokemon<generation> const attacker) {
	return BOUNDED_CONDITIONAL(
		weakens_physical_attacks(attacker.status()) and blocks_burn_damage_penalty(attacker.ability()),
		2_bi,
		1_bi
	);
}

template<Generation generation>
auto physical_vs_special_modifier(ActivePokemon<generation> const attacker, ExecutedMove const executed, ActivePokemon<generation> const defender, Weather const weather) {
	auto const attacker_ability = attacker.ability();
	auto const defender_ability = defender.ability();
	// For all integers a, b, and c:
	// (a / b) / c == a / (b * c)
	// See: http://math.stackexchange.com/questions/147771/rewriting-repeated-integer-division-with-multiplication
	return BOUNDED_CONDITIONAL(is_physical(generation, executed.move),
		rational(
			calculate_attack(attacker, executed.move.type, defender_ability, weather, executed.critical_hit),
			50_bi * calculate_defense(defender, executed.move.name, weather, executed.critical_hit) * weakening_from_status(attacker)
		),
		rational(
			calculate_special_attack(attacker, executed.move.type, defender_ability, weather, executed.critical_hit),
			50_bi * calculate_special_defense(defender, attacker_ability, weather, executed.critical_hit)
		)
	);
}

template<Generation generation>
auto screen_divisor(ExecutedMove const move, Team<generation> const & defender) {
	return BOUNDED_CONDITIONAL(screen_is_active(move, defender), 2_bi, 1_bi);
}

auto critical_hit_multiplier(Generation const generation, Ability const ability, bool const critical_hit) {
	auto const boosted = ability == Ability::Sniper;
	return BOUNDED_CONDITIONAL(!critical_hit,
		rational(1_bi, 1_bi),
		BOUNDED_CONDITIONAL(generation >= Generation::six,
			BOUNDED_CONDITIONAL(boosted, rational(9_bi, 4_bi), rational(3_bi, 2_bi)),
			BOUNDED_CONDITIONAL(boosted, rational(3_bi, 1_bi), rational(2_bi, 1_bi))
		)
	);
}

auto tinted_lens_multiplier(Ability const ability, Effectiveness const & effectiveness) {
	return BOUNDED_CONDITIONAL(
		strengthens_nve_attacks(ability) and effectiveness.is_not_very_effective(),
		2_bi,
		1_bi
	);
}

auto resistance_berry_divisor(bool const move_weakened_from_item) {
	return BOUNDED_CONDITIONAL(move_weakened_from_item, 2_bi, 1_bi);
}

template<Generation generation>
auto regular_damage(Team<generation> const & attacker_team, ExecutedMove const executed, bool const move_weakened_from_item, Team<generation> const & defender_team, Weather const weather) {
	auto const attacker = attacker_team.pokemon();
	auto const attacker_ability = attacker.ability();
	auto const defender = defender_team.pokemon();
	auto const effectiveness = Effectiveness(generation, executed.move.type, defender.types());

	auto const temp =
		(level_multiplier(attacker) + 2_bi) *
		move_power(attacker_team, executed, defender_team, weather) *
		physical_vs_special_modifier(attacker, executed, defender, weather) /
		screen_divisor(executed, defender_team) *
		calculate_weather_modifier(executed.move.type, weather, weather_is_blocked_by_ability(attacker_ability, defender.ability())) *
		calculate_flash_fire_modifier(attacker, executed.move.type) +
		2_bi;

	return bounded::max(1_bi, temp *
		critical_hit_multiplier(generation, attacker_ability, executed.critical_hit) *
		calculate_item_modifier(attacker, weather) *
		calculate_me_first_modifier(attacker) *
		RandomDamage()() *
		calculate_stab_modifier(attacker, executed.move.type) *
		effectiveness *
		calculate_ability_effectiveness_modifier(defender.ability(), effectiveness) *
		calculate_expert_belt_modifier(attacker.item(weather), effectiveness) *
		tinted_lens_multiplier(attacker_ability, effectiveness) /
		resistance_berry_divisor(move_weakened_from_item)
	);
}

template<Generation generation>
auto raw_damage(Team<generation> const & attacker_team, ExecutedMove const executed, bool const move_weakened_from_item, Team<generation> const & defender_team, OtherMove const defender_move, Weather const weather) -> damage_type {
	auto const attacker = attacker_team.pokemon();
	auto const defender = defender_team.pokemon();
	switch (executed.move.name) {
		case Moves::Bide:
			// TODO: Determine the damage here
			return 0_bi;
		case Moves::Counter:
			return defender_move.used_move_is_physical(generation) ? attacker.direct_damage_received() * 2_bi : 0_bi;
		case Moves::Dragon_Rage:
			return 40_bi;
		case Moves::Endeavor:
			return bounded::max(defender.hp().current() - attacker.hp().current(), 0_bi);
		case Moves::Final_Gambit:
			return attacker.hp().current();
		case Moves::Fissure:
		case Moves::Guillotine:
		case Moves::Horn_Drill:
		case Moves::Sheer_Cold:
			return BOUNDED_CONDITIONAL(defender.ability() == Ability::Sturdy, 0_bi, defender.hp().max());
		case Moves::Metal_Burst:
			return attacker.direct_damage_received() * 3_bi / 2_bi;
		case Moves::Mirror_Coat:
			return defender_move.used_move_is_special(generation) ? attacker.direct_damage_received() * 2_bi : 0_bi;
		case Moves::Night_Shade:
		case Moves::Seismic_Toss:
			return attacker.level()();
		case Moves::Psywave:
			return executed.variable.psywave_damage(generation, attacker.level());
		case Moves::Sonic_Boom:
			return 20_bi;
		case Moves::Super_Fang:
			return defender.hp().current() / 2_bi;
		default:
			return static_cast<damage_type>(regular_damage(
				attacker_team,
				executed,
				move_weakened_from_item,
				defender_team,
				weather
			));
	}
}

}	// namespace

template<Generation generation>
auto calculate_damage(Team<generation> const & attacker, ExecutedMove const executed, bool const move_weakened_from_item, Team<generation> const & defender, OtherMove const defender_move, Weather const weather) -> damage_type {
	return affects_target(executed.move, defender.pokemon(), weather) ?
		raw_damage(attacker, executed, move_weakened_from_item, defender, defender_move, weather) :
		static_cast<damage_type>(0_bi);
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto calculate_damage<generation>(Team<generation> const & attacker, ExecutedMove const executed, bool const move_weakened_from_item, Team<generation> const & defender, OtherMove const defender_move, Weather const weather) -> damage_type

TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXPLICIT_INSTANTIATION(Generation::eight);

}	// namespace technicalmachine
