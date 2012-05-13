// Damage calculator
// Copyright (C) 2012 David Stone
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
#include "move.hpp"
#include "move_power.hpp"
#include "pokemon.hpp"
#include "status.hpp"
#include "team.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {

unsigned capped_damage (Team const & attacker, Team const & defender, Weather const & weather);
unsigned regular_damage (Team const & attacker, Team const & defender, Weather const & weather);

unsigned calculate_level_multiplier (Pokemon const & attacker);
unsigned physical_vs_special_modifier (Pokemon const & attacker, Pokemon const & defender, unsigned damage);
unsigned calculate_screen_divisor (Team const & attacker, Team const & defender);
bool screen_is_active (Team const & attacker, Team const & defender);
bool reflect_is_active (Move const & move, Team const & defender);
bool light_screen_is_active (Move const & move, Team const & defender);
unsigned weakening_from_status (Pokemon const & attacker);
unsigned calculate_weather_modifier (Type type, Weather const & weather, unsigned damage);
unsigned calculate_flash_fire_modifier (Team const & attacker, unsigned damage);
unsigned calculate_critical_hit_multiplier (Team const & attacker);
unsigned calculate_item_modifier (Pokemon const & attacker, unsigned damage);
unsigned calculate_me_first_modifier (Team const & attacker, unsigned damage);

unsigned calculate_random_modifier (Move const & move, unsigned damage);
unsigned calculate_stab_modifier (Team const & attacker, unsigned damage);
unsigned calculate_stab_boost (Ability ability, unsigned damage);
unsigned calculate_effectiveness_modifier (Move const & move, Pokemon const & defender, unsigned damage);
unsigned calculate_ability_effectiveness_modifier (Ability ability, unsigned effectiveness, unsigned damage);
unsigned calculate_expert_belt_modifier (Item item, unsigned effectiveness, unsigned damage);
unsigned calculate_tinted_lens_multiplier (Ability ability, unsigned effectiveness);
unsigned calculate_resistance_berry_divisor (Item item, Type type, unsigned effectiveness);
bool resistance_berry_activates (Item item, Type type, unsigned effectiveness);

}	// unnamed namespace

unsigned damage_calculator (Team const & attacker, Team const & defender, Weather const & weather) {
	return attacker.pokemon().move().affects_target(defender, weather) ?
		capped_damage (attacker, defender, weather) :
		0;
}

namespace {

unsigned capped_damage (Team const & attacker, Team const & defender, Weather const & weather) {
	unsigned damage = uncapped_damage (attacker, defender, weather);
	if (damage >= defender.pokemon().hp.stat) {
		damage = defender.pokemon().hp.stat;
		if (attacker.pokemon().move().cannot_ko() or defender.endure)
			--damage;
	}
	return damage;
}

}	// unnamed namespace

unsigned uncapped_damage (Team const & attacker, Team const & defender, Weather const & weather) {
	Pokemon const & pokemon = attacker.pokemon();
	switch (pokemon.move().name) {
		case Move::DRAGON_RAGE:
			return 40;
		case Move::ENDEAVOR:
			return static_cast<unsigned> ((std::max) (defender.pokemon().hp.stat - pokemon.hp.stat, 0));
		case Move::FISSURE:
		case Move::GUILLOTINE:
		case Move::HORN_DRILL:
		case Move::SHEER_COLD:
			return defender.pokemon().hp.max;
		case Move::NIGHT_SHADE:
		case Move::SEISMIC_TOSS:
			return pokemon.level;
		case Move::PSYWAVE:
			return pokemon.move().variable().psywave_damage(pokemon.level);
		case Move::SONICBOOM:
			return 20;
		case Move::SUPER_FANG:
			return defender.pokemon().hp.stat / 2;
		default:
			return regular_damage (attacker, defender, weather);
	}
}

