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
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
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
		(move_type != Type::Ground or grounded(target, weather));
}

auto reflect_is_active(Moves const move, Team const & defender) {
	return defender.screens.reflect() and is_physical(move);
}

auto light_screen_is_active(Moves const move, Team const & defender) {
	return defender.screens.light_screen() and is_special(move);
}

auto screen_is_active(Moves const move, Team const & defender, bool const critical_hit) {
	return !critical_hit and (reflect_is_active(move, defender) or light_screen_is_active(move, defender));
}


auto calculate_weather_modifier(Type const type, Weather const weather) {
	return
		BOUNDED_CONDITIONAL(is_strengthened_by_weather(type, weather), rational(3_bi, 2_bi),
		BOUNDED_CONDITIONAL(is_weakened_by_weather(type, weather), rational(1_bi, 2_bi),
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
auto calculate_item_modifier(ActivePokemon const attacker) -> ItemModifier {
	switch (get_item(attacker)) {
		case Item::Life_Orb:
			return rational(13_bi, 10_bi);
		case Item::Metronome:
			return attacker.last_used_move().metronome_boost();
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
		calculate_stab_boost(get_ability(attacker)),
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


bool resistance_berry_activates(Item const item, Type const type, Effectiveness const effectiveness) {
	// Perhaps I should create some sort of item function that returns the type
	// that the item grants resistance toward (and some sort of guard type to
	// indicate that the item does not grant resistance). Then I can
	// `return type == returned_type;`
	if (item == Item::Chilan_Berry)
		return type == Type::Normal;
	else if (effectiveness.is_super_effective()) {
		switch (item) {
			case Item::Babiri_Berry:
				return type == Type::Steel;
			case Item::Charti_Berry:
				return type == Type::Rock;
			case Item::Chople_Berry:
				return type == Type::Fighting;
			case Item::Coba_Berry:
				return type == Type::Flying;
			case Item::Colbur_Berry:
				return type == Type::Dark;
			case Item::Haban_Berry:
				return type == Type::Dragon;
			case Item::Kasib_Berry:
				return type == Type::Ghost;
			case Item::Kebia_Berry:
				return type == Type::Poison;
			case Item::Occa_Berry:
				return type == Type::Fire;
			case Item::Passho_Berry:
				return type == Type::Water;
			case Item::Payapa_Berry:
				return type == Type::Psychic;
			case Item::Rindo_Berry:
				return type == Type::Grass;
			case Item::Shuca_Berry:
				return type == Type::Ground;
			case Item::Tanga_Berry:
				return type == Type::Bug;
			case Item::Wacan_Berry:
				return type == Type::Electric;
			case Item::Yache_Berry:
				return type == Type::Ice;
			default:
				return false;
		}
	}
	return false;
}

constexpr bool cannot_ko(Moves const move) {
	return move == Moves::False_Swipe;
}

auto level_multiplier(Pokemon const & attacker) -> decltype(get_level(attacker)() * 2_bi / 5_bi) {
	return get_level(attacker)() * 2_bi / 5_bi;
}

auto weakening_from_status(Pokemon const & attacker) {
	return BOUNDED_CONDITIONAL(
		weakens_physical_attacks(get_status(attacker)) and blocks_burn_damage_penalty(get_ability(attacker)),
		2_bi,
		1_bi
	);
}

auto physical_vs_special_modifier(Generation const generation, ActivePokemon const attacker, Moves const move, ActivePokemon const defender, Weather const weather, bool const critical_hit) {
	// For all integers a, b, and c:
	// (a / b) / c == a / (b * c)
	// See: http://math.stackexchange.com/questions/147771/rewriting-repeated-integer-division-with-multiplication
	return BOUNDED_CONDITIONAL(is_physical(move),
		rational(
			calculate_attack(generation, attacker, weather, critical_hit),
			50_bi * calculate_defense(generation, defender, weather, critical_hit) * weakening_from_status(attacker)
		),
		rational(
			calculate_special_attack(generation, attacker, weather, critical_hit),
			50_bi * calculate_special_defense(generation, defender, weather, critical_hit)
		)
	);
}

auto screen_divisor(Moves const move, Team const & defender, bool const critical_hit) {
	return BOUNDED_CONDITIONAL(screen_is_active(move, defender, critical_hit), 2_bi, 1_bi);
}

auto critical_hit_multiplier(ActivePokemon const attacker, bool const critical_hit) {
	return BOUNDED_CONDITIONAL(
		!critical_hit, 1_bi,
		BOUNDED_CONDITIONAL(boosts_critical_hits(get_ability(attacker)), 3_bi, 2_bi)
	);
}

auto tinted_lens_multiplier(Ability const ability, Effectiveness const & effectiveness) {
	return BOUNDED_CONDITIONAL(
		strengthens_nve_attacks(ability) and effectiveness.is_not_very_effective(),
		2_bi,
		1_bi
	);
}

auto resistance_berry_divisor(Item const item, Type const type, Effectiveness const & effectiveness) {
	return BOUNDED_CONDITIONAL(resistance_berry_activates(item, type, effectiveness), 2_bi, 1_bi);
}

auto regular_damage(Generation const generation, Team const & attacker_team, ExecutedMove const move, Type const move_type, Team const & defender_team, Weather const weather) {
	auto const attacker = attacker_team.pokemon();
	auto const defender = defender_team.pokemon();
	auto const effectiveness = Effectiveness(generation, move_type, defender.types());

	auto const temp =
		(level_multiplier(attacker) + 2_bi) *
		move_power(generation, attacker_team, move, defender_team, weather) *
		physical_vs_special_modifier(generation, attacker, move.name, defender, weather, move.critical_hit) /
		screen_divisor(move.name, defender_team, move.critical_hit) *
		calculate_weather_modifier(move_type, weather) *
		calculate_flash_fire_modifier(attacker, move_type) +
		2_bi;

	return bounded::max(1_bi, temp *
		critical_hit_multiplier(attacker, move.critical_hit) *
		calculate_item_modifier(attacker) *
		calculate_me_first_modifier(attacker) *
		attacker.random_damage_multiplier() *
		calculate_stab_modifier(attacker, move_type) *
		effectiveness *
		calculate_ability_effectiveness_modifier(get_ability(defender), effectiveness) *
		calculate_expert_belt_modifier(get_item(attacker), effectiveness) *
		tinted_lens_multiplier(get_ability(attacker), effectiveness) /
		resistance_berry_divisor(get_item(defender), move_type, effectiveness)
	);
}

auto raw_damage(Generation const generation, Team const & attacker_team, ExecutedMove const move, Type const move_type, Team const & defender_team, OtherMove const defender_move, Weather const weather) -> damage_type {
	auto const attacker = attacker_team.pokemon();
	auto const defender = defender_team.pokemon();
	switch (move.name) {
		case Moves::Bide:
			// TODO: Determine the damage here
			return 0_bi;
		case Moves::Counter:
			return defender_move.used_move_is_physical() ? attacker.direct_damage_received() * 2_bi : 0_bi;
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
			return get_hp(defender).max();
		case Moves::Metal_Burst:
			return attacker.direct_damage_received() * 3_bi / 2_bi;
		case Moves::Mirror_Coat:
			return defender_move.used_move_is_special() ? attacker.direct_damage_received() * 2_bi : 0_bi;
		case Moves::Night_Shade:
		case Moves::Seismic_Toss:
			return get_level(attacker)();
		case Moves::Psywave:
			return move.variable.psywave_damage(get_level(attacker));
		case Moves::Sonic_Boom:
			return 20_bi;
		case Moves::Super_Fang:
			return get_hp(defender).current() / 2_bi;
		default:
			return static_cast<damage_type>(regular_damage(
				generation,
				attacker_team,
				move,
				move_type,
				defender_team,
				weather
			));
	}
}

auto restricted_damage(Generation const generation, Team const & attacker, ExecutedMove const move, Type const move_type, Team const & defender_team, OtherMove const defender_move, Weather const weather) {
	auto const defender = defender_team.pokemon();
	auto const damage = raw_damage(generation, attacker, move, move_type, defender_team, defender_move, weather);
	return (cannot_ko(move.name) or defender.cannot_be_koed()) ?
		static_cast<damage_type>(bounded::min(get_hp(defender).current() - 1_bi, damage)) :
		damage;
}

}	// namespace


auto calculate_uncapped_damage(Generation const generation, Team const & attacker, ExecutedMove const move, Team const & defender, OtherMove const defender_move, Weather const weather) -> damage_type {
	auto const move_type = get_type(generation, move.name, get_hidden_power(attacker.pokemon()).type());
	return affects_target(generation, move_type, defender.pokemon(), weather) ?
		restricted_damage(generation, attacker, move, move_type, defender, defender_move, weather) :
		static_cast<damage_type>(0_bi);
}

auto calculate_damage(Generation const generation, Team const & attacker, ExecutedMove const move, Team const & defender, OtherMove const defender_move, Weather const weather) -> HP::current_type {
	return bounded::min(
		calculate_uncapped_damage(generation, attacker, move, defender, defender_move, weather),
		get_hp(defender.pokemon()).current()
	);
}


}	// namespace technicalmachine
