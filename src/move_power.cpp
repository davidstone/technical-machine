// Move power calculator
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

#include "move_power.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "ability.hpp"
#include "gender.hpp"
#include "item.hpp"
#include "move.hpp"
#include "pokemon.hpp"
#include "status.hpp"
#include "team.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {
class Rational {
	public:
		explicit Rational(unsigned const n = 1, unsigned const d = 1):
			numerator(n),
			denominator(d) {
		}
		template<typename Integer>
		friend Integer operator*=(Integer & number, Rational const rational) {
			number *= rational.numerator;
			number /= rational.denominator;
			return number;
		}
		template<typename Integer>
		friend Integer operator*(Integer const number, Rational const rational) {
			return number *= rational;
		}
		template<typename Integer>
		friend Integer operator*(Rational rational, Integer const number) {
			return rational * number;
		}
	private:
		unsigned numerator;
		unsigned denominator;
};

uint16_t calculate_base_power (Pokemon const & attacker, Team const & defender, int stockpile);
uint8_t second_lowest_bit (Stat const & stat);
bool doubling (Team const & attacker, Team const & defender, Weather const & weather);
unsigned item_modifier (Pokemon const & attacker);
bool mud_or_water_sport (Team const & attacker, Team const & defender);
Rational attacker_ability_modifier (Pokemon const & attacker, Pokemon const & defender);
bool pinch_ability_activates (Pokemon const & attacker, Type::Types type);
Rational defender_ability_modifier (Move const & move, Ability ability);

}	// anonymous namespace

unsigned move_power (Team const & attacker, Team const & defender, Weather const & weather) {
	unsigned power = calculate_base_power(attacker.pokemon(), defender, attacker.stockpile);

	if (doubling (attacker, defender, weather))
		power *= 2;

	power = power * item_modifier (attacker.pokemon()) / 10;

	if (attacker.charge and attacker.pokemon().move().type() == Type::ELECTRIC)
		power *= 2;

	if (mud_or_water_sport (attacker, defender))
		power /= 2;

	power *= attacker_ability_modifier(attacker.pokemon(), defender.pokemon());
	power *= defender_ability_modifier (attacker.pokemon().move(), defender.pokemon().ability);
	
	if (power == 0)
		power = 1;
	return power;
}

