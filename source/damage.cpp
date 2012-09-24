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
#include "status.hpp"
#include "rational.hpp"
#include "team.hpp"

#include "move/move.hpp"
#include "move/moves.hpp"
#include "move/power.hpp"

#include "pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {

unsigned capped_damage (Team const & attacker, Team const & defender, Weather const & weather);
unsigned regular_damage (Team const & attacker, Team const & defender, Weather const & weather);

unsigned calculate_level_multiplier (Pokemon const & attacker);
Rational physical_vs_special_modifier (Pokemon const & attacker, Pokemon const & defender);
unsigned calculate_screen_divisor (Team const & attacker, Team const & defender);
bool screen_is_active (Team const & attacker, Team const & defender);
bool reflect_is_active (Move const & move, Team const & defender);
bool light_screen_is_active (Move const & move, Team const & defender);
unsigned weakening_from_status (Pokemon const & attacker);
Rational calculate_weather_modifier (Type type, Weather const & weather);
Rational calculate_flash_fire_modifier (Team const & attacker);
unsigned calculate_critical_hit_multiplier (Team const & attacker);
Rational calculate_item_modifier (Pokemon const & attacker);
Rational calculate_me_first_modifier (Team const & attacker);

Rational calculate_stab_modifier (Team const & attacker);
Rational calculate_stab_boost (Ability ability);
std::vector<Rational> calculate_effectiveness_modifier (Move const & move, Pokemon const & defender);
Rational calculate_ability_effectiveness_modifier (Ability ability, unsigned effectiveness);
Rational calculate_expert_belt_modifier (Item item, unsigned effectiveness);
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
	Stat const & hp = defender.pokemon().hp();
	if (damage >= hp.stat) {
		damage = hp.stat;
		if (attacker.pokemon().move().cannot_ko() or defender.cannot_be_koed())
			--damage;
	}
	return damage;
}

}	// unnamed namespace

unsigned uncapped_damage (Team const & attacker, Team const & defender, Weather const & weather) {
	auto const & pokemon = attacker.pokemon();
	switch (pokemon.move().name) {
		case Moves::DRAGON_RAGE:
			return 40;
		case Moves::ENDEAVOR:
			return static_cast<unsigned> (std::max(defender.pokemon().hp().stat - pokemon.hp().stat, 0));
		case Moves::FISSURE:
		case Moves::GUILLOTINE:
		case Moves::HORN_DRILL:
		case Moves::SHEER_COLD:
			return defender.pokemon().hp().max;
		case Moves::NIGHT_SHADE:
		case Moves::SEISMIC_TOSS:
			return pokemon.level();
		case Moves::PSYWAVE:
			return pokemon.move().variable().psywave_damage(pokemon.level());
		case Moves::SONICBOOM:
			return 20;
		case Moves::SUPER_FANG:
			return defender.pokemon().hp().stat / 2;
		default:
			return regular_damage (attacker, defender, weather);
	}
}

namespace {

unsigned regular_damage (Team const & attacker, Team const & defender, Weather const & weather) {
	auto const & pokemon = attacker.pokemon();

	unsigned damage = calculate_level_multiplier(pokemon.get_pokemon());
	damage += 2;

	damage *= move_power(attacker, defender, weather);
	damage *= physical_vs_special_modifier(pokemon.get_pokemon(), defender.pokemon().get_pokemon());
	damage /= calculate_screen_divisor(attacker, defender);
	damage *= calculate_weather_modifier(pokemon.move().type(), weather);
	damage *= calculate_flash_fire_modifier(attacker);
	damage += 2;

	damage *= calculate_critical_hit_multiplier(attacker);
	damage *= calculate_item_modifier(pokemon.get_pokemon());
	damage *= calculate_me_first_modifier(attacker);

	damage *= pokemon.move().r();
	damage *= calculate_stab_modifier(attacker);
	for (Rational const r : calculate_effectiveness_modifier(pokemon.move(), defender.pokemon().get_pokemon()))
		damage *= r;
	unsigned const effectiveness = pokemon.move().type().get_effectiveness(defender.pokemon().get_pokemon());
	damage *= calculate_ability_effectiveness_modifier (defender.pokemon().ability(), effectiveness);
	damage *= calculate_expert_belt_modifier (pokemon.item(), effectiveness);
	damage *= calculate_tinted_lens_multiplier (pokemon.ability(), effectiveness);
	damage /= calculate_resistance_berry_divisor (defender.pokemon().item(), pokemon.move().type(), effectiveness);

	return std::max(damage, 1u);
}

}	// unnamed namespace


