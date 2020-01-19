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
#include <tm/item.hpp>
#include <tm/status.hpp>
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
enum class Generation : std::uint8_t;

namespace {

bool affects_target(Generation const generation, Type const & move_type, ActivePokemon const target, Weather const weather) {
	return
		!Effectiveness(generation, move_type, target.types()).has_no_effect() and
		(move_type != Type::Ground or grounded(generation, target, weather));
}

auto reflect_is_active(Generation const generation, KnownMove const move, Team const & defender) {
	return defender.reflect() and is_physical(generation, move);
}

auto light_screen_is_active(Generation const generation, KnownMove const move, Team const & defender) {
	return defender.light_screen() and is_special(generation, move);
}

auto screen_is_active(Generation const generation, ExecutedMove const executed, Team const & defender) {
	return !executed.critical_hit and (reflect_is_active(generation, executed.move, defender) or light_screen_is_active(generation, executed.move, defender));
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

auto calculate_flash_fire_modifier(ActivePokemon const attacker, Type const move_type) {
	return BOUNDED_CONDITIONAL(attacker.flash_fire_is_active() and is_boosted_by_flash_fire(move_type),
		rational(3_bi, 2_bi),
		rational(1_bi, 1_bi)
	);
}

using ItemModifier = rational<bounded::integer<10, 20>, bounded::integer<10, 10>>;
auto calculate_item_modifier(Generation const generation, ActivePokemon const attacker, Weather const weather) -> ItemModifier {
	switch (attacker.item(generation, weather)) {
		case Item::Life_Orb:
			return rational(13_bi, 10_bi);
		case Item::Metronome:
			return attacker.last_used_move().metronome_boost(generation);
		default:
			return rational(10_bi, 10_bi);
	}
}

auto calculate_me_first_modifier(ActivePokemon const attacker) {
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

auto calculate_stab_modifier(ActivePokemon const attacker, Type const move_type) {
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


auto level_multiplier(Pokemon const & attacker) -> decltype(get_level(attacker)() * 2_bi / 5_bi) {
	return get_level(attacker)() * 2_bi / 5_bi;
}

auto weakening_from_status(ActivePokemon const attacker) {
	return BOUNDED_CONDITIONAL(
		weakens_physical_attacks(get_status(attacker)) and blocks_burn_damage_penalty(attacker.ability()),
		2_bi,
		1_bi
	);
}

auto physical_vs_special_modifier(Generation const generation, ActivePokemon const attacker, ExecutedMove const executed, ActivePokemon const defender, Weather const weather) {
	auto const attacker_ability = attacker.ability();
	auto const defender_ability = defender.ability();
	// For all integers a, b, and c:
	// (a / b) / c == a / (b * c)
	// See: http://math.stackexchange.com/questions/147771/rewriting-repeated-integer-division-with-multiplication
	return BOUNDED_CONDITIONAL(is_physical(generation, executed.move),
		rational(
			calculate_attack(generation, attacker, executed.move.type, defender_ability, weather, executed.critical_hit),
			50_bi * calculate_defense(generation, defender, executed.move.name, weather, executed.critical_hit) * weakening_from_status(attacker)
		),
		rational(
			calculate_special_attack(generation, attacker, executed.move.type, defender_ability, weather, executed.critical_hit),
			50_bi * calculate_special_defense(generation, defender, attacker_ability, weather, executed.critical_hit)
		)
	);
}

auto screen_divisor(Generation const generation, ExecutedMove const move, Team const & defender) {
	return BOUNDED_CONDITIONAL(screen_is_active(generation, move, defender), 2_bi, 1_bi);
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

auto regular_damage(Generation const generation, Team const & attacker_team, ExecutedMove const executed, bool const move_weakened_from_item, Team const & defender_team, Weather const weather) {
	auto const attacker = attacker_team.pokemon();
	auto const attacker_ability = attacker.ability();
	auto const defender = defender_team.pokemon();
	auto const effectiveness = Effectiveness(generation, executed.move.type, defender.types());

	auto const temp =
		(level_multiplier(attacker) + 2_bi) *
		move_power(generation, attacker_team, executed, defender_team, weather) *
		physical_vs_special_modifier(generation, attacker, executed, defender, weather) /
		screen_divisor(generation, executed, defender_team) *
		calculate_weather_modifier(executed.move.type, weather, weather_is_blocked_by_ability(attacker_ability, defender.ability())) *
		calculate_flash_fire_modifier(attacker, executed.move.type) +
		2_bi;

	return bounded::max(1_bi, temp *
		critical_hit_multiplier(generation, attacker_ability, executed.critical_hit) *
		calculate_item_modifier(generation, attacker, weather) *
		calculate_me_first_modifier(attacker) *
		attacker.random_damage_multiplier() *
		calculate_stab_modifier(attacker, executed.move.type) *
		effectiveness *
		calculate_ability_effectiveness_modifier(defender.ability(), effectiveness) *
		calculate_expert_belt_modifier(attacker.item(generation, weather), effectiveness) *
		tinted_lens_multiplier(attacker_ability, effectiveness) /
		resistance_berry_divisor(move_weakened_from_item)
	);
}

auto raw_damage(Generation const generation, Team const & attacker_team, ExecutedMove const executed, bool const move_weakened_from_item, Team const & defender_team, OtherMove const defender_move, Weather const weather) -> damage_type {
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
			return bounded::max(get_hp(defender).current() - get_hp(attacker).current(), 0_bi);
		case Moves::Final_Gambit:
			return get_hp(attacker).current();
		case Moves::Fissure:
		case Moves::Guillotine:
		case Moves::Horn_Drill:
		case Moves::Sheer_Cold:
			return BOUNDED_CONDITIONAL(defender.ability() == Ability::Sturdy, 0_bi, get_hp(defender).max());
		case Moves::Metal_Burst:
			return attacker.direct_damage_received() * 3_bi / 2_bi;
		case Moves::Mirror_Coat:
			return defender_move.used_move_is_special(generation) ? attacker.direct_damage_received() * 2_bi : 0_bi;
		case Moves::Night_Shade:
		case Moves::Seismic_Toss:
			return get_level(attacker)();
		case Moves::Psywave:
			return executed.variable.psywave_damage(get_level(attacker));
		case Moves::Sonic_Boom:
			return 20_bi;
		case Moves::Super_Fang:
			return get_hp(defender).current() / 2_bi;
		default:
			return static_cast<damage_type>(regular_damage(
				generation,
				attacker_team,
				executed,
				move_weakened_from_item,
				defender_team,
				weather
			));
	}
}

}	// namespace


auto calculate_damage(Generation const generation, Team const & attacker, ExecutedMove const executed, bool const move_weakened_from_item, Team const & defender, OtherMove const defender_move, Weather const weather) -> damage_type {
	return affects_target(generation, executed.move.type, defender.pokemon(), weather) ?
		raw_damage(generation, attacker, executed, move_weakened_from_item, defender, defender_move, weather) :
		static_cast<damage_type>(0_bi);
}

}	// namespace technicalmachine
