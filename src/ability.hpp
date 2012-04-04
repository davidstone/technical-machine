// Ability data structure
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

#ifndef ABILITY_HPP_
#define ABILITY_HPP_

#include <string>
#include "status.hpp"

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
			WATER_VEIL, WHITE_SMOKE, WONDER_GUARD, END
		};
		Abilities name;
		Ability ();
		explicit Ability (Abilities ability);
		explicit Ability (std::string const & str);
		bool is_set () const;
		void set_if_unknown (Abilities ability);
		bool blocks_switching (Team const & switcher, Weather const & weather) const;
		bool blocks_weather () const;

		template<Status::Statuses status>
		bool blocks_status (Weather const & weather) const;
		bool blocks_confusion () const;
		bool reflects_status () const;
		bool absorbs_poison_damage () const;
		bool blocks_burn_damage_penalty () const;
		bool blocks_paralysis_speed_penalty () const;
		bool can_clear_status (Status status) const;
		bool clears_status_on_switch () const;
		bool is_immune_to_ground () const;
		bool wakes_up_early () const;
		bool weakens_burn () const;
		
		bool harms_sleepers () const;

		bool blocks_phazing () const;
		bool blocks_recoil () const;
		bool blocks_secondary_damage () const;
		bool blocks_sound_moves () const;
		bool cannot_miss () const;
		bool damages_leechers () const;
		bool weakens_se_attacks () const;
		bool strengthens_nve_attacks () const;
		
		bool ignores_blockers () const;
		
		bool boosts_critical_hits () const;
		bool boosts_defense (Status status) const;
		bool boosts_special_attack (Weather const & weather) const;
		bool boosts_special_defense (Weather const & weather) const;
		bool boosts_speed () const;
		bool boosts_speed_when_flinched () const;
		bool boosts_stab () const;
		
		bool is_loafing (bool loaf) const;
		static std::string to_string (Abilities name);
		std::string to_string () const;
		static Abilities from_string (std::string const & str);
		
		static void activate_on_switch (Team & switcher, Team & other, Weather & weather);
};
}
#endif	// ABILITY_HPP_
