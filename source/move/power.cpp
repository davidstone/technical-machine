// Move power calculator
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

#include "power.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>

#include "base_power.hpp"
#include "move.hpp"
#include "moves.hpp"

#include "../ability.hpp"
#include "../gender.hpp"
#include "../item.hpp"
#include "../rational.hpp"
#include "../status.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../pokemon/active_pokemon.hpp"
#include "../pokemon/pokemon.hpp"

namespace technicalmachine {
namespace {

unsigned calculate_base_power(ActivePokemon const & attacker, ActivePokemon const & defender, Variable const & variable);
unsigned second_lowest_bit (Stat const & stat);
bool doubling (ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather);
unsigned item_modifier (Pokemon const & attacker);
Rational defender_ability_modifier(Pokemon const & attacker, Ability const ability);
}	// namespace

unsigned move_power (ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather, Variable const & variable) {
	unsigned const base_power = calculate_base_power(attacker, defender, variable);
	unsigned power = base_power;

	if (doubling (attacker, defender, weather))
		power *= 2;

	power *= Rational(item_modifier(attacker), 10);

	if (attacker.charge_boosted())
		power *= 2;

	if (defender.sport_is_active(attacker.move()))
		power /= 2;

	power *= Ability::attacker_modifier(attacker, defender, base_power);
	
	power *= defender_ability_modifier(attacker, defender.ability());
	
	return std::max(power, 1u);
}

namespace {

Rational defender_ability_modifier(Pokemon const & attacker, Ability const ability) {
	Move const & move = attacker.move();
	switch (ability.name()) {
		case Ability::Dry_Skin:
			return (Type(move, attacker) == Type::Fire) ? Rational(5, 4) : Rational(1);
		case Ability::Heatproof:
			return (Type(move, attacker) == Type::Fire) ? Rational(1, 2) : Rational(1);
		case Ability::Thick_Fat: {
			Type const type(move, attacker);
			return (type == Type::Fire or type == Type::Ice) ?
				Rational(1, 2) :
				Rational(1);
		}
		default:
			return Rational(1);
	}
}

unsigned return_power(Pokemon const & pokemon) {
	return pokemon.happiness() * 2u / 5;
}

unsigned calculate_base_power(ActivePokemon const & attacker, ActivePokemon const & defender, Variable const & variable) {
	switch (attacker.move().name()) {
		case Moves::Crush_Grip:
		case Moves::Wring_Out:
			return 120u * defender.hp_ratio() + 1;
		case Moves::Eruption:
		case Moves::Water_Spout:
			return 150u * attacker.hp_ratio();
		case Moves::Flail:
		case Moves::Reversal: {
			unsigned const k = 64u * attacker.hp_ratio();
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
		case Moves::Fling:
			return attacker.item().get_fling_power();
		case Moves::Frustration:
			return 102 - return_power(attacker);
		case Moves::Fury_Cutter:
			return attacker.fury_cutter_power();
		case Moves::Grass_Knot:
		case Moves::Low_Kick:
			return defender.power_of_mass_based_moves();
		case Moves::Gyro_Ball: {
			unsigned const uncapped_power = 25u * defender.stat(Stat::SPE).stat / attacker.stat(Stat::SPE).stat + 1;
			return std::min(uncapped_power, 150u);
		}
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return attacker.momentum_move_power();
		case Moves::Hidden_Power: {
			static constexpr std::array<std::pair<Stat::Stats, unsigned>, 6> stat_and_position {{
				{ Stat::HP, 0 },
				{ Stat::ATK, 1 },
				{ Stat::DEF, 2 },
				{ Stat::SPE, 3 },
				{ Stat::SPA, 4 },
				{ Stat::SPD, 5 }
			}};
			auto const sum = [&](unsigned value, std::pair<Stat::Stats, unsigned> const & stat) {
				return value + second_lowest_bit(attacker.stat(stat.first)) * (1u << stat.second);
			};
			return std::accumulate(std::begin(stat_and_position), std::end(stat_and_position), 0u, sum) * 40 / 63 + 30;
		}
		case Moves::Magnitude:
			return variable.value();
		case Moves::Natural_Gift:
			return attacker.item().get_berry_power ();
		case Moves::Present:
			assert (!variable.present_heals());
			return variable.value();
		case Moves::Punishment: {
			unsigned const uncapped_power = 60 + 20 * defender.positive_stat_boosts();
			return std::min(uncapped_power, 200u);
		}
		case Moves::Return:
			return return_power(attacker);
		case Moves::Spit_Up:
			return attacker.spit_up_power();
		case Moves::Triple_Kick:
			return attacker.triple_kick_power();
		case Moves::Trump_Card:
			return attacker.move().pp.trump_card_power();
		default:
			return BasePower(attacker.move())();
	}
}

unsigned second_lowest_bit (Stat const & stat) {
	return static_cast<unsigned>(stat.iv >> 1) % 2;
}

bool doubling (ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather) {
	// I account for the doubling of the base power for Pursuit in the
	// switching function by simply multiplying the final base power by 2.
	// Regardless of the combination of modifiers, this does not change the
	// final base power. The exception is if the attacker's ally uses Helping
	// Hand, the target uses U-turn, and the attacker uses Pursuit with a
	// slower Pokemon that has Rivalry and a Muscle Band and neither the
	// attacker nor target is genderless. This will cause the base power to be
	// 1 less than it should be.

	auto const move = attacker.move().name();
	if (defender.vanish_doubles_power(move))
		return true;
	switch (move) {
		case Moves::Assurance:
			return defender.damaged();
		case Moves::Avalanche: 
		case Moves::Revenge:
			return attacker.damaged();
		case Moves::Brine:
			return defender.stat(Stat::HP).stat <= defender.stat(Stat::HP).max / 2;
		case Moves::Facade:
			return attacker.status().boosts_facade();
		case Moves::Ice_Ball:
		case Moves::Rollout:
			return attacker.defense_curled();
		case Moves::Payback:
			return defender.moved();
		case Moves::SmellingSalt:
			return defender.status().boosts_smellingsalt();
		case Moves::SolarBeam:
			return !weather.rain();
		case Moves::Stomp:
			return defender.minimized();
		case Moves::Wake_Up_Slap:
			return defender.status().is_sleeping();
		case Moves::Weather_Ball:
			return weather.hail() or weather.rain() or weather.sand() or weather.sun();
		default:
			return false;
	}
}

unsigned item_modifier (Pokemon const & attacker) {
	Type const type(attacker.move(), attacker);
	switch (attacker.item().name) {
		case Item::MUSCLE_BAND:
			if (is_physical(attacker.move()))
				return 11;
			break;
		case Item::WISE_GLASSES:
			if (is_special(attacker.move()))
				return 11;
			break;
		case Item::INSECT_PLATE:
		case Item::SILVERPOWDER:
			if (type == Type::Bug)
				return 12;
			break;
		case Item::DREAD_PLATE:	
		case Item::BLACKGLASSES:
			if (type == Type::Dark)
				return 12;
			break;
		case Item::DRACO_PLATE:
		case Item::DRAGON_FANG:
			if (type == Type::Dragon)
				return 12;
			break;
		case Item::ZAP_PLATE:
		case Item::MAGNET:
			if (type == Type::Electric)
				return 12;
			break;
		case Item::FIST_PLATE:
		case Item::BLACK_BELT:
			if (type == Type::Fighting)
				return 12;
			break;
		case Item::FLAME_PLATE:
		case Item::CHARCOAL:
			if (type == Type::Fire)
				return 12;
			break;
		case Item::SKY_PLATE:
		case Item::SHARP_BEAK:
			if (type == Type::Flying)
				return 12;
			break;
		case Item::SPOOKY_PLATE:
		case Item::SPELL_TAG:
			if (type == Type::Ghost)
 				return 12;
			break;
		case Item::MEADOW_PLATE:
		case Item::MIRACLE_SEED:
			if (type == Type::Grass)
				return 12;
			break;
		case Item::EARTH_PLATE:
		case Item::SOFT_SAND:
			if (type == Type::Ground)
				return 12;
			break;
		case Item::ICICLE_PLATE:
		case Item::NEVERMELTICE:
			if (type == Type::Ice)
				return 12;
			break;
		case Item::SILK_SCARF:
			if (type == Type::Normal)
				return 12;
			break;
		case Item::TOXIC_PLATE:
		case Item::POISON_BARB:
			if (type == Type::Poison)
				return 12;
			break;
		case Item::MIND_PLATE:
		case Item::TWISTEDSPOON:
		case Item::ODD_INCENSE:
			if (type == Type::Psychic)
				return 12;
			break;
		case Item::STONE_PLATE:
		case Item::HARD_STONE:
		case Item::ROCK_INCENSE:
			if (type == Type::Rock)
				return 12;
			break;
		case Item::IRON_PLATE:
		case Item::METAL_COAT:
			if (type == Type::Steel)
				return 12;
			break;
		case Item::SPLASH_PLATE:
		case Item::MYSTIC_WATER:
		case Item::SEA_INCENSE:
		case Item::WAVE_INCENSE:
			if (type == Type::Water)
				return 12;
			break;
		case Item::ADAMANT_ORB:
			if (is_boosted_by_adamant_orb(attacker) and (type == Type::Dragon or type == Type::Steel))
				return 12;
			break;
		case Item::GRISEOUS_ORB:
			if (is_boosted_by_griseous_orb(attacker) and (type == Type::Dragon or type == Type::Ghost))
				return 12;
			break;
		case Item::LUSTROUS_ORB:
			if (is_boosted_by_lustrous_orb(attacker) and (type == Type::Dragon or type == Type::Water))
				return 12;
			break;
		default:
			break;
	}
	return 10;
}

}	// unnamed namespace
}	// namespace technicalmachine
