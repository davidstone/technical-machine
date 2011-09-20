// Ability data structure
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <map>
#include <string>
#include "ability.h"
#include "pokemon.h"
#include "team.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

Ability::Ability ():
	name (END_ABILITY) {
}

Ability::Ability (Abilities ability):
	name (ability) {
}

bool Ability::blocks_switching (Team const & switcher, Weather const weather) const {
	switch (name) {
		case SHADOW_TAG:
			if (switcher.pokemon->ability.name != Ability::SHADOW_TAG)
				return true;
			return false;
		case ARENA_TRAP:
			if (grounded (switcher, weather))
				return true;
			return false;
		case MAGNET_PULL:
			if (is_type (switcher, STEEL))
				return true;
			return false;
		default:
			return false;
	}
}

bool Ability::is_set () const {
	return name != END_ABILITY;
}

std::string Ability::get_name () const {
	/*
	Stench replaces Honey Gather, Illuminate, Pickup, and Run Away.

	Air Lock replaces Cloud Nine.

	Pure Power replaces Huge Power.

	Battle Armor replaces Shell Armor.

	Insomnia replaces Vital Spirit.

	Clear Body replaces White Smoke.

	Solid Rock replaces Filter.
	*/

	static std::string const ability_name [] = { "Adaptability", "Aftermath", "Air Lock", "Anger Point", "Anticipation", "Arena Trap", "Bad Dreams", "Battle Armor", "Blaze", "Chlorophyll", "Clear Body", "Color Change", "Compoundeyes", "Cute Charm", "Damp", "Download", "Drizzle", "Drought", "Dry Skin", "Early Bird", "Effect Spore", "Flame Body", "Flash Fire", "Flower Gift", "Forecast", "Forewarn", "Frisk", "Gluttony", "Guts", "Heatproof", "Hustle", "Hydration", "Hyper Cutter", "Ice Body", "Immunity", "Inner Focus", "Insomnia", "Intimidate", "Iron Fist", "Keen Eye", "Klutz", "Leaf Guard", "Levitate", "Lightningrod", "Limber", "Liquid Ooze", "Magic Guard", "Magma Armor", "Magnet Pull", "Marvel Scale", "Minus", "Mold Breaker", "Motor Drive", "Multitype", "Natural Cure", "No Guard", "Normalize", "Oblivious", "Overgrow", "Own Tempo", "Plus", "Poison Heal", "Poison Point", "Pressure", "Pure Power", "Quick Feet", "Rain Dish", "Reckless", "Rivalry", "Rock Head", "Rough Skin", "Sand Stream", "Sand Veil", "Scrappy", "Serene Grace", "Shadow Tag", "Shed Skin", "Shield Dust", "Simple", "Skill Link", "Slow Start", "Sniper", "Snow Cloak", "Snow Warning", "Solar Power", "Solid Rock", "Soundproof", "Speed Boost", "Stall", "Static", "Steadfast", "Stench", "Sticky Hold", "Storm Drain", "Sturdy", "Suction Cups", "Super Luck", "Swarm", "Swift Swim", "Synchronize", "Tangled Feet", "Technician", "Thick Fat", "Tinted Lens", "Torrent", "Trace", "Truant", "Unaware", "Unburden", "Volt Absorb", "Water Absorb", "Water Veil", "Wonder Guard", "END_ABILITY" };
	return ability_name [name];
}

