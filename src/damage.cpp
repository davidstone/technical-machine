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

#include <vector>

#include "ability.hpp"
#include "item.hpp"
#include "move.hpp"
#include "pokemon.hpp"
#include "status.hpp"
#include "team.hpp"
#include "weather.hpp"

namespace technicalmachine {

// I split my damage calculator up into a function that calculates as much as
// possible with known data, one that calculates without the random number, and
// a function that does the rest of the work because in many cases, I have the
// damage calculator in a deep inner loop, and pre-calculating non-random
// numbers allows me to move much of that calculator to a shallower part of
// code, and pre-calculating known information moves even more out. Profiling
// showed this to be a sound optimization.

namespace {

unsigned calculate_screen_multiplier (Team const & attacker, Team const & defender);
bool screen_is_active (Team const & attacker, Team const & defender);
constexpr bool reflect_is_active (Move const & move, Team const & defender);
constexpr bool light_screen_is_active (Move const & move, Team const & defender);
unsigned calculate_weather_multiplier (Type const type, Weather const & weather);

unsigned physical_vs_special_modifier (Pokemon const & attacker, Pokemon const & defender, unsigned const damage);
unsigned weakening_from_burn (Pokemon const & attacker);
unsigned calculate_flash_fire_modifier (Team const & attacker, unsigned damage);
unsigned calculate_critical_hit_multiplier (Team const & attacker);
unsigned calculate_item_modifier (Pokemon const & attacker, unsigned damage);
unsigned calculate_me_first_modifier (Team const & attacker, unsigned damage);
unsigned calculate_stab_multiplier (Team const & attacker);
unsigned calculate_ability_effectiveness_multiplier (Ability ability, unsigned effectiveness);
unsigned calculate_expert_belt_multiplier (Item item, unsigned effectiveness);
unsigned calculate_tinted_lens_multiplier (Ability ability, unsigned effectiveness);
unsigned calculate_resistance_berry_multiplier (Item item, Type type, unsigned effectiveness);
bool resistance_berry_activates (Item item, Type type, unsigned effectiveness);

}	// unnamed namespace

unsigned damage_calculator (Team const & attacker, Team const & defender, Weather const & weather) {
	Pokemon const & pokemon = attacker.pokemon();
	unsigned effectiveness = get_effectiveness (pokemon.move().type, defender.pokemon());
	if ((effectiveness > 0) and (pokemon.move().type != Type::GROUND or grounded (defender, weather))) {
		switch (pokemon.move().name) {
			case Move::DRAGON_RAGE:
				return 40;
			case Move::ENDEAVOR:
				return (defender.pokemon().hp.stat > pokemon.hp.stat) ? defender.pokemon().hp.stat - pokemon.hp.stat : 0;
			case Move::FISSURE:
			case Move::GUILLOTINE:
			case Move::HORN_DRILL:
			case Move::SHEER_COLD:
				return defender.pokemon().hp.max;
			case Move::NIGHT_SHADE:
			case Move::SEISMIC_TOSS:
				return pokemon.level;
			case Move::PSYWAVE:
				return pokemon.level * pokemon.move().variable().first / 10;
			case Move::SONICBOOM:
				return 20;
			case Move::SUPER_FANG:
				return defender.pokemon().hp.stat / 2;
			default: {
				unsigned rl;						// Reflect / Light Screen (2)
				unsigned weather_mod;		// Sunny Day / Rain Dance (1 if weakened, 3 if strengthened) / 2
				unsigned stab;					// Same Type Attack Bonus: 3 / 2
				unsigned aem;					// Ability Effectiveness Multiplier: Solid Rock (3), Filter (3) / 4
				unsigned eb;						// Expert Belt: 6 / 5
				unsigned tl;							// Tinted Lens (2)
				unsigned rb;						// Resistance berries (2)
				unsigned damage = damage_known (attacker, defender, weather, rl, weather_mod);
				damage = damage_non_random (attacker, defender, rl, weather_mod, stab, effectiveness, aem, eb, tl, rb, damage);
				std::vector <unsigned> effectiveness_vector = get_effectiveness_variables (pokemon.move().type, defender.pokemon());
				return damage_random (pokemon, defender, stab, effectiveness_vector, aem, eb, tl, rb, damage);
			}
		}
	}
	return 0;
}

unsigned damage_known (Team const & attacker, Team const & defender, Weather const & weather, unsigned & rl, unsigned & weather_mod) {

	rl = calculate_screen_multiplier (attacker, defender);
	weather_mod = calculate_weather_multiplier (attacker.pokemon().move().type, weather);

	return attacker.pokemon().level * 2 / 5 + 2;
}

namespace {

unsigned calculate_screen_multiplier (Team const & attacker, Team const & defender) {
	return screen_is_active (attacker, defender) ? 2 : 1;
}

bool screen_is_active (Team const & attacker, Team const & defender) {
	Move const & move = attacker.pokemon().move();
	return (reflect_is_active (move, defender) or light_screen_is_active (move, defender)) and !attacker.ch;
}

constexpr bool reflect_is_active (Move const & move, Team const & defender) {
	return defender.reflect and move.physical;
}

constexpr bool light_screen_is_active (Move const & move, Team const & defender) {
	return defender.light_screen and !move.physical;
}

unsigned calculate_weather_multiplier (Type const type, Weather const & weather) {
	if ((weather.rain and type == Type::WATER) or (weather.sun and type == Type::FIRE))
		return 3;
	else if ((weather.rain and type == Type::FIRE) or (weather.sun and type == Type::WATER))
		return 1;
	else
		return 2;
}

}	// unnamed namespace

unsigned damage_non_random (Team const & attacker, Team const & defender, unsigned rl, unsigned weather_mod, unsigned & stab, unsigned effectiveness, unsigned & aem, unsigned & eb, unsigned & tl, unsigned & rb, unsigned damage) {

	damage *= attacker.pokemon().move().power;
	damage = physical_vs_special_modifier (attacker.pokemon(), defender.pokemon(), damage);

	damage = damage / rl * weather_mod / 2;
	damage = calculate_flash_fire_modifier (attacker, damage);
	damage += 2;

	damage *= calculate_critical_hit_multiplier (attacker);
	damage = calculate_item_modifier (attacker.pokemon(), damage);
	damage = calculate_me_first_modifier (attacker, damage);

	stab = calculate_stab_multiplier (attacker);

	aem = calculate_ability_effectiveness_multiplier (defender.pokemon().ability, effectiveness);
	
	eb = calculate_expert_belt_multiplier (attacker.pokemon().item, effectiveness);
	
	tl = calculate_tinted_lens_multiplier (attacker.pokemon().ability, effectiveness);
	
	rb = calculate_resistance_berry_multiplier (defender.pokemon().item, attacker.pokemon().move().type, effectiveness);

	return damage;
}

namespace {

unsigned physical_vs_special_modifier (Pokemon const & attacker, Pokemon const & defender, unsigned const damage) {
	if (attacker.move().physical)
		return damage * attacker.atk.stat / 50 / defender.def.stat / weakening_from_burn (attacker);
	else
		return damage * attacker.spa.stat / 50 / defender.spd.stat;
}

unsigned weakening_from_burn (Pokemon const & attacker) {
	return (attacker.status.name == Status::BURN and attacker.ability.name != Ability::GUTS) ? 2 : 1;
}

unsigned calculate_flash_fire_modifier (Team const & attacker, unsigned const damage) {
	return damage * ((attacker.flash_fire and attacker.pokemon().move().type == Type::FIRE) ? 3 : 2) / 2;
}

unsigned calculate_critical_hit_multiplier (Team const & attacker) {
	if (attacker.ch)
		return (attacker.pokemon().ability.name == Ability::SNIPER) ? 3 : 2;
	return 1;
}

unsigned calculate_item_modifier (Pokemon const & attacker, unsigned const damage) {
	switch (attacker.item.name) {
		case Item::LIFE_ORB:
			return damage * 13 / 10;
		case Item::METRONOME:
			return damage * ((attacker.move().times_used >= 10) ? 2 : (10 + attacker.move().times_used) / 10);
		default:
			return damage;
	}
}

unsigned calculate_me_first_modifier (Team const & attacker, unsigned const damage) {
	if (attacker.me_first)
		return damage * 3 / 2;
	return damage;
}

unsigned calculate_stab_multiplier (Team const & attacker) {
	if (is_type (attacker, attacker.pokemon().move().type))
		return (attacker.pokemon().ability.name == Ability::ADAPTABILITY) ? 4 : 3;
	return 2;
}

unsigned calculate_ability_effectiveness_multiplier (Ability const ability, unsigned const effectiveness) {
	return (ability.weakens_SE_attacks () and effectiveness > 4) ? 3 : 4;
}

unsigned calculate_expert_belt_multiplier (Item const item, unsigned const effectiveness) {
	return (item.name == Item::EXPERT_BELT and effectiveness > 4) ? 6 : 5;
}

unsigned calculate_tinted_lens_multiplier (Ability const ability, unsigned const effectiveness) {
	return (ability.name == Ability::TINTED_LENS and effectiveness < 4) ? 2 : 1;
}

unsigned calculate_resistance_berry_multiplier (Item const item, Type const type, unsigned const effectiveness) {
	return resistance_berry_activates (item, type, effectiveness) ? 2 : 1;
}

bool resistance_berry_activates (Item const item, Type const type, unsigned const effectiveness) {
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

unsigned damage_random (Pokemon const & attacker, Team const & defender, unsigned stab, std::vector <unsigned> const & effectiveness, unsigned aem, unsigned eb, unsigned tl, unsigned rb, unsigned damage) {
	damage = damage * attacker.move().r / 100 * stab / 2;
	for (unsigned const effective : effectiveness)
		damage = damage * effective / 2;
	damage = damage * aem / 4 * eb / 5 * tl / rb;
	if (damage == 0)
		damage = 1;
	else if (damage >= defender.pokemon().hp.stat) {
		damage = defender.pokemon().hp.stat;
		if (attacker.move().name == Move::FALSE_SWIPE or defender.endure)
			--damage;
	}
	return damage;
}

void recoil (Pokemon & user, unsigned damage, unsigned denominator) {
	if (user.ability.name != Ability::MAGIC_GUARD and user.ability.name != Ability::ROCK_HEAD) {
		if (damage <= 2 * denominator - 1)
			--user.hp.stat;
		else
			damage_side_effect (user, damage / denominator);
	}
}

void damage_side_effect (Pokemon & user, unsigned damage) {
	if (user.hp.stat > damage)
		user.hp.stat -= damage;
	else
		user.hp.stat = 0;
}

}	// namespace technicalmachine
