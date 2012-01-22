// Damage calculator
// Copyright (C) 2011 David Stone
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

unsigned damageknown (Team const & attacker, Team const & defender, Weather const & weather, unsigned & rl, unsigned & weather_mod, unsigned & ff, unsigned & mf) {
	if (((defender.reflect and attacker.pokemon().move().physical)
			or (defender.light_screen and !attacker.pokemon().move().physical))
			and !attacker.ch)
		rl = 2;
	else
		rl = 1;

	if ((weather.rain and attacker.pokemon().move().type == Type::WATER)
			or (weather.sun and attacker.pokemon().move().type == Type::FIRE))
		weather_mod = 3;
	else if ((weather.rain and attacker.pokemon().move().type == Type::FIRE)
			or (weather.sun and attacker.pokemon().move().type == Type::WATER))
		weather_mod = 1;
	else
		weather_mod = 2;

	if (attacker.flash_fire and attacker.pokemon().move().type == Type::FIRE)
		ff = 3;
	else
		ff = 2;

	if (attacker.me_first)
		mf = 3;
	else
		mf = 2;

	return attacker.pokemon().level * 2 / 5 + 2;
}

unsigned damagenonrandom (Team const & attacker, Team const & defender, unsigned rl, unsigned weather_mod, unsigned ff, unsigned mf, unsigned & stab, unsigned effectiveness, unsigned & aem, unsigned & eb, unsigned & tl, unsigned & rb, unsigned damage) {

	damage *= attacker.pokemon().move().power;

	if (attacker.pokemon().move().physical) {
		damage = damage * attacker.pokemon().atk.stat / 50 / defender.pokemon().def.stat;
		if (attacker.pokemon().status.name == Status::BURN and attacker.pokemon().ability.name != Ability::GUTS)
			damage /= 2;
	}
	else
		damage = damage * attacker.pokemon().spa.stat / 50 / defender.pokemon().spd.stat;

	damage = damage / rl * weather_mod / 2 * ff / 2 + 2;

	if (attacker.ch) {
		if (attacker.pokemon().ability.name == Ability::SNIPER)
			damage *= 3;
		else
			damage *= 2;
	}
	
	switch (attacker.pokemon().item.name) {
		case Item::LIFE_ORB:
			damage = damage * 13 / 10;
			break;
		case Item::METRONOME:
			if (attacker.pokemon().move().times_used >= 10)
				damage *= 2;
			else
				damage = damage * (10 + attacker.pokemon().move().times_used) / 10;
			break;
		default:
			break;
	}

	damage = damage * mf / 2;

	if (is_type (attacker, attacker.pokemon().move().type)) {
		if (attacker.pokemon().ability.name == Ability::ADAPTABILITY)
			stab = 4;
		else
			stab = 3;
	}
	else
		stab = 2;

	if (defender.pokemon().ability.weakens_SE_attacks () and effectiveness > 4)
		aem = 3;
	else
		aem = 4;

	if (attacker.pokemon().item.name == Item::EXPERT_BELT and effectiveness > 4)
		eb = 6;
	else
		eb = 5;

	if (attacker.pokemon().ability.name == Ability::TINTED_LENS and effectiveness < 4)
		tl = 2;
	else
		tl = 1;

	rb = 1;
	if (defender.pokemon().item.name == Item::CHILAN_BERRY and attacker.pokemon().move().type == Type::NORMAL)
		rb = 2;
	else if (effectiveness > 4) {
		switch (attacker.pokemon().item.name) {
			case Item::BABIRI_BERRY:
				if (attacker.pokemon().move().type == Type::STEEL)
					rb = 2;
				break;
			case Item::CHARTI_BERRY:
				if (attacker.pokemon().move().type == Type::ROCK)
					rb = 2;
				break;
			case Item::CHOPLE_BERRY:
				if (attacker.pokemon().move().type == Type::FIGHTING)
					rb = 2;
				break;
			case Item::COBA_BERRY:
				if (attacker.pokemon().move().type == Type::FLYING)
					rb = 2;
				break;
			case Item::COLBUR_BERRY:
				if (attacker.pokemon().move().type == Type::DARK)
					rb = 2;
				break;
			case Item::HABAN_BERRY:
				if (attacker.pokemon().move().type == Type::DRAGON)
					rb = 2;
				break;
			case Item::KASIB_BERRY:
				if (attacker.pokemon().move().type == Type::GHOST)
					rb = 2;
				break;
			case Item::KEBIA_BERRY:
				if (attacker.pokemon().move().type == Type::POISON)
					rb = 2;
				break;
			case Item::OCCA_BERRY:
				if (attacker.pokemon().move().type == Type::FIRE)
					rb = 2;
				break;
			case Item::PASSHO_BERRY:
				if (attacker.pokemon().move().type == Type::WATER)
					rb = 2;
				break;
			case Item::PAYAPA_BERRY:
				if (attacker.pokemon().move().type == Type::PSYCHIC)
					rb = 2;
				break;
			case Item::RINDO_BERRY:
				if (attacker.pokemon().move().type == Type::GRASS)
					rb = 2;
				break;
			case Item::SHUCA_BERRY:
				if (attacker.pokemon().move().type == Type::GROUND)
					rb = 2;
				break;
			case Item::TANGA_BERRY:
				if (attacker.pokemon().move().type == Type::BUG)
					rb = 2;
				break;
			case Item::WACAN_BERRY:
				if (attacker.pokemon().move().type == Type::ELECTRIC)
					rb = 2;
				break;
			case Item::YACHE_BERRY:
				if (attacker.pokemon().move().type == Type::ICE)
					rb = 2;
				break;
			default:
				break;
		}
	}

	return damage;
}