namespace {

uint16_t calculate_base_power (Pokemon const & attacker, Team const & defender, int stockpile) {
	switch (attacker.move().name) {
		case Move::CRUSH_GRIP:
		case Move::WRING_OUT:
			return 120 * defender.pokemon().hp.stat / defender.pokemon().hp.max + 1;
		case Move::ERUPTION:
		case Move::WATER_SPOUT:
			return 150 * attacker.hp.stat / attacker.hp.max;
		case Move::FLAIL:
		case Move::REVERSAL: {
			unsigned const k = 64u * attacker.hp.stat / attacker.hp.max;
			if (k <= 1)
				return 200;
			else if (k <= 5)
				return 150;
			else if (k <= 12)
				return 100;
			else if (k <= 21)
				return 80;
			else if (k <= 42)
				return 40;
			else
				return 20;
		}
		case Move::FLING:
			return attacker.item.get_fling_power ();
		case Move::FRUSTRATION:
			return 102 - attacker.happiness * 2 / 5;
		case Move::FURY_CUTTER:
			// 10 * 2 ^ n
			return 10 << std::min (static_cast<unsigned> (attacker.move().times_used), 4u);
		case Move::GRASS_KNOT:
		case Move::LOW_KICK:
			return defender.pokemon().mass();
		case Move::GYRO_BALL: {
			uint16_t const base_power = 25 * defender.pokemon().spe.stat / attacker.spe.stat + 1;
			return (base_power <= 150) ? base_power : 150;
		}
		case Move::ICE_BALL:
		case Move::ROLLOUT:
			// 30 * 2 ^ n
			return 30 << std::min (static_cast<unsigned> (attacker.move().times_used), 4u);
		case Move::HIDDEN_POWER: {
			uint8_t const u = second_lowest_bit (attacker.hp) * (1 << 0);	// 1
			uint8_t const v = second_lowest_bit (attacker.atk) * (1 << 1);	// 2
			uint8_t const w = second_lowest_bit (attacker.def) * (1 << 2);	// 4
			uint8_t const x = second_lowest_bit (attacker.spe) * (1 << 3);	// 8
			uint8_t const y = second_lowest_bit (attacker.spa) * (1 << 4);	// 16
			uint8_t const z = second_lowest_bit (attacker.spd) * (1 << 5);	// 32
			return (u + v + w + x + y + z) * 40 / 63 + 30;
		}
		case Move::MAGNITUDE:
			return attacker.move().variable().value();
		case Move::NATURAL_GIFT:
			return attacker.item.get_berry_power ();
		case Move::PRESENT:
			assert (!attacker.move().variable().present_heals());
			return attacker.move().variable().value();
		case Move::PUNISHMENT: {
			auto const positive_values = [](int const stage) { return std::max(stage, 0); };
			uint16_t const base_power = 60 + 20 * defender.stage.accumulate(positive_values);
			return std::min(base_power, static_cast<uint16_t>(200));
		}
		case Move::RETURN:
			return attacker.happiness * 2 / 5;
		case Move::SPIT_UP:
			return stockpile * 100;
		case Move::TRIPLE_KICK:
			return 10 * std::min (static_cast<unsigned> (attacker.move().times_used), 3u);
		case Move::TRUMP_CARD:
			switch (attacker.move().pp) {
				case 0:
					return 200;
				case 1:
					return 80;
				case 2:
					return 60;
				case 3:
					return 50;
				default:
					return 40;
			}
		default:
			return attacker.move().base_power();
	}
}

uint8_t second_lowest_bit (Stat const & stat) {
	return (stat.iv >> 1) % 2;
}

bool doubling (Team const & attacker, Team const & defender, Weather const & weather) {
	// I account for the doubling of the base power for Pursuit in the
	// switching function by simply multiplying the final base power by 2.
	// Regardless of the combination of modifiers, this does not change the
	// final base power. The exception is if the attacker's ally uses Helping
	// Hand, the target uses U-turn, and the attacker uses Pursuit with a
	// slower Pokemon that has Rivalry and a Muscle Band and neither the
	// attacker nor target is genderless. This will cause the base power to be
	// 1 less than it should be.

	switch (attacker.pokemon().move().name) {
		case Move::ASSURANCE:
			return defender.damaged;
		case Move::AVALANCHE: 
		case Move::REVENGE:
			return attacker.damaged;
		case Move::BRINE:
			return defender.pokemon().hp.stat <= defender.pokemon().hp.max / 2;
		case Move::EARTHQUAKE:
		case Move::MAGNITUDE:
			return defender.vanish == DUG;
		case Move::FACADE:
			return attacker.pokemon().status.boosts_facade();
		case Move::GUST:
			return defender.vanish == BOUNCED or defender.vanish == FLOWN;
		case Move::ICE_BALL:
		case Move::ROLLOUT:
			return attacker.defense_curl;
		case Move::PAYBACK:
			return defender.moved;
		case Move::SMELLINGSALT:
			return defender.pokemon().status.boosts_smellingsalt();
		case Move::SOLARBEAM:
			return !weather.rain();
		case Move::STOMP:
			return defender.minimize;
		case Move::SURF:
			return defender.vanish == DIVED;
		case Move::WAKE_UP_SLAP:
			return defender.pokemon().status.is_sleeping();
		case Move::WEATHER_BALL:
			return weather.hail() or weather.rain() or weather.sand() or weather.sun();
		default:
			return false;
	}
}

unsigned item_modifier (Pokemon const & attacker) {
	switch (attacker.item.name) {
		case Item::MUSCLE_BAND:
			if (attacker.move().is_physical())
				return 11;
			break;
		case Item::WISE_GLASSES:
			if (attacker.move().is_special())
				return 11;
			break;
		case Item::INSECT_PLATE:
		case Item::SILVERPOWDER:
			if (attacker.move().type() == Type::BUG)
				return 12;
			break;
		case Item::DREAD_PLATE:	
		case Item::BLACKGLASSES:
			if (attacker.move().type() == Type::DARK)
				return 12;
			break;
		case Item::DRACO_PLATE:
		case Item::DRAGON_FANG:
			if (attacker.move().type() == Type::DRAGON)
				return 12;
			break;
		case Item::ZAP_PLATE:
		case Item::MAGNET:
			if (attacker.move().type() == Type::ELECTRIC)
				return 12;
			break;
		case Item::FIST_PLATE:
		case Item::BLACK_BELT:
			if (attacker.move().type() == Type::FIGHTING)
				return 12;
			break;
		case Item::FLAME_PLATE:
		case Item::CHARCOAL:
			if (attacker.move().type() == Type::FIRE)
				return 12;
			break;
		case Item::SKY_PLATE:
		case Item::SHARP_BEAK:
			if (attacker.move().type() == Type::FLYING)
				return 12;
			break;
		case Item::SPOOKY_PLATE:
		case Item::SPELL_TAG:
			if (attacker.move().type() == Type::GHOST)
 				return 12;
			break;
		case Item::MEADOW_PLATE:
		case Item::MIRACLE_SEED:
			if (attacker.move().type() == Type::GRASS)
				return 12;
			break;
		case Item::EARTH_PLATE:
		case Item::SOFT_SAND:
			if (attacker.move().type() == Type::GROUND)
				return 12;
			break;
		case Item::ICICLE_PLATE:
		case Item::NEVERMELTICE:
			if (attacker.move().type() == Type::ICE)
				return 12;
			break;
		case Item::SILK_SCARF:
			if (attacker.move().type() == Type::NORMAL)
				return 12;
			break;
		case Item::TOXIC_PLATE:
		case Item::POISON_BARB:
			if (attacker.move().type() == Type::POISON)
				return 12;
			break;
		case Item::MIND_PLATE:
		case Item::TWISTEDSPOON:
		case Item::ODD_INCENSE:
			if (attacker.move().type() == Type::PSYCHIC)
				return 12;
			break;
		case Item::STONE_PLATE:
		case Item::HARD_STONE:
		case Item::ROCK_INCENSE:
			if (attacker.move().type() == Type::ROCK)
				return 12;
			break;
		case Item::IRON_PLATE:
		case Item::METAL_COAT:
			if (attacker.move().type() == Type::STEEL)
				return 12;
			break;
		case Item::SPLASH_PLATE:
		case Item::MYSTIC_WATER:
		case Item::SEA_INCENSE:
		case Item::WAVE_INCENSE:
			if (attacker.move().type() == Type::WATER)
				return 12;
			break;
		case Item::ADAMANT_ORB:
			if (attacker.name == DIALGA and (attacker.move().type() == Type::DRAGON or attacker.move().type() == Type::STEEL))
				return 12;
			break;
		case Item::GRISEOUS_ORB:
			if (attacker.name == GIRATINA_O and (attacker.move().type() == Type::DRAGON or attacker.move().type() == Type::GHOST))
				return 12;
			break;
		case Item::LUSTROUS_ORB:
			if (attacker.name == PALKIA and (attacker.move().type() == Type::DRAGON or attacker.move().type() == Type::WATER))
				return 12;
			break;
		default:
			break;
	}
	return 10;
}

bool mud_or_water_sport (Team const & attacker, Team const & defender) {
	bool const mud_sport = defender.mud_sport and attacker.pokemon().move().type() == Type::ELECTRIC;
	bool const water_sport = defender.water_sport and attacker.pokemon().move().type() == Type::FIRE;
	return mud_sport or water_sport;
}

Rational attacker_ability_modifier (Pokemon const & attacker, Pokemon const & defender) {
	switch (attacker.ability.name) {
		case Ability::TECHNICIAN:
			return attacker.move().is_boosted_by_technician() ? Rational(3, 2) : Rational();
		case Ability::BLAZE:
			return pinch_ability_activates(attacker, Type::FIRE) ? Rational(3, 2) : Rational();
		case Ability::OVERGROW:
			return pinch_ability_activates(attacker, Type::GRASS) ? Rational(3, 2) : Rational();
		case Ability::SWARM:
			return pinch_ability_activates(attacker, Type::BUG) ? Rational(3, 2) : Rational();
		case Ability::TORRENT:
			return pinch_ability_activates(attacker, Type::WATER) ? Rational(3, 2) : Rational();
		case Ability::IRON_FIST:
			return attacker.move().is_boosted_by_iron_fist() ? Rational(6, 5) : Rational();
		case Ability::RECKLESS:
			return attacker.move().is_boosted_by_reckless() ? Rational(6, 5) : Rational();
		case Ability::RIVALRY:
			return Rational(static_cast<unsigned>(4 + attacker.gender.multiplier(defender.gender)), 4);
		default:
			return Rational();
	}
}

bool pinch_ability_activates (Pokemon const & attacker, Type::Types const type) {
	return attacker.move().type() == type and attacker.hp.stat <= attacker.hp.max / 3;
}

Rational defender_ability_modifier (Move const & move, Ability const ability) {
	switch (ability.name) {
		case Ability::DRY_SKIN:
			return (move.type() == Type::FIRE) ? Rational(5, 4) : Rational();
		case Ability::HEATPROOF:
			return (move.type() == Type::FIRE) ? Rational(1, 2) : 	Rational();
		case Ability::THICK_FAT:
			return (move.type() == Type::FIRE or move.type() == Type::ICE) ? Rational(1, 2) : Rational();
		default:
			return Rational();
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
