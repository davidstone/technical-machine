// Ability data structure
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

#ifndef ABILITY_H_
#define ABILITY_H_

#include <string>

namespace technicalmachine {

class Team;
class Weather;

class Ability {
	public:
		enum Abilities {
			ADAPTABILITY, AFTERMATH, AIR_LOCK, ANGER_POINT, ANTICIPATION,
			ARENA_TRAP, BAD_DREAMS, BATTLE_ARMOR, BLAZE, CHLOROPHYLL,
			CLEAR_BODY, CLOUD_NINE, COLOR_CHANGE, COMPOUNDEYES, CUTE_CHARM,
			DAMP, DOWNLOAD, DRIZZLE, DROUGHT, DRY_SKIN,
			EARLY_BIRD, EFFECT_SPORE, FILTER, FLAME_BODY, FLASH_FIRE,
			FLOWER_GIFT, FORECAST, FOREWARN, FRISK, GLUTTONY,
			GUTS, HEATPROOF, HONEY_GATHER, HUGE_POWER, HUSTLE,
			HYDRATION, HYPER_CUTTER, ICE_BODY, ILLUMINATE, IMMUNITY,
			INNER_FOCUS, INSOMNIA, INTIMIDATE, IRON_FIST, KEEN_EYE,
			KLUTZ, LEAF_GUARD, LEVITATE, LIGHTNINGROD, LIMBER,
			LIQUID_OOZE, MAGIC_GUARD, MAGMA_ARMOR, MAGNET_PULL, MARVEL_SCALE,
			MINUS, MOLD_BREAKER, MOTOR_DRIVE, MULTITYPE, NATURAL_CURE,
			NO_GUARD, NORMALIZE, OBLIVIOUS, OVERGROW, OWN_TEMPO,
			PICKUP, PLUS, POISON_HEAL, POISON_POINT, PRESSURE,
			PURE_POWER, QUICK_FEET, RAIN_DISH, RECKLESS, RIVALRY,
			ROCK_HEAD, ROUGH_SKIN, RUN_AWAY, SAND_STREAM, SAND_VEIL,
			SCRAPPY, SERENE_GRACE, SHADOW_TAG, SHED_SKIN, SHELL_ARMOR,
			SHIELD_DUST, SIMPLE, SKILL_LINK, SLOW_START, SNIPER,
			SNOW_CLOAK, SNOW_WARNING, SOLAR_POWER, SOLID_ROCK, SOUNDPROOF,
			SPEED_BOOST, STALL, STATIC, STEADFAST, STENCH,
			STICKY_HOLD, STORM_DRAIN, STURDY, SUCTION_CUPS, SUPER_LUCK,
			SWARM, SWIFT_SWIM, SYNCHRONIZE, TANGLED_FEET, TECHNICIAN,
			THICK_FAT, TINTED_LENS, TORRENT, TRACE, TRUANT,
			UNAWARE, UNBURDEN, VITAL_SPIRIT, VOLT_ABSORB, WATER_ABSORB,
			WATER_VEIL, WHITE_SMOKE, WONDER_GUARD, END_ABILITY
		};
		Abilities name;

		Ability ();
		explicit Ability (Abilities ability);
		bool is_set () const;
		bool blocks_switching (Team const & switcher, Weather const & weather) const;
		bool blocks_weather () const;
		bool blocks_burn (Weather const & weather) const;
		bool blocks_freeze () const;
		bool blocks_paralysis (Weather const & weather) const;
		bool blocks_poison (Weather const & weather) const;
		bool blocks_sleep (Weather const & weather) const;
		bool weakens_SE_attacks () const;
		static std::string to_string (Abilities name);
		std::string to_string () const;
		static Abilities from_string (std::string const & str);
		void set_name_from_string (std::string const & str);
};
}
#endif		// ABILITY_H_