class Map {
	public:
		std::map <std::string, Ability::Abilities> ability;
		Map () {
			ability ["Adaptability"] = Ability::ADAPTABILITY;
			ability ["Aftermath"] = Ability::AFTERMATH;
			ability ["Air Lock"] = Ability::AIR_LOCK;
			ability ["Anger Point"] = Ability::ANGER_POINT;
			ability ["Anticipation"] = Ability::ANTICIPATION;
			ability ["Arena Trap"] = Ability::ARENA_TRAP;
			ability ["Bad Dreams"] = Ability::BAD_DREAMS;
			ability ["Battle Armor"] = Ability::BATTLE_ARMOR;
			ability ["Blaze"] = Ability::BLAZE;
			ability ["Chlorophyll"] = Ability::CHLOROPHYLL;
			ability ["Clear Body"] = Ability::CLEAR_BODY;
			ability ["Cloud Nine"] = Ability::AIR_LOCK;
			ability ["Color Change"] = Ability::COLOR_CHANGE;
			ability ["Compoundeyes"] = Ability::COMPOUNDEYES;
			ability ["Cute Charm"] = Ability::CUTE_CHARM;
			ability ["Damp"] = Ability::DAMP;
			ability ["Download"] = Ability::DOWNLOAD;
			ability ["Drizzle"] = Ability::DRIZZLE;
			ability ["Drought"] = Ability::DROUGHT;
			ability ["Dry Skin"] = Ability::DRY_SKIN;
			ability ["Early Bird"] = Ability::EARLY_BIRD;
			ability ["Effect Spore"] = Ability::EFFECT_SPORE;
			ability ["Filter"] = Ability::SOLID_ROCK;
			ability ["Flame Body"] = Ability::FLAME_BODY;
			ability ["Flash Fire"] = Ability::FLASH_FIRE;
			ability ["Flower Gift"] = Ability::FLOWER_GIFT;
			ability ["Forecast"] = Ability::FORECAST;
			ability ["Forewarn"] = Ability::FOREWARN;
			ability ["Frisk"] = Ability::FRISK;
			ability ["Gluttony"] = Ability::GLUTTONY;
			ability ["Guts"] = Ability::GUTS;
			ability ["Heatproof"] = Ability::HEATPROOF;
			ability ["Honey Gather"] = Ability::STENCH;
			ability ["Huge Power"] = Ability::PURE_POWER;
			ability ["Hustle"] = Ability::HUSTLE;
			ability ["Hydration"] = Ability::HYDRATION;
			ability ["Hyper Cutter"] = Ability::HYPER_CUTTER;
			ability ["Ice Body"] = Ability::ICE_BODY;
			ability ["Illuminate"] = Ability::STENCH;
			ability ["Immunity"] = Ability::IMMUNITY;
			ability ["Inner Focus"] = Ability::INNER_FOCUS;
			ability ["Insomnia"] = Ability::INSOMNIA;
			ability ["Intimidate"] = Ability::INTIMIDATE;
			ability ["Iron Fist"] = Ability::IRON_FIST;
			ability ["Keen Eye"] = Ability::KEEN_EYE;
			ability ["Klutz"] = Ability::KLUTZ;
			ability ["Leaf Guard"] = Ability::LEAF_GUARD;
			ability ["Levitate"] = Ability::LEVITATE;
			ability ["Lightningrod"] = Ability::LIGHTNINGROD;
			ability ["Limber"] = Ability::LIMBER;
			ability ["Liquid Ooze"] = Ability::LIQUID_OOZE;
			ability ["Magic Guard"] = Ability::MAGIC_GUARD;
			ability ["Magma Armor"] = Ability::MAGMA_ARMOR;
			ability ["Magnet Pull"] = Ability::MAGNET_PULL;
			ability ["Marvel Scale"] = Ability::MARVEL_SCALE;
			ability ["Minus"] = Ability::MINUS;
			ability ["Mold Breaker"] = Ability::MOLD_BREAKER;
			ability ["Motor Drive"] = Ability::MOTOR_DRIVE;
			ability ["Multitype"] = Ability::MULTITYPE;
			ability ["Natural Cure"] = Ability::NATURAL_CURE;
			ability ["No Guard"] = Ability::NO_GUARD;
			ability ["Normalize"] = Ability::NORMALIZE;
			ability ["Oblivious"] = Ability::OBLIVIOUS;
			ability ["Overgrow"] = Ability::OVERGROW;
			ability ["Own Tempo"] = Ability::OWN_TEMPO;
			ability ["Pickup"] = Ability::STENCH;
			ability ["Plus"] = Ability::PLUS;
			ability ["Poison Heal"] = Ability::POISON_HEAL;
			ability ["Poison Point"] = Ability::POISON_POINT;
			ability ["Pressure"] = Ability::PRESSURE;
			ability ["Pure Power"] = Ability::PURE_POWER;
			ability ["Quick Feet"] = Ability::QUICK_FEET;
			ability ["Rain Dish"] = Ability::RAIN_DISH;
			ability ["Reckless"] = Ability::RECKLESS;
			ability ["Rivalry"] = Ability::RIVALRY;
			ability ["Rock Head"] = Ability::ROCK_HEAD;
			ability ["Rough Skin"] = Ability::ROUGH_SKIN;
			ability ["Run Away"] = Ability::STENCH;
			ability ["Sand Stream"] = Ability::SAND_STREAM;
			ability ["Sand Veil"] = Ability::SAND_VEIL;
			ability ["Scrappy"] = Ability::SCRAPPY;
			ability ["Serene Grace"] = Ability::SERENE_GRACE;
			ability ["Shadow Tag"] = Ability::SHADOW_TAG;
			ability ["Shed Skin"] = Ability::SHED_SKIN;
			ability ["Shell Armor"] = Ability::BATTLE_ARMOR;
			ability ["Shield Dust"] = Ability::SHIELD_DUST;
			ability ["Simple"] = Ability::SIMPLE;
			ability ["Skill Link"] = Ability::SKILL_LINK;
			ability ["Slow Start"] = Ability::SLOW_START;
			ability ["Sniper"] = Ability::SNIPER;
			ability ["Snow Cloak"] = Ability::SNOW_CLOAK;
			ability ["Snow Warning"] = Ability::SNOW_WARNING;
			ability ["Solar Power"] = Ability::SOLAR_POWER;
			ability ["Solid Rock"] = Ability::SOLID_ROCK;
			ability ["Soundproof"] = Ability::SOUNDPROOF;
			ability ["Speed Boost"] = Ability::SPEED_BOOST;
			ability ["Stall"] = Ability::STALL;
			ability ["Static"] = Ability::STATIC;
			ability ["Steadfast"] = Ability::STEADFAST;
			ability ["Stench"] = Ability::STENCH;
			ability ["Sticky Hold"] = Ability::STICKY_HOLD;
			ability ["Storm Drain"] = Ability::STORM_DRAIN;
			ability ["Sturdy"] = Ability::STURDY;
			ability ["Suction Cups"] = Ability::SUCTION_CUPS;
			ability ["Super Luck"] = Ability::SUPER_LUCK;
			ability ["Swarm"] = Ability::SWARM;
			ability ["Swift Swim"] = Ability::SWIFT_SWIM;
			ability ["Synchronize"] = Ability::SYNCHRONIZE;
			ability ["Tangled Feet"] = Ability::TANGLED_FEET;
			ability ["Technician"] = Ability::TECHNICIAN;
			ability ["Thick Fat"] = Ability::THICK_FAT;
			ability ["Tinted Lens"] = Ability::TINTED_LENS;
			ability ["Torrent"] = Ability::TORRENT;
			ability ["Trace"] = Ability::TRACE;
			ability ["Truant"] = Ability::TRUANT;
			ability ["Unaware"] = Ability::UNAWARE;
			ability ["Unburden"] = Ability::UNBURDEN;
			ability ["Vital Spirit"] = Ability::INSOMNIA;
			ability ["Volt Absorb"] = Ability::VOLT_ABSORB;
			ability ["Water Absorb"] = Ability::WATER_ABSORB;
			ability ["Water Veil"] = Ability::WATER_VEIL;
			ability ["White Smoke"] = Ability::CLEAR_BODY;
			ability ["Wonder Guard"] = Ability::WONDER_GUARD;
		}
		Ability::Abilities find (std::string const & str) const {
			return ability.find (str)->second;
		}
};

Ability::Abilities Ability::name_from_string (std::string const & str) {
	static Map const map;
	return map.find (str);
}

void Ability::set_name_from_string (std::string const & str) {
	name = name_from_string (str);
}
}
