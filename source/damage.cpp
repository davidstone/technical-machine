// Damage calculator
// Copyright (C) 2014 David Stone
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

#include "damage.hpp"

#include <algorithm>
#include <vector>

#include "ability.hpp"
#include "item.hpp"
#include "status.hpp"
#include "rational.hpp"
#include "team.hpp"
#include "variable.hpp"

#include "move/move.hpp"
#include "move/moves.hpp"
#include "move/power.hpp"

#include "pokemon/pokemon.hpp"

#include "stat/calculate.hpp"

#include "type/effectiveness.hpp"

namespace technicalmachine {
using namespace bounded::literal;
namespace {

#define CONDITIONAL(a, b, c) BOUNDED_INTEGER_CONDITIONAL(a, b, c)

bool affects_target(Type const & move_type, ActivePokemon const & target, Weather const & weather);

damage_type regular_damage(Team const & attacker, Team const & defender, Weather const & weather, Variable const & variable);

bool screen_is_active (ActivePokemon const & attacker, Team const & defender);
bool reflect_is_active (Move const & move, Team const & defender);
bool light_screen_is_active (Move const & move, Team const & defender);
Rational calculate_weather_modifier (Type type, Weather const & weather);
Rational calculate_flash_fire_modifier (ActivePokemon const & attacker);
Rational calculate_item_modifier(ActivePokemon const & attacker);
Rational calculate_me_first_modifier (ActivePokemon const & attacker);

Rational calculate_stab_modifier (ActivePokemon const & attacker);
Rational calculate_stab_boost (Ability ability);
Rational calculate_ability_effectiveness_modifier (Ability ability, Effectiveness const & effectiveness);
Rational calculate_expert_belt_modifier (Item item, Effectiveness const & effectiveness);
bool resistance_berry_activates (Item item, Type type, Effectiveness const & effectiveness);


bool affects_target(Type const & move_type, ActivePokemon const & target, Weather const & weather) {
	return !Effectiveness(move_type, target).has_no_effect() and (move_type != Type::Ground or grounded(target, weather));
}

constexpr bool cannot_ko(Moves const move) {
	return move == Moves::False_Swipe;
}

damage_type raw_damage(Team const & attacker_team, Team const & defender, Weather const & weather, Variable const & variable) {
	auto const & attacker = attacker_team.pokemon();
	switch (static_cast<Moves>(attacker.move())) {
		case Moves::Dragon_Rage:
			return 40_bi;
		case Moves::Endeavor:
			return bounded::max(get_hp(defender.pokemon()).current() - get_hp(attacker).current(), 0_bi);
		case Moves::Fissure:
		case Moves::Guillotine:
		case Moves::Horn_Drill:
		case Moves::Sheer_Cold:
			return get_hp(defender.pokemon()).max();
		case Moves::Night_Shade:
		case Moves::Seismic_Toss:
			return get_level(attacker)();
		case Moves::Psywave:
			return variable.psywave_damage(get_level(attacker));
		case Moves::SonicBoom:
			return 20_bi;
		case Moves::Super_Fang:
			return get_hp(defender.pokemon()).current() / 2_bi;
		default:
			return regular_damage(attacker_team, defender, weather, variable);
	}
}

damage_type capped_damage(Team const & attacker, Team const & defender, Weather const & weather, Variable const & variable) {
	auto const damage = raw_damage(attacker, defender, weather, variable);
	return (cannot_ko(attacker.pokemon().move()) or defender.pokemon().cannot_be_koed()) ?
		static_cast<damage_type>(bounded::min(get_hp(defender.pokemon()).current() - 1_bi, damage)) :
		damage;
}

}	// namespace

damage_type damage_calculator(Team const & attacker, Team const & defender, Weather const & weather, Variable const & variable) {
	return affects_target(Type(attacker.pokemon().move(), attacker.pokemon()), defender.pokemon(), weather) ?
		capped_damage(attacker, defender, weather, variable) :
		static_cast<damage_type>(0_bi);
}

namespace {

auto level_multiplier(Pokemon const & attacker) -> decltype(get_level(attacker)() * 2_bi / 5_bi) {
	return get_level(attacker)() * 2_bi / 5_bi;
}

auto weakening_from_status(Pokemon const & attacker) {
	return CONDITIONAL(
		get_status(attacker).weakens_physical_attacks() and get_ability(attacker).blocks_burn_damage_penalty(),
		2_bi,
		1_bi
	);
}

auto physical_vs_special_modifier(ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather) {
	// For all integers a, b, and c:
	// (a / b) / c == a / (b * c)
	// See: http://math.stackexchange.com/questions/147771/rewriting-repeated-integer-division-with-multiplication
	return CONDITIONAL(is_physical(attacker.move()),
		make_rational(
			calculate_attack(attacker, weather),
			50_bi * calculate_defense(defender, weather, attacker.critical_hit()) * weakening_from_status(attacker)
		),
		make_rational(
			calculate_special_attack(attacker, weather),
			50_bi * calculate_special_defense(defender, weather, attacker.critical_hit())
		)
	);
}

auto screen_divisor(ActivePokemon const & attacker, Team const & defender) {
	return CONDITIONAL(screen_is_active (attacker, defender), 2_bi, 1_bi);
}

auto critical_hit_multiplier(ActivePokemon const & attacker) {
	return CONDITIONAL(
		!attacker.critical_hit(), 1_bi,
		CONDITIONAL(get_ability(attacker).boosts_critical_hits(), 3_bi, 2_bi)
	);
}

auto tinted_lens_multiplier(Ability const ability, Effectiveness const & effectiveness) {
	return CONDITIONAL(
		ability.strengthens_nve_attacks() and effectiveness.is_not_very_effective(),
		2_bi,
		1_bi
	);
}

auto resistance_berry_divisor(Item const item, Type const type, Effectiveness const & effectiveness) {
	return CONDITIONAL(resistance_berry_activates(item, type, effectiveness), 2_bi, 1_bi);
}

damage_type regular_damage(Team const & attacker_team, Team const & defender, Weather const & weather, Variable const & variable) {
	auto const & attacker = attacker_team.pokemon();
	auto damage = static_cast<unsigned>(level_multiplier(attacker) + 2_bi);

	damage *= move_power(attacker_team, defender, weather, variable);
	damage *= Rational(physical_vs_special_modifier(attacker, defender.pokemon(), weather));
	damage /= screen_divisor(attacker, defender);
	Type const type(attacker.move(), attacker);
	damage *= calculate_weather_modifier(type, weather);
	damage *= calculate_flash_fire_modifier(attacker);
	damage += 2;

	damage *= critical_hit_multiplier(attacker);
	damage *= calculate_item_modifier(attacker);
	damage *= calculate_me_first_modifier(attacker);

	damage *= attacker.random_damage_multiplier();
	damage *= calculate_stab_modifier(attacker);

	Effectiveness const effectiveness(type, defender.pokemon());
	damage *= effectiveness;
	damage *= calculate_ability_effectiveness_modifier(get_ability(defender.pokemon()), effectiveness);
	damage *= calculate_expert_belt_modifier(get_item(attacker), effectiveness);
	damage *= tinted_lens_multiplier(get_ability(attacker), effectiveness);
	damage /= resistance_berry_divisor(get_item(defender.pokemon()), type, effectiveness);

	return bounded::max(static_cast<damage_type>(damage), 1_bi);
}

}	// namespace


void recoil(Pokemon & user, damage_type const damage, bounded::checked_integer<1, 4> const denominator) {
	if (!get_ability(user).blocks_recoil()) {
		get_hp(user) -= bounded::max(damage / denominator, 1_bi);
	}
}

namespace {

bool screen_is_active (ActivePokemon const & attacker, Team const & defender) {
	Move const & move = attacker.move();
	return (reflect_is_active (move, defender) or light_screen_is_active (move, defender)) and !attacker.critical_hit();
}

bool reflect_is_active (Move const & move, Team const & defender) {
	return defender.screens.reflect() and is_physical(move);
}

bool light_screen_is_active (Move const & move, Team const & defender) {
	return defender.screens.light_screen() and is_special(move);
}

Rational calculate_weather_modifier (Type const type, Weather const & weather) {
	if (type.is_strengthened_by_weather(weather))
		return Rational(3, 2);
	else if (type.is_weakened_by_weather(weather))
		return Rational(1, 2);
	else
		return Rational(1);
}

Rational calculate_flash_fire_modifier (ActivePokemon const & attacker) {
	Type const type(attacker.move(), attacker);
	return (attacker.flash_fire_is_active() and type.is_boosted_by_flash_fire()) ? Rational(3, 2) : Rational(1);
}

Rational calculate_item_modifier(ActivePokemon const & attacker) {
	switch (get_item(attacker).name) {
		case Item::LIFE_ORB:
			return Rational(13, 10);
		case Item::METRONOME:
			return Rational(attacker.metronome_boost());
		default:
			return Rational(1);
	}
}

Rational calculate_me_first_modifier (ActivePokemon const & attacker) {
	return attacker.me_first_is_active() ? Rational(3, 2) : Rational(1);
}

Rational calculate_stab_modifier (ActivePokemon const & attacker) {
	Type const type(attacker.move(), attacker);
	return is_type(attacker, type) ? calculate_stab_boost(get_ability(attacker)) : Rational(1);
}

Rational calculate_stab_boost (Ability const ability) {
	return ability.boosts_stab() ? Rational(2) : Rational(3, 2);
}

Rational calculate_ability_effectiveness_modifier (Ability const ability, Effectiveness const & effectiveness) {
	return (ability.weakens_se_attacks() and effectiveness.is_super_effective()) ? Rational(3, 4) : Rational(1);
}

Rational calculate_expert_belt_modifier (Item const item, Effectiveness const & effectiveness) {
	return (item.boosts_super_effective_moves() and effectiveness.is_super_effective()) ? Rational(6, 5) : Rational(1);
}

bool resistance_berry_activates(Item const item, Type const type, Effectiveness const & effectiveness) {
	// Perhaps I should create some sort of item function that returns the type
	// that the item grants resistance toward (and some sort of guard type to
	// indicate that the item does not grant resistance). Then I can
	// `return type == returned_type;`
	if (item.name == Item::CHILAN_BERRY)
		return type == Type::Normal;
	else if (effectiveness.is_super_effective()) {
		switch (item.name) {
			case Item::BABIRI_BERRY:
				return type == Type::Steel;
			case Item::CHARTI_BERRY:
				return type == Type::Rock;
			case Item::CHOPLE_BERRY:
				return type == Type::Fighting;
			case Item::COBA_BERRY:
				return type == Type::Flying;
			case Item::COLBUR_BERRY:
				return type == Type::Dark;
			case Item::HABAN_BERRY:
				return type == Type::Dragon;
			case Item::KASIB_BERRY:
				return type == Type::Ghost;
			case Item::KEBIA_BERRY:
				return type == Type::Poison;
			case Item::OCCA_BERRY:
				return type == Type::Fire;
			case Item::PASSHO_BERRY:
				return type == Type::Water;
			case Item::PAYAPA_BERRY:
				return type == Type::Psychic;
			case Item::RINDO_BERRY:
				return type == Type::Grass;
			case Item::SHUCA_BERRY:
				return type == Type::Ground;
			case Item::TANGA_BERRY:
				return type == Type::Bug;
			case Item::WACAN_BERRY:
				return type == Type::Electric;
			case Item::YACHE_BERRY:
				return type == Type::Ice;
			default:
				return false;
		}
	}
	return false;
}

}	// unnamed namespace
}	// namespace technicalmachine
