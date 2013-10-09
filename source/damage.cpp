// Damage calculator
// Copyright (C) 2013 David Stone
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

#include "type/effectiveness.hpp"

namespace technicalmachine {
namespace {

bool affects_target(Type const & move_type, ActivePokemon const & target, Weather const & weather);

unsigned capped_damage(ActivePokemon const & attacker, Team const & defender, Weather const & weather, Variable const & variable);
unsigned regular_damage(ActivePokemon const & attacker, Team const & defender, Weather const & weather, Variable const & variable);

unsigned calculate_level_multiplier (Pokemon const & attacker);
Rational physical_vs_special_modifier (Pokemon const & attacker, Pokemon const & defender);
unsigned calculate_screen_divisor (ActivePokemon const & attacker, Team const & defender);
bool screen_is_active (ActivePokemon const & attacker, Team const & defender);
bool reflect_is_active (Move const & move, Team const & defender);
bool light_screen_is_active (Move const & move, Team const & defender);
unsigned weakening_from_status (Pokemon const & attacker);
Rational calculate_weather_modifier (Type type, Weather const & weather);
Rational calculate_flash_fire_modifier (ActivePokemon const & attacker);
unsigned calculate_critical_hit_multiplier (ActivePokemon const & attacker);
Rational calculate_item_modifier(ActivePokemon const & attacker);
Rational calculate_me_first_modifier (ActivePokemon const & attacker);

Rational calculate_stab_modifier (ActivePokemon const & attacker);
Rational calculate_stab_boost (Ability ability);
Rational calculate_ability_effectiveness_modifier (Ability ability, Effectiveness const & effectiveness);
Rational calculate_expert_belt_modifier (Item item, Effectiveness const & effectiveness);
unsigned calculate_tinted_lens_multiplier (Ability ability, Effectiveness const & effectiveness);
unsigned calculate_resistance_berry_divisor (Item item, Type type, Effectiveness const & effectiveness);
bool resistance_berry_activates (Item item, Type type, Effectiveness const & effectiveness);

}	// unnamed namespace

unsigned damage_calculator(ActivePokemon const & attacker, Team const & defender, Weather const & weather, Variable const & variable) {
	return affects_target(Type(attacker.move(), attacker), defender.pokemon(), weather) ?
		capped_damage (attacker, defender, weather, variable) :
		0;
}

namespace {

bool affects_target(Type const & move_type, ActivePokemon const & target, Weather const & weather) {
	return !move_type.get_effectiveness(target).has_no_effect() and (move_type != Type::Ground or grounded(target, weather));
}

constexpr bool cannot_ko(Moves const move) {
	return move == Moves::False_Swipe;
}

unsigned capped_damage(ActivePokemon const & attacker, Team const & defender, Weather const & weather, Variable const & variable) {
	unsigned damage = uncapped_damage (attacker, defender, weather, variable);
	Stat const & hp = defender.pokemon().stat(Stat::HP);
	if (damage >= hp.stat) {
		damage = hp.stat;
		if (cannot_ko(attacker.move()) or defender.pokemon().cannot_be_koed())
			--damage;
	}
	return damage;
}

}	// unnamed namespace

unsigned uncapped_damage(ActivePokemon const & attacker, Team const & defender, Weather const & weather, Variable const & variable) {
	switch (attacker.move().name()) {
		case Moves::Dragon_Rage:
			return 40;
		case Moves::Endeavor:
			return static_cast<unsigned> (std::max(defender.pokemon().stat(Stat::HP).stat - attacker.stat(Stat::HP).stat, 0));
		case Moves::Fissure:
		case Moves::Guillotine:
		case Moves::Horn_Drill:
		case Moves::Sheer_Cold:
			return defender.pokemon().stat(Stat::HP).max;
		case Moves::Night_Shade:
		case Moves::Seismic_Toss:
			return attacker.level();
		case Moves::Psywave:
			return variable.psywave_damage(attacker.level());
		case Moves::SonicBoom:
			return 20;
		case Moves::Super_Fang:
			return defender.pokemon().stat(Stat::HP).stat / 2;
		default:
			return regular_damage(attacker, defender, weather, variable);
	}
}

namespace {

unsigned regular_damage(ActivePokemon const & attacker, Team const & defender, Weather const & weather, Variable const & variable) {
	unsigned damage = calculate_level_multiplier(attacker);
	damage += 2;

	damage *= move_power(attacker, defender.pokemon(), weather, variable);
	damage *= physical_vs_special_modifier(attacker, defender.pokemon());
	damage /= calculate_screen_divisor(attacker, defender);
	Type const type(attacker.move(), attacker);
	damage *= calculate_weather_modifier(type, weather);
	damage *= calculate_flash_fire_modifier(attacker);
	damage += 2;

	damage *= calculate_critical_hit_multiplier(attacker);
	damage *= calculate_item_modifier(attacker);
	damage *= calculate_me_first_modifier(attacker);

	damage *= attacker.random_damage_multiplier();
	damage *= calculate_stab_modifier(attacker);

	Effectiveness const effectiveness = type.get_effectiveness(defender.pokemon());
	damage *= effectiveness;
	damage *= calculate_ability_effectiveness_modifier(defender.pokemon().ability(), effectiveness);
	damage *= calculate_expert_belt_modifier(attacker.item(), effectiveness);
	damage *= calculate_tinted_lens_multiplier(attacker.ability(), effectiveness);
	damage /= calculate_resistance_berry_divisor(defender.pokemon().item(), type, effectiveness);

	return std::max(damage, 1u);
}

}	// unnamed namespace


unsigned apply_damage(Pokemon & pokemon, unsigned damage) {
	auto & hp = pokemon.stat(Stat::HP);
	damage = std::min(damage, static_cast<unsigned>(hp.stat));
	hp.stat -= damage;
	return damage;
}

void recoil (Pokemon & user, unsigned damage, unsigned denominator) {
	if (!user.ability().blocks_recoil()) {
		apply_damage(user, std::min(damage / denominator, 1u));
	}
}

namespace {

unsigned calculate_level_multiplier (Pokemon const & attacker) {
	return attacker.level() * 2u / 5;
}

Rational physical_vs_special_modifier (Pokemon const & attacker, Pokemon const & defender) {
	// For all integers a, b, and c:
	// (a / b) / c == a / (b * c)
	// See: http://math.stackexchange.com/questions/147771/rewriting-repeated-integer-division-with-multiplication
	return is_physical(attacker.move()) ?
		Rational(attacker.stat(Stat::ATK).stat, 50u * defender.stat(Stat::DEF).stat * weakening_from_status(attacker)) :
		Rational(attacker.stat(Stat::SPA).stat, 50u * defender.stat(Stat::SPD).stat);
}

unsigned weakening_from_status (Pokemon const & attacker) {
	return (attacker.status().weakens_physical_attacks() and attacker.ability().blocks_burn_damage_penalty()) ? 2 : 1;
}

unsigned calculate_screen_divisor (ActivePokemon const & attacker, Team const & defender) {
	return screen_is_active (attacker, defender) ? 2 : 1;
}

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

unsigned calculate_critical_hit_multiplier (ActivePokemon const & attacker) {
	if (!attacker.critical_hit())
		return 1;
	return (attacker.ability().boosts_critical_hits()) ? 3 : 2;
}

Rational calculate_item_modifier(ActivePokemon const & attacker) {
	switch (attacker.item().name) {
		case Item::LIFE_ORB:
			return Rational(13, 10);
		case Item::METRONOME:
			return attacker.metronome_boost();
		default:
			return Rational(1);
	}
}

Rational calculate_me_first_modifier (ActivePokemon const & attacker) {
	return attacker.me_first_is_active() ? Rational(3, 2) : Rational(1);
}

Rational calculate_stab_modifier (ActivePokemon const & attacker) {
	Type const type(attacker.move(), attacker);
	return is_type(attacker, type) ? calculate_stab_boost(attacker.ability()) : Rational(1);
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

unsigned calculate_tinted_lens_multiplier (Ability const ability, Effectiveness const & effectiveness) {
	return (ability.strengthens_nve_attacks() and effectiveness.is_not_very_effective()) ? 2 : 1;
}

unsigned calculate_resistance_berry_divisor (Item const item, Type const type, Effectiveness const & effectiveness) {
	return resistance_berry_activates(item, type, effectiveness) ? 2 : 1;
}

bool resistance_berry_activates (Item const item, Type const type, Effectiveness const & effectiveness) {
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
