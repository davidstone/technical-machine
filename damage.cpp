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

#include "damage.h"

#include <vector>

#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "status.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void movepower (Team & attacker, Team const & defender, Weather const & weather) {

	// I account for the doubling of the base power for Pursuit in the switching function by simply multiplying the final base power by 2. Regardless of the combination of modifiers, this does not change the final base power. The exception is if the attacker's ally uses Helping Hand. The target uses U-turn and the attacker uses Pursuit with a slower Pokemon that has Rivalry and a Muscle Band and neither the attacker nor target is genderless. This will cause the base power to be 1 less than it should be.

	// Variable power moves
	
	switch (attacker.pokemon->move->name) {
		case Move::CRUSH_GRIP:
		case Move::WRING_OUT:
			attacker.pokemon->move->basepower = 120 * defender.pokemon->hp.stat / defender.pokemon->hp.max + 1;
			break;
		case Move::ERUPTION:
		case Move::WATER_SPOUT:
			attacker.pokemon->move->basepower = 150 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max;
			break;
		case Move::FLAIL:
		case Move::REVERSAL: {
			unsigned k = 64 * attacker.pokemon->hp.stat / attacker.pokemon->hp.max;
			if (k <= 1)
				attacker.pokemon->move->basepower = 200;
			else if (k <= 5)
				attacker.pokemon->move->basepower = 150;
			else if (k <= 12)
				attacker.pokemon->move->basepower = 100;
			else if (k <= 21)
				attacker.pokemon->move->basepower = 80;
			else if (k <= 42)
				attacker.pokemon->move->basepower = 40;
			else
				attacker.pokemon->move->basepower = 20;
			break;
		}
		case Move::FLING:
			attacker.pokemon->move->basepower = attacker.pokemon->item.get_fling_power ();
			break;
		case Move::FRUSTRATION:
			attacker.pokemon->move->basepower = 102 - attacker.pokemon->happiness * 2 / 5;
			break;
		case Move::FURY_CUTTER:
			// 10 * 2 ^ attacker.pokemon->move->times_used
			attacker.pokemon->move->basepower = 10 << attacker.pokemon->move->times_used;
			break;
		case Move::GRASS_KNOT:
		case Move::LOW_KICK:
			attacker.pokemon->move->basepower = defender.pokemon->mass;
			break;
		case Move::GYRO_BALL:
			attacker.pokemon->move->basepower = 25 * defender.pokemon->spe.stat / attacker.pokemon->spe.stat + 1;
			if (attacker.pokemon->move->basepower > 150)
				attacker.pokemon->move->basepower = 150;
			break;
		case Move::ICE_BALL:
		case Move::ROLLOUT:
			// 30 * 2 ^ attacker.pokemon->move->times_used
			attacker.pokemon->move->basepower = 30 << attacker.pokemon->move->times_used;
			break;
		case Move::HIDDEN_POWER: {
			// The second-least significant bit of each stat determines the power of Hidden Power
			unsigned const u = (attacker.pokemon->hp.iv >> 1) % 2;
			unsigned const v = ((attacker.pokemon->atk.iv >> 1) % 2) * 2;
			unsigned const w = ((attacker.pokemon->def.iv >> 1) % 2) * 4;
			unsigned const x = ((attacker.pokemon->spe.iv >> 1) % 2) * 8;
			unsigned const y = ((attacker.pokemon->spa.iv >> 1) % 2) * 16;
			unsigned const z = ((attacker.pokemon->spd.iv >> 1) % 2) * 32;
	
			attacker.pokemon->move->basepower = (u + v + w + x + y + z) * 40 / 63 + 30;
			break;
		}
		case Move::MAGNITUDE:
			attacker.pokemon->move->basepower = attacker.pokemon->move->variable->first;
			break;
		case Move::NATURAL_GIFT:
			attacker.pokemon->move->basepower = attacker.pokemon->item.get_berry_power ();
			break;
		case Move::PRESENT:
			attacker.pokemon->move->basepower = attacker.pokemon->move->variable->first;
			break;
		case Move::PUNISHMENT:
			attacker.pokemon->move->basepower = 60;
			if (defender.stage [Stat::ATK] > 0)
				attacker.pokemon->move->basepower += 20 * defender.stage [Stat::ATK];
			if (defender.stage [Stat::DEF] > 0)
				attacker.pokemon->move->basepower += 20 * defender.stage [Stat::DEF];
			if (defender.stage [Stat::SPA] > 0)
				attacker.pokemon->move->basepower += 20 * defender.stage [Stat::SPA];
			if (defender.stage [Stat::SPD] > 0)
				attacker.pokemon->move->basepower += 20 * defender.stage [Stat::SPD];
			if (defender.stage [Stat::SPE] > 0)
				attacker.pokemon->move->basepower += 20 * defender.stage [Stat::SPE];
			if (attacker.pokemon->move->basepower > 200)
				attacker.pokemon->move->basepower = 200;
			break;
		case Move::RETURN:
			attacker.pokemon->move->basepower = attacker.pokemon->happiness * 2 / 5;
			break;
		case Move::SPIT_UP:
			attacker.pokemon->move->basepower = attacker.stockpile * 100;
			break;
		case Move::TRIPLE_KICK:
			attacker.pokemon->move->basepower = 10 * attacker.pokemon->move->times_used;
			break;
		case Move::TRUMP_CARD:
			switch (attacker.pokemon->move->pp) {
				case 0:
					attacker.pokemon->move->basepower = 200;
					break;
				case 1:
					attacker.pokemon->move->basepower = 80;
					break;
				case 2:
					attacker.pokemon->move->basepower = 60;
					break;
				case 3:
					attacker.pokemon->move->basepower = 50;
					break;
				default:
					attacker.pokemon->move->basepower = 40;
					break;
			}
			break;
		default:
			break;
	}
	attacker.pokemon->move->power = attacker.pokemon->move->basepower;

	bool doubling = false;
	switch (attacker.pokemon->move->name) {
		case Move::ASSURANCE:
			if (defender.damaged)
				doubling = true;
			break;
		case Move::AVALANCHE: 
		case Move::REVENGE:
			if (attacker.damaged)
				doubling = true;
			break;
		case Move::BRINE:
			if (defender.pokemon->hp.stat <= defender.pokemon->hp.max / 2)
				doubling = true;
			break;
		case Move::EARTHQUAKE:
		case Move::MAGNITUDE:
			if (defender.vanish == DUG)
				doubling = true;
			break;
		case Move::FACADE:
			switch (attacker.pokemon->status.name) {
				case Status::BURN:
				case Status::PARALYSIS:
				case Status::POISON_NORMAL:
				case Status::POISON_TOXIC:
					doubling = true;
					break;
				default:
					break;
			}
			break;
		case Move::GUST:
			if (defender.vanish == BOUNCED or defender.vanish == FLOWN)
				doubling = true;
			break;
		case Move::ICE_BALL:
		case Move::ROLLOUT:
			if (attacker.defense_curl)
				doubling = true;
			break;
		case Move::PAYBACK:
			if (defender.moved)
				doubling = true;
			break;
		case Move::SMELLINGSALT:
			if (defender.pokemon->status.name == Status::PARALYSIS)
				doubling = true;
			break;
		case Move::SOLARBEAM:
			if (weather.rain == 0)
				doubling = true;
			break;
		case Move::STOMP:
			if (defender.minimize)
				doubling = true;
			break;
		case Move::SURF:
			if (defender.vanish == DIVED)
				doubling = true;
			break;
		case Move::WAKE_UP_SLAP:
			if (defender.pokemon->status.is_sleeping ())
				doubling = true;
			break;
		case Move::WEATHER_BALL:
			if (weather.hail or weather.rain or weather.sand or weather.sun)
				doubling = true;
			break;
		default:
			break;
	}
	if (doubling)
		attacker.pokemon->move->power *= 2;

	unsigned item_modifier = 10;
	switch (attacker.pokemon->item.name) {
		case Item::MUSCLE_BAND:
			if (attacker.pokemon->move->physical)
				item_modifier = 11;
			break;
		case Item::WISE_GLASSES:
			if (!attacker.pokemon->move->physical)
				item_modifier = 11;
			break;
		case Item::INSECT_PLATE:
		case Item::SILVERPOWDER:
			if (attacker.pokemon->move->type == BUG)
				item_modifier = 12;
			break;
		case Item::DREAD_PLATE:	
		case Item::BLACKGLASSES:
			if (attacker.pokemon->move->type == DARK)
				item_modifier = 12;
			break;
		case Item::DRACO_PLATE:
		case Item::DRAGON_FANG:
			if (attacker.pokemon->move->type == DRAGON)
				item_modifier = 12;
			break;
		case Item::ZAP_PLATE:
		case Item::MAGNET:
			if (attacker.pokemon->move->type == ELECTRIC)
				item_modifier = 12;
			break;
		case Item::FIST_PLATE:
		case Item::BLACK_BELT:
			if (attacker.pokemon->move->type == FIGHTING)
				item_modifier = 12;
			break;
		case Item::FLAME_PLATE:
		case Item::CHARCOAL:
			if (attacker.pokemon->move->type == FIRE)
				item_modifier = 12;
			break;
		case Item::SKY_PLATE:
		case Item::SHARP_BEAK:
			if (attacker.pokemon->move->type == FLYING)
				item_modifier = 12;
			break;
		case Item::SPOOKY_PLATE:
		case Item::SPELL_TAG:
			if (attacker.pokemon->move->type == GHOST)
 				item_modifier = 12;
			break;
		case Item::MEADOW_PLATE:
		case Item::MIRACLE_SEED:
			if (attacker.pokemon->move->type == GRASS)
				item_modifier = 12;
			break;
		case Item::EARTH_PLATE:
		case Item::SOFT_SAND:
			if (attacker.pokemon->move->type == GROUND)
				item_modifier = 12;
			break;
		case Item::ICICLE_PLATE:
		case Item::NEVERMELTICE:
			if (attacker.pokemon->move->type == ICE)
				item_modifier = 12;
			break;
		case Item::SILK_SCARF:
			if (attacker.pokemon->move->type == NORMAL)
				item_modifier = 12;
			break;
		case Item::TOXIC_PLATE:
		case Item::POISON_BARB:
			if (attacker.pokemon->move->type == POISON)
				item_modifier = 12;
			break;
		case Item::MIND_PLATE:
		case Item::TWISTEDSPOON:
		case Item::ODD_INCENSE:
			if (attacker.pokemon->move->type == PSYCHIC_TYPE)
				item_modifier = 12;
			break;
		case Item::STONE_PLATE:
		case Item::HARD_STONE:
		case Item::ROCK_INCENSE:
			if (attacker.pokemon->move->type == ROCK)
				item_modifier = 12;
			break;
		case Item::IRON_PLATE:
		case Item::METAL_COAT:
			if (attacker.pokemon->move->type == STEEL)
				item_modifier = 12;
			break;
		case Item::SPLASH_PLATE:
		case Item::MYSTIC_WATER:
		case Item::SEA_INCENSE:
		case Item::WAVE_INCENSE:
			if (attacker.pokemon->move->type == WATER)
				item_modifier = 12;
			break;
		case Item::ADAMANT_ORB:
			if (attacker.pokemon->name == DIALGA and (attacker.pokemon->move->type == DRAGON or attacker.pokemon->move->type == STEEL))
				item_modifier = 12;
			break;
		case Item::GRISEOUS_ORB:
			if (attacker.pokemon->name == GIRATINA_O and (attacker.pokemon->move->type == DRAGON or attacker.pokemon->move->type == GHOST))
				item_modifier = 12;
			break;
		case Item::LUSTROUS_ORB:
			if (attacker.pokemon->name == PALKIA and (attacker.pokemon->move->type == DRAGON or attacker.pokemon->move->type == WATER))
				item_modifier = 12;
			break;
		default:
			break;
	}
	attacker.pokemon->move->power = attacker.pokemon->move->power * item_modifier / 10;

	if (attacker.charge and attacker.pokemon->move->type == ELECTRIC)
		attacker.pokemon->move->power *= 2;

	if ((defender.mud_sport and attacker.pokemon->move->type == ELECTRIC) or (defender.water_sport and attacker.pokemon->move->type == FIRE))
		attacker.pokemon->move->power /= 2;

	switch (attacker.pokemon->ability.name) {
		case Ability::TECHNICIAN:
			if (attacker.pokemon->move->basepower <= 60)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case Ability::BLAZE:
			if (attacker.pokemon->move->type == FIRE and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case Ability::OVERGROW:
			if (attacker.pokemon->move->type == GRASS and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case Ability::SWARM:
			if (attacker.pokemon->move->type == BUG and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case Ability::TORRENT:
			if (attacker.pokemon->move->type == WATER and attacker.pokemon->hp.stat <= attacker.pokemon->hp.max / 3)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 3 / 2;
			break;
		case Ability::IRON_FIST:
			switch (attacker.pokemon->move->name) {
				case Move::BULLET_PUNCH:
				case Move::COMET_PUNCH:
				case Move::DIZZY_PUNCH:
				case Move::DRAIN_PUNCH:
				case Move::DYNAMICPUNCH:
				case Move::FIRE_PUNCH:
				case Move::FOCUS_PUNCH:
				case Move::HAMMER_ARM:
				case Move::ICE_PUNCH:
				case Move::MACH_PUNCH:
				case Move::MEGA_PUNCH:
				case Move::METEOR_MASH:
				case Move::SHADOW_PUNCH:
				case Move::SKY_UPPERCUT:
				case Move::THUNDERPUNCH:
					attacker.pokemon->move->power = attacker.pokemon->move->power * 6 / 5;
					break;
				default:
					break;
			}
		case Ability::RECKLESS:
			switch (attacker.pokemon->move->name) {
				case Move::BRAVE_BIRD:
				case Move::DOUBLE_EDGE:
				case Move::FLARE_BLITZ:
				case Move::HEAD_SMASH:
				case Move::SUBMISSION:
				case Move::TAKE_DOWN:
				case Move::VOLT_TACKLE:
				case Move::WOOD_HAMMER:
					attacker.pokemon->move->power = attacker.pokemon->move->power * 6 / 5;
					break;
				default:
					break;
		}
		case Ability::RIVALRY:
			// Same gender == 20 + 5, opposite gender == 20 - 5
			attacker.pokemon->move->power = attacker.pokemon->move->power * (20 + 5 * attacker.pokemon->gender.multiplier (defender.pokemon->gender)) / 20;
			break;
		default:
			break;
	}

	switch (defender.pokemon->ability.name) {
		case Ability::DRY_SKIN:
			if (attacker.pokemon->move->type == FIRE)
				attacker.pokemon->move->power = attacker.pokemon->move->power * 5 / 4;
			break;
		case Ability::HEATPROOF:
			if (attacker.pokemon->move->type == FIRE)
				attacker.pokemon->move->power /= 2;
			break;
		case Ability::THICK_FAT:
			if (attacker.pokemon->move->type == FIRE or attacker.pokemon->move->type == ICE)
				attacker.pokemon->move->power /= 2;
			break;
		default:
			break;
	}

	if (attacker.pokemon->move->power == 0)
		attacker.pokemon->move->power = 1;
}

// I split my damage calculator up into a function that calculates as much as possible with known data, one that calculates without the random number, and a function that does the rest of the work because in many cases, I have the damage calculator in a deep inner loop, and pre-calculating non-random numbers allows me to move much of that calculator to a shallower part of code, and pre-calculating known information moves even more out. Profiling showed this to be a sound optimization.

unsigned damageknown (Team const & attacker, Team const & defender, Weather const & weather, unsigned & rl, unsigned & weather_mod, unsigned & ff, unsigned & mf) {
	if (((defender.reflect and attacker.pokemon->move->physical)
			or (defender.light_screen and !attacker.pokemon->move->physical))
			and !attacker.ch)
		rl = 2;
	else
		rl = 1;

	if ((weather.rain and attacker.pokemon->move->type == WATER)
			or (weather.sun and attacker.pokemon->move->type == FIRE))
		weather_mod = 3;
	else if ((weather.rain and attacker.pokemon->move->type == FIRE)
			or (weather.sun and attacker.pokemon->move->type == WATER))
		weather_mod = 1;
	else
		weather_mod = 2;

	if (attacker.ff and attacker.pokemon->move->type == FIRE)
		ff = 3;
	else
		ff = 2;

	if (attacker.mf)
		mf = 3;
	else
		mf = 2;

	return attacker.pokemon->level * 2 / 5 + 2;
}

unsigned damagenonrandom (Team const & attacker, Team const & defender, unsigned rl, unsigned weather_mod, unsigned ff, unsigned mf, unsigned & stab, unsigned effectiveness, unsigned & aem, unsigned & eb, unsigned & tl, unsigned & rb, unsigned damage) {

	damage *= attacker.pokemon->move->power;

	if (attacker.pokemon->move->physical) {
		damage = damage * attacker.pokemon->atk.stat / 50 / defender.pokemon->def.stat;
		if (attacker.pokemon->status.name == Status::BURN and attacker.pokemon->ability.name != Ability::GUTS)
			damage /= 2;
	}
	else
		damage = damage * attacker.pokemon->spa.stat / 50 / defender.pokemon->spd.stat;

	damage = damage / rl * weather_mod / 2 * ff / 2 + 2;

	if (attacker.ch) {
		if (attacker.pokemon->ability.name == Ability::SNIPER)
			damage *= 3;
		else
			damage *= 2;
	}
	
	switch (attacker.pokemon->item.name) {
		case Item::LIFE_ORB:
			damage = damage * 13 / 10;
			break;
		case Item::METRONOME:
			if (attacker.pokemon->move->times_used >= 10)
				damage *= 2;
			else
				damage = damage * (10 + attacker.pokemon->move->times_used) / 10;
			break;
		default:
			break;
	}

	damage = damage * mf / 2;

	if (is_type (attacker, attacker.pokemon->move->type)) {
		if (attacker.pokemon->ability.name == Ability::ADAPTABILITY)
			stab = 4;
		else
			stab = 3;
	}
	else
		stab = 2;

	if (defender.pokemon->ability.weakens_SE_attacks () and effectiveness > 4)
		aem = 3;
	else
		aem = 4;

	if (attacker.pokemon->item.name == Item::EXPERT_BELT and effectiveness > 4)
		eb = 6;
	else
		eb = 5;

	if (attacker.pokemon->ability.name == Ability::TINTED_LENS and effectiveness < 4)
		tl = 2;
	else
		tl = 1;

	rb = 1;
	if (defender.pokemon->item.name == Item::CHILAN_BERRY and attacker.pokemon->move->type == NORMAL)
		rb = 2;
	else if (effectiveness > 4) {
		switch (attacker.pokemon->item.name) {
			case Item::BABIRI_BERRY:
				if (attacker.pokemon->move->type == STEEL)
					rb = 2;
				break;
			case Item::CHARTI_BERRY:
				if (attacker.pokemon->move->type == ROCK)
					rb = 2;
				break;
			case Item::CHOPLE_BERRY:
				if (attacker.pokemon->move->type == FIGHTING)
					rb = 2;
				break;
			case Item::COBA_BERRY:
				if (attacker.pokemon->move->type == FLYING)
					rb = 2;
				break;
			case Item::COLBUR_BERRY:
				if (attacker.pokemon->move->type == DARK)
					rb = 2;
				break;
			case Item::HABAN_BERRY:
				if (attacker.pokemon->move->type == DRAGON)
					rb = 2;
				break;
			case Item::KASIB_BERRY:
				if (attacker.pokemon->move->type == GHOST)
					rb = 2;
				break;
			case Item::KEBIA_BERRY:
				if (attacker.pokemon->move->type == POISON)
					rb = 2;
				break;
			case Item::OCCA_BERRY:
				if (attacker.pokemon->move->type == FIRE)
					rb = 2;
				break;
			case Item::PASSHO_BERRY:
				if (attacker.pokemon->move->type == WATER)
					rb = 2;
				break;
			case Item::PAYAPA_BERRY:
				if (attacker.pokemon->move->type == PSYCHIC_TYPE)
					rb = 2;
				break;
			case Item::RINDO_BERRY:
				if (attacker.pokemon->move->type == GRASS)
					rb = 2;
				break;
			case Item::SHUCA_BERRY:
				if (attacker.pokemon->move->type == GROUND)
					rb = 2;
				break;
			case Item::TANGA_BERRY:
				if (attacker.pokemon->move->type == BUG)
					rb = 2;
				break;
			case Item::WACAN_BERRY:
				if (attacker.pokemon->move->type == ELECTRIC)
					rb = 2;
				break;
			case Item::YACHE_BERRY:
				if (attacker.pokemon->move->type == ICE)
					rb = 2;
				break;
			default:
				break;
		}
	}

	return damage;
}

unsigned damagerandom (Pokemon const & attacker, Team const & defender, unsigned stab, std::vector <unsigned> const & effectiveness, unsigned aem, unsigned eb, unsigned tl, unsigned rb, unsigned damage) {
	damage = damage * attacker.move->r / 100 * stab / 2;
	for (std::vector <unsigned>::const_iterator it = effectiveness.begin(); it != effectiveness.end(); ++it)
		damage = damage * *it / 2;
	damage = damage * aem / 4 * eb / 5 * tl / rb;
	if (damage == 0)
		damage = 1;
	else if (damage >= defender.pokemon->hp.stat) {
		damage = defender.pokemon->hp.stat;
		if (attacker.move->name == Move::FALSE_SWIPE or defender.endure)
			--damage;
	}
	return damage;
}

unsigned damagecalculator (Team const & attacker, Team const & defender, Weather const & weather) {
	unsigned damage = 0;
	unsigned effectiveness = get_effectiveness (attacker.pokemon->move->type, *defender.pokemon);
	if ((effectiveness > 0) and (attacker.pokemon->move->type != GROUND or grounded (defender, weather))) {
		switch (attacker.pokemon->move->name) {
			case Move::DRAGON_RAGE:
				damage = 40;
				break;
			case Move::ENDEAVOR:
				if (defender.pokemon->hp.stat > attacker.pokemon->hp.stat)
					damage = defender.pokemon->hp.stat - attacker.pokemon->hp.stat;
				else
					damage = 0;
				break;
			case Move::FISSURE:
			case Move::GUILLOTINE:
			case Move::HORN_DRILL:
			case Move::SHEER_COLD:
				damage = defender.pokemon->hp.max;
				break;
			case Move::NIGHT_SHADE:
			case Move::SEISMIC_TOSS:
				damage = attacker.pokemon->level;
				break;
			case Move::PSYWAVE:
				damage = attacker.pokemon->level * attacker.pokemon->move->variable->first / 10;
				break;
			case Move::SONICBOOM:
				damage = 20;
				break;
			case Move::SUPER_FANG:
				damage = defender.pokemon->hp.stat / 2;

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
				std::vector <unsigned> effectiveness_vector = get_effectiveness_variables (attacker.pokemon->move->type, *defender.pokemon);
				damage = damagerandom (*attacker.pokemon, defender, stab, effectiveness_vector, aem, eb, tl, rb, damage);
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

}