unsigned damagerandom (Pokemon const & attacker, Team const & defender, unsigned stab, std::vector <unsigned> const & effectiveness, unsigned aem, unsigned eb, unsigned tl, unsigned rb, unsigned damage) {
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

unsigned damagecalculator (Team const & attacker, Team const & defender, Weather const & weather) {
	unsigned damage = 0;
	unsigned effectiveness = get_effectiveness (attacker.pokemon().move().type, defender.pokemon());
	if ((effectiveness > 0) and (attacker.pokemon().move().type != Type::GROUND or grounded (defender, weather))) {
		switch (attacker.pokemon().move().name) {
			case Move::DRAGON_RAGE:
				damage = 40;
				break;
			case Move::ENDEAVOR:
				if (defender.pokemon().hp.stat > attacker.pokemon().hp.stat)
					damage = defender.pokemon().hp.stat - attacker.pokemon().hp.stat;
				else
					damage = 0;
				break;
			case Move::FISSURE:
			case Move::GUILLOTINE:
			case Move::HORN_DRILL:
			case Move::SHEER_COLD:
				damage = defender.pokemon().hp.max;
				break;
			case Move::NIGHT_SHADE:
			case Move::SEISMIC_TOSS:
				damage = attacker.pokemon().level;
				break;
			case Move::PSYWAVE:
				damage = attacker.pokemon().level * attacker.pokemon().move().variable().first / 10;
				break;
			case Move::SONICBOOM:
				damage = 20;
				break;
			case Move::SUPER_FANG:
				damage = defender.pokemon().hp.stat / 2;

			default: {
				unsigned rl;						// Reflect / Light Screen (2)
				unsigned weather_mod;		// Sunny Day / Rain Dance (1 if weakened, 3 if strengthened) / 2
				unsigned ff;						// Flash Fire: 3 / 2
				unsigned mf;						// Me First: 3 / 2
				unsigned stab;					// Same Type Attack Bonus: 3 / 2
				unsigned aem;					// Ability Effectiveness Multiplier: Solid Rock (3), Filter (3) / 4
				unsigned eb;						// Expert Belt: 6 / 5
				unsigned tl;							// Tinted Lens (2)
				unsigned rb;						// Resistance berries (2)
				damage = damageknown (attacker, defender, weather, rl, weather_mod, ff, mf);
				damage = damagenonrandom (attacker, defender, rl, weather_mod, ff, mf, stab, effectiveness, aem, eb, tl, rb, damage);
				std::vector <unsigned> effectiveness_vector = get_effectiveness_variables (attacker.pokemon().move().type, defender.pokemon());
				damage = damagerandom (attacker.pokemon(), defender, stab, effectiveness_vector, aem, eb, tl, rb, damage);
			}
		}
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

} // namespace technicalmachine
