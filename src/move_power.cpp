// Move power calculator
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

#include "move_power.hpp"

#include "ability.hpp"
#include "gender.hpp"
#include "item.hpp"
#include "move.hpp"
#include "pokemon.hpp"
#include "status.hpp"
#include "team.hpp"
#include "weather.hpp"

namespace technicalmachine {

void move_power (Team & attacker, Team const & defender, Weather const & weather) {

	// I account for the doubling of the base power for Pursuit in the
	// switching function by simply multiplying the final base power by 2.
	// Regardless of the combination of modifiers, this does not change the
	// final base power. The exception is if the attacker's ally uses Helping
	// Hand. The target uses U-turn and the attacker uses Pursuit with a slower
	// Pokemon that has Rivalry and a Muscle Band and neither the attacker nor
	// target is genderless. This will cause the base power to be 1 less than
	// it should be.

	// Variable power moves
	
	switch (attacker.pokemon().move().name) {
		case Move::CRUSH_GRIP:
		case Move::WRING_OUT:
			attacker.pokemon().move().basepower = 120 * defender.pokemon().hp.stat / defender.pokemon().hp.max + 1;
			break;
		case Move::ERUPTION:
		case Move::WATER_SPOUT:
			attacker.pokemon().move().basepower = 150 * attacker.pokemon().hp.stat / attacker.pokemon().hp.max;
			break;
		case Move::FLAIL:
		case Move::REVERSAL: {
			unsigned k = 64 * attacker.pokemon().hp.stat / attacker.pokemon().hp.max;
			if (k <= 1)
				attacker.pokemon().move().basepower = 200;
			else if (k <= 5)
				attacker.pokemon().move().basepower = 150;
			else if (k <= 12)
				attacker.pokemon().move().basepower = 100;
			else if (k <= 21)
				attacker.pokemon().move().basepower = 80;
			else if (k <= 42)
				attacker.pokemon().move().basepower = 40;
			else
				attacker.pokemon().move().basepower = 20;
			break;
		}
		case Move::FLING:
			attacker.pokemon().move().basepower = attacker.pokemon().item.get_fling_power ();
			break;
		case Move::FRUSTRATION:
			attacker.pokemon().move().basepower = 102 - attacker.pokemon().happiness * 2 / 5;
			break;
		case Move::FURY_CUTTER: {
			unsigned n = attacker.pokemon().move().times_used;
			if (n > 4)
				n = 4;
			// 10 * 2 ^ n
			attacker.pokemon().move().basepower = 10 << n;
			break;
		}
		case Move::GRASS_KNOT:
		case Move::LOW_KICK:
			attacker.pokemon().move().basepower = defender.pokemon().mass;
			break;
		case Move::GYRO_BALL:
			attacker.pokemon().move().basepower = 25 * defender.pokemon().spe.stat / attacker.pokemon().spe.stat + 1;
			if (attacker.pokemon().move().basepower > 150)
				attacker.pokemon().move().basepower = 150;
			break;
		case Move::ICE_BALL:
		case Move::ROLLOUT: {
			unsigned n = attacker.pokemon().move().times_used;
			if (n > 4)
				n = 4;
			// 30 * 2 ^ n
			attacker.pokemon().move().basepower = 30 << n;
			break;
		}
		case Move::HIDDEN_POWER: {
			// The second-least significant bit of each stat determines the power of Hidden Power
			unsigned const u = (attacker.pokemon().hp.iv >> 1) % 2;
			unsigned const v = ((attacker.pokemon().atk.iv >> 1) % 2) * 2;
			unsigned const w = ((attacker.pokemon().def.iv >> 1) % 2) * 4;
			unsigned const x = ((attacker.pokemon().spe.iv >> 1) % 2) * 8;
			unsigned const y = ((attacker.pokemon().spa.iv >> 1) % 2) * 16;
			unsigned const z = ((attacker.pokemon().spd.iv >> 1) % 2) * 32;
	
			attacker.pokemon().move().basepower = (u + v + w + x + y + z) * 40 / 63 + 30;
			break;
		}
		case Move::MAGNITUDE:
			attacker.pokemon().move().basepower = attacker.pokemon().move().variable().first;
			break;
		case Move::NATURAL_GIFT:
			attacker.pokemon().move().basepower = attacker.pokemon().item.get_berry_power ();
			break;
		case Move::PRESENT:
			attacker.pokemon().move().basepower = attacker.pokemon().move().variable().first;
			break;
		case Move::PUNISHMENT:
			attacker.pokemon().move().basepower = 60;
			if (defender.stage [Stat::ATK] > 0)
				attacker.pokemon().move().basepower += 20 * defender.stage [Stat::ATK];
			if (defender.stage [Stat::DEF] > 0)
				attacker.pokemon().move().basepower += 20 * defender.stage [Stat::DEF];
			if (defender.stage [Stat::SPA] > 0)
				attacker.pokemon().move().basepower += 20 * defender.stage [Stat::SPA];
			if (defender.stage [Stat::SPD] > 0)
				attacker.pokemon().move().basepower += 20 * defender.stage [Stat::SPD];
			if (defender.stage [Stat::SPE] > 0)
				attacker.pokemon().move().basepower += 20 * defender.stage [Stat::SPE];
			if (attacker.pokemon().move().basepower > 200)
				attacker.pokemon().move().basepower = 200;
			break;
		case Move::RETURN:
			attacker.pokemon().move().basepower = attacker.pokemon().happiness * 2 / 5;
			break;
		case Move::SPIT_UP:
			attacker.pokemon().move().basepower = attacker.stockpile * 100;
			break;
		case Move::TRIPLE_KICK: {
			unsigned n = attacker.pokemon().move().times_used + 1;
			if (n > 3)
				n = 3;
			attacker.pokemon().move().basepower = 10 * n;
			break;
		}
		case Move::TRUMP_CARD:
			switch (attacker.pokemon().move().pp) {
				case 0:
					attacker.pokemon().move().basepower = 200;
					break;
				case 1:
					attacker.pokemon().move().basepower = 80;
					break;
				case 2:
					attacker.pokemon().move().basepower = 60;
					break;
				case 3:
					attacker.pokemon().move().basepower = 50;
					break;
				default:
					attacker.pokemon().move().basepower = 40;
					break;
			}
			break;
		default:
			break;
	}
	attacker.pokemon().move().power = attacker.pokemon().move().basepower;

	bool doubling = false;
	switch (attacker.pokemon().move().name) {
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
			if (defender.pokemon().hp.stat <= defender.pokemon().hp.max / 2)
				doubling = true;
			break;
		case Move::EARTHQUAKE:
		case Move::MAGNITUDE:
			if (defender.vanish == DUG)
				doubling = true;
			break;
		case Move::FACADE:
			switch (attacker.pokemon().status.name) {
				case Status::BURN:
				case Status::PARALYSIS:
				case Status::POISON:
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
			if (defender.pokemon().status.name == Status::PARALYSIS)
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
			if (defender.pokemon().status.is_sleeping ())
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
		attacker.pokemon().move().power *= 2;

	unsigned item_modifier = 10;
	switch (attacker.pokemon().item.name) {
		case Item::MUSCLE_BAND:
			if (attacker.pokemon().move().physical)
				item_modifier = 11;
			break;
		case Item::WISE_GLASSES:
			if (!attacker.pokemon().move().physical)
				item_modifier = 11;
			break;
		case Item::INSECT_PLATE:
		case Item::SILVERPOWDER:
			if (attacker.pokemon().move().type == Type::BUG)
				item_modifier = 12;
			break;
		case Item::DREAD_PLATE:	
		case Item::BLACKGLASSES:
			if (attacker.pokemon().move().type == Type::DARK)
				item_modifier = 12;
			break;
		case Item::DRACO_PLATE:
		case Item::DRAGON_FANG:
			if (attacker.pokemon().move().type == Type::DRAGON)
				item_modifier = 12;
			break;
		case Item::ZAP_PLATE:
		case Item::MAGNET:
			if (attacker.pokemon().move().type == Type::ELECTRIC)
				item_modifier = 12;
			break;
		case Item::FIST_PLATE:
		case Item::BLACK_BELT:
			if (attacker.pokemon().move().type == Type::FIGHTING)
				item_modifier = 12;
			break;
		case Item::FLAME_PLATE:
		case Item::CHARCOAL:
			if (attacker.pokemon().move().type == Type::FIRE)
				item_modifier = 12;
			break;
		case Item::SKY_PLATE:
		case Item::SHARP_BEAK:
			if (attacker.pokemon().move().type == Type::FLYING)
				item_modifier = 12;
			break;
		case Item::SPOOKY_PLATE:
		case Item::SPELL_TAG:
			if (attacker.pokemon().move().type == Type::GHOST)
 				item_modifier = 12;
			break;
		case Item::MEADOW_PLATE:
		case Item::MIRACLE_SEED:
			if (attacker.pokemon().move().type == Type::GRASS)
				item_modifier = 12;
			break;
		case Item::EARTH_PLATE:
		case Item::SOFT_SAND:
			if (attacker.pokemon().move().type == Type::GROUND)
				item_modifier = 12;
			break;
		case Item::ICICLE_PLATE:
		case Item::NEVERMELTICE:
			if (attacker.pokemon().move().type == Type::ICE)
				item_modifier = 12;
			break;
		case Item::SILK_SCARF:
			if (attacker.pokemon().move().type == Type::NORMAL)
				item_modifier = 12;
			break;
		case Item::TOXIC_PLATE:
		case Item::POISON_BARB:
			if (attacker.pokemon().move().type == Type::POISON)
				item_modifier = 12;
			break;
		case Item::MIND_PLATE:
		case Item::TWISTEDSPOON:
		case Item::ODD_INCENSE:
			if (attacker.pokemon().move().type == Type::PSYCHIC)
				item_modifier = 12;
			break;
		case Item::STONE_PLATE:
		case Item::HARD_STONE:
		case Item::ROCK_INCENSE:
			if (attacker.pokemon().move().type == Type::ROCK)
				item_modifier = 12;
			break;
		case Item::IRON_PLATE:
		case Item::METAL_COAT:
			if (attacker.pokemon().move().type == Type::STEEL)
				item_modifier = 12;
			break;
		case Item::SPLASH_PLATE:
		case Item::MYSTIC_WATER:
		case Item::SEA_INCENSE:
		case Item::WAVE_INCENSE:
			if (attacker.pokemon().move().type == Type::WATER)
				item_modifier = 12;
			break;
		case Item::ADAMANT_ORB:
			if (attacker.pokemon().name == DIALGA and (attacker.pokemon().move().type == Type::DRAGON or attacker.pokemon().move().type == Type::STEEL))
				item_modifier = 12;
			break;
		case Item::GRISEOUS_ORB:
			if (attacker.pokemon().name == GIRATINA_O and (attacker.pokemon().move().type == Type::DRAGON or attacker.pokemon().move().type == Type::GHOST))
				item_modifier = 12;
			break;
		case Item::LUSTROUS_ORB:
			if (attacker.pokemon().name == PALKIA and (attacker.pokemon().move().type == Type::DRAGON or attacker.pokemon().move().type == Type::WATER))
				item_modifier = 12;
			break;
		default:
			break;
	}
	attacker.pokemon().move().power = attacker.pokemon().move().power * item_modifier / 10;

	if (attacker.charge and attacker.pokemon().move().type == Type::ELECTRIC)
		attacker.pokemon().move().power *= 2;

	if ((defender.mud_sport and attacker.pokemon().move().type == Type::ELECTRIC) or (defender.water_sport and attacker.pokemon().move().type == Type::FIRE))
		attacker.pokemon().move().power /= 2;

	switch (attacker.pokemon().ability.name) {
		case Ability::TECHNICIAN:
			if (attacker.pokemon().move().basepower <= 60)
				attacker.pokemon().move().power = attacker.pokemon().move().power * 3 / 2;
			break;
		case Ability::BLAZE:
			if (attacker.pokemon().move().type == Type::FIRE and attacker.pokemon().hp.stat <= attacker.pokemon().hp.max / 3)
				attacker.pokemon().move().power = attacker.pokemon().move().power * 3 / 2;
			break;
		case Ability::OVERGROW:
			if (attacker.pokemon().move().type == Type::GRASS and attacker.pokemon().hp.stat <= attacker.pokemon().hp.max / 3)
				attacker.pokemon().move().power = attacker.pokemon().move().power * 3 / 2;
			break;
		case Ability::SWARM:
			if (attacker.pokemon().move().type == Type::BUG and attacker.pokemon().hp.stat <= attacker.pokemon().hp.max / 3)
				attacker.pokemon().move().power = attacker.pokemon().move().power * 3 / 2;
			break;
		case Ability::TORRENT:
			if (attacker.pokemon().move().type == Type::WATER and attacker.pokemon().hp.stat <= attacker.pokemon().hp.max / 3)
				attacker.pokemon().move().power = attacker.pokemon().move().power * 3 / 2;
			break;
		case Ability::IRON_FIST:
			switch (attacker.pokemon().move().name) {
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
					attacker.pokemon().move().power = attacker.pokemon().move().power * 6 / 5;
					break;
				default:
					break;
			}
		case Ability::RECKLESS:
			switch (attacker.pokemon().move().name) {
				case Move::BRAVE_BIRD:
				case Move::DOUBLE_EDGE:
				case Move::FLARE_BLITZ:
				case Move::HEAD_SMASH:
				case Move::SUBMISSION:
				case Move::TAKE_DOWN:
				case Move::VOLT_TACKLE:
				case Move::WOOD_HAMMER:
					attacker.pokemon().move().power = attacker.pokemon().move().power * 6 / 5;
					break;
				default:
					break;
		}
		case Ability::RIVALRY:
			// Same gender == 20 + 5, opposite gender == 20 - 5
			attacker.pokemon().move().power = attacker.pokemon().move().power * (20 + 5 * attacker.pokemon().gender.multiplier (defender.pokemon().gender)) / 20;
			break;
		default:
			break;
	}

	switch (defender.pokemon().ability.name) {
		case Ability::DRY_SKIN:
			if (attacker.pokemon().move().type == Type::FIRE)
				attacker.pokemon().move().power = attacker.pokemon().move().power * 5 / 4;
			break;
		case Ability::HEATPROOF:
			if (attacker.pokemon().move().type == Type::FIRE)
				attacker.pokemon().move().power /= 2;
			break;
		case Ability::THICK_FAT:
			if (attacker.pokemon().move().type == Type::FIRE or attacker.pokemon().move().type == Type::ICE)
				attacker.pokemon().move().power /= 2;
			break;
		default:
			break;
	}

	if (attacker.pokemon().move().power == 0)
		attacker.pokemon().move().power = 1;
}

}	// namespace technicalmachine