void recoil (Pokemon & user, unsigned damage, unsigned denominator) {
	if (!user.ability().blocks_recoil()) {
		if (damage <= 2 * denominator - 1)
			--user.hp.stat;
		else
			damage_side_effect (user, damage / denominator);
	}
}

void damage_side_effect (Pokemon & user, unsigned damage) {
	damage = std::min(damage, static_cast<unsigned> (user.hp.stat));
	user.hp.stat -= damage;
}



namespace {

unsigned calculate_level_multiplier (Pokemon const & attacker) {
	return attacker.level() * 2u / 5;
}

Rational physical_vs_special_modifier (Pokemon const & attacker, Pokemon const & defender) {
	// For all integers a, b, and c:
	// (a / b) / c == a / (b * c)
	// See: http://math.stackexchange.com/questions/147771/rewriting-repeated-integer-division-with-multiplication
	return attacker.move().is_physical() ?
		Rational(attacker.atk.stat, 50u * defender.def.stat * weakening_from_status (attacker)) :
		Rational(attacker.spa.stat, 50u * defender.spd.stat);
}

unsigned weakening_from_status (Pokemon const & attacker) {
	return (attacker.status().weakens_physical_attacks() and attacker.ability().blocks_burn_damage_penalty()) ? 2 : 1;
}

unsigned calculate_screen_divisor (Team const & attacker, Team const & defender) {
	return screen_is_active (attacker, defender) ? 2 : 1;
}

bool screen_is_active (Team const & attacker, Team const & defender) {
	Move const & move = attacker.pokemon().move();
	return (reflect_is_active (move, defender) or light_screen_is_active (move, defender)) and !attacker.critical_hit();
}

bool reflect_is_active (Move const & move, Team const & defender) {
	return defender.screens.reflect() and move.is_physical();
}

bool light_screen_is_active (Move const & move, Team const & defender) {
	return defender.screens.light_screen() and move.is_special();
}

Rational calculate_weather_modifier (Type const type, Weather const & weather) {
	if (type.is_strengthened_by_weather(weather))
		return Rational(3, 2);
	else if (type.is_weakened_by_weather(weather))
		return Rational(1, 2);
	else
		return Rational(1);
}

Rational calculate_flash_fire_modifier (Team const & attacker) {
	Type const & type = attacker.pokemon().move().type();
	return (attacker.flash_fire_is_active() and type.is_boosted_by_flash_fire()) ? Rational(3, 2) : Rational(1);
}

unsigned calculate_critical_hit_multiplier (Team const & attacker) {
	if (!attacker.critical_hit())
		return 1;
	return (attacker.pokemon().ability().boosts_critical_hits()) ? 3 : 2;
}

Rational calculate_item_modifier (Pokemon const & attacker) {
	switch (attacker.item().name) {
		case Item::LIFE_ORB:
			return Rational(13, 10);
		case Item::METRONOME:
			return attacker.move().metronome_boost();
		default:
			return Rational(1);
	}
}

Rational calculate_me_first_modifier (Team const & attacker) {
	return attacker.me_first_is_active() ? Rational(3, 2) : Rational(1);
}

Rational calculate_stab_modifier (Team const & attacker) {
	auto const & pokemon = attacker.pokemon();
	return is_type(attacker, pokemon.move().type()) ? calculate_stab_boost(pokemon.ability()) : Rational(1);
}

Rational calculate_stab_boost (Ability const ability) {
	return ability.boosts_stab() ? Rational(2) : Rational(3, 2);
}

std::vector<Rational> calculate_effectiveness_modifier (Move const & move, Pokemon const & defender) {
	std::vector<Rational> rationals;
	for (unsigned const effectiveness : move.type().get_effectiveness_variables(defender))
		rationals.emplace_back(effectiveness, 2);
	return rationals;
}

Rational calculate_ability_effectiveness_modifier (Ability const ability, unsigned const effectiveness) {
	return (ability.weakens_se_attacks() and effectiveness > 4) ? Rational(3, 4) : Rational(1);
}

Rational calculate_expert_belt_modifier (Item const item, unsigned const effectiveness) {
	return (item.boosts_super_effective_moves() and effectiveness > 4) ? Rational(6, 5) : Rational(1);
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