namespace {

unsigned regular_damage (Team const & attacker, Team const & defender, Weather const & weather) {
	Pokemon const & pokemon = attacker.pokemon();

	unsigned damage = calculate_level_multiplier (pokemon);
	damage += 2;

	damage *= move_power(attacker, defender, weather);
	damage = physical_vs_special_modifier (pokemon, defender.pokemon(), damage);
	damage /= calculate_screen_divisor (attacker, defender);
	damage = calculate_weather_modifier (pokemon.move().type(), weather, damage);
	damage = calculate_flash_fire_modifier (attacker, damage);
	damage += 2;

	damage *= calculate_critical_hit_multiplier (attacker);
	damage = calculate_item_modifier (pokemon, damage);
	damage = calculate_me_first_modifier (attacker, damage);

	damage = calculate_random_modifier (pokemon.move(), damage);
	damage = calculate_stab_modifier (attacker, damage);
	damage = calculate_effectiveness_modifier (pokemon.move(), defender.pokemon(), damage);

	unsigned const effectiveness = pokemon.move().type().get_effectiveness (defender.pokemon());
	damage = calculate_ability_effectiveness_modifier (defender.pokemon().ability, effectiveness, damage);
	damage = calculate_expert_belt_modifier (pokemon.item, effectiveness, damage);
	damage *= calculate_tinted_lens_multiplier (pokemon.ability, effectiveness);
	damage /= calculate_resistance_berry_divisor (defender.pokemon().item, pokemon.move().type(), effectiveness);

	return (damage > 0) ? damage : 1;
}

}	// unnamed namespace


void recoil (Pokemon & user, unsigned damage, unsigned denominator) {
	if (!user.ability.blocks_recoil()) {
		if (damage <= 2 * denominator - 1)
			--user.hp.stat;
		else
			damage_side_effect (user, damage / denominator);
	}
}

void damage_side_effect (Pokemon & user, unsigned damage) {
	damage = (std::min) (damage, static_cast<unsigned> (user.hp.stat));
	user.hp.stat -= damage;
}



namespace {

unsigned calculate_level_multiplier (Pokemon const & attacker) {
	return attacker.level * 2u / 5;
}

unsigned physical_vs_special_modifier (Pokemon const & attacker, Pokemon const & defender, unsigned const damage) {
	return attacker.move().is_physical() ?
		damage * attacker.atk.stat / 50 / defender.def.stat / weakening_from_status (attacker) :
		damage * attacker.spa.stat / 50 / defender.spd.stat;
}

unsigned weakening_from_status (Pokemon const & attacker) {
	return (attacker.status.weakens_physical_attacks() and attacker.ability.blocks_burn_damage_penalty()) ? 2 : 1;
}

unsigned calculate_screen_divisor (Team const & attacker, Team const & defender) {
	return screen_is_active (attacker, defender) ? 2 : 1;
}

bool screen_is_active (Team const & attacker, Team const & defender) {
	Move const & move = attacker.pokemon().move();
	return (reflect_is_active (move, defender) or light_screen_is_active (move, defender)) and !attacker.ch;
}

bool reflect_is_active (Move const & move, Team const & defender) {
	return defender.reflect and move.is_physical();
}

bool light_screen_is_active (Move const & move, Team const & defender) {
	return defender.light_screen and move.is_special();
}

unsigned calculate_weather_modifier (Type const type, Weather const & weather, unsigned const damage) {
	if (type.is_strengthened_by_weather(weather))
		return damage * 3 / 2;
	else if (type.is_weakened_by_weather(weather))
		return damage / 2;
	else
		return damage;
}

unsigned calculate_flash_fire_modifier (Team const & attacker, unsigned const damage) {
	return (attacker.flash_fire and attacker.pokemon().move().type().is_boosted_by_flash_fire()) ? damage * 3 / 2 : damage;
}

unsigned calculate_critical_hit_multiplier (Team const & attacker) {
	if (attacker.ch)
		return (attacker.pokemon().ability.boosts_critical_hits ()) ? 3 : 2;
	return 1;
}

unsigned calculate_item_modifier (Pokemon const & attacker, unsigned const damage) {
	switch (attacker.item.name) {
		case Item::LIFE_ORB:
			return damage * 13 / 10;
		case Item::METRONOME:
			return damage * ((attacker.move().times_used >= 10) ? 2 : (10u + attacker.move().times_used) / 10);
		default:
			return damage;
	}
}

unsigned calculate_me_first_modifier (Team const & attacker, unsigned const damage) {
	return (attacker.me_first) ? damage * 3 / 2 : damage;
}




unsigned calculate_random_modifier (Move const & move, unsigned const damage) {
	return damage * move.r / 100;
}

unsigned calculate_stab_modifier (Team const & attacker, unsigned const damage) {
	return is_type (attacker, attacker.pokemon().move().type()) ?
		calculate_stab_boost (attacker.pokemon().ability, damage) :
		damage;
}

unsigned calculate_stab_boost (Ability const ability, unsigned const damage) {
	return ability.boosts_stab() ? damage * 2 : damage * 3 / 2;
}

unsigned calculate_effectiveness_modifier (Move const & move, Pokemon const & defender, unsigned damage) {
	std::vector <unsigned> const effectiveness = move.type().get_effectiveness_variables (defender);
	for (unsigned const effective : effectiveness)
		damage = damage * effective / 2;
	return damage;
}

unsigned calculate_ability_effectiveness_modifier (Ability const ability, unsigned const effectiveness, unsigned const damage) {
	return (ability.weakens_se_attacks () and effectiveness > 4) ? 3 * damage / 4 : damage;
}

unsigned calculate_expert_belt_modifier (Item const item, unsigned const effectiveness, unsigned const damage) {
	return (item.boosts_super_effective_moves() and effectiveness > 4) ? damage * 6 / 5 : damage;
}

unsigned calculate_tinted_lens_multiplier (Ability const ability, unsigned const effectiveness) {
	return (ability.strengthens_nve_attacks() and effectiveness < 4) ? 2 : 1;
}

unsigned calculate_resistance_berry_divisor (Item const item, Type const type, unsigned const effectiveness) {
	return resistance_berry_activates (item, type, effectiveness) ? 2 : 1;
}

bool resistance_berry_activates (Item const item, Type const type, unsigned const effectiveness) {
	// Perhaps I should create some sort of item function that returns the type
	// that the item grants resistance toward (and some sort of guard type to
	// indicate that the item does not grant resistance). Then I can
	// `return type == returned_type;`
	if (item.name == Item::CHILAN_BERRY)
		return type == Type::NORMAL;
	else if (effectiveness > 4) {
		switch (item.name) {
			case Item::BABIRI_BERRY:
				return type == Type::STEEL;
			case Item::CHARTI_BERRY:
				return type == Type::ROCK;
			case Item::CHOPLE_BERRY:
				return type == Type::FIGHTING;
			case Item::COBA_BERRY:
				return type == Type::FLYING;
			case Item::COLBUR_BERRY:
				return type == Type::DARK;
			case Item::HABAN_BERRY:
				return type == Type::DRAGON;
			case Item::KASIB_BERRY:
				return type == Type::GHOST;
			case Item::KEBIA_BERRY:
				return type == Type::POISON;
			case Item::OCCA_BERRY:
				return type == Type::FIRE;
			case Item::PASSHO_BERRY:
				return type == Type::WATER;
			case Item::PAYAPA_BERRY:
				return type == Type::PSYCHIC;
			case Item::RINDO_BERRY:
				return type == Type::GRASS;
			case Item::SHUCA_BERRY:
				return type == Type::GROUND;
			case Item::TANGA_BERRY:
				return type == Type::BUG;
			case Item::WACAN_BERRY:
				return type == Type::ELECTRIC;
			case Item::YACHE_BERRY:
				return type == Type::ICE;
			default:
				return false;
		}
	}
	return false;
}

}	// unnamed namespace
}	// namespace technicalmachine
