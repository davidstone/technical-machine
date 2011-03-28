// Ability map
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

void set_ability_map (std::map <std::string, abilities> &abilities_map) {
	abilities_map["Adaptability"] = ADAPTABILITY;
	abilities_map["Aftermath"] = AFTERMATH;
	abilities_map["Air Lock"] = AIR_LOCK;
	abilities_map["Anger Point"] = ANGER_POINT;
	abilities_map["Anticipation"] = ANTICIPATION;
	abilities_map["Arena Trap"] = ARENA_TRAP;
	abilities_map["Bad Dreams"] = BAD_DREAMS;
	abilities_map["Battle Armor"] = BATTLE_ARMOR;
	abilities_map["Blaze"] = BLAZE;
	abilities_map["Chlorophyll"] = CHLOROPHYLL;
	abilities_map["Clear Body"] = CLEAR_BODY;
	abilities_map["Cloud Nine"] = AIR_LOCK;
	abilities_map["Color Change"] = COLOR_CHANGE;
	abilities_map["Compoundeyes"] = COMPOUNDEYES;
	abilities_map["Cute Charm"] = CUTE_CHARM;
	abilities_map["Damp"] = DAMP;
	abilities_map["Download"] = DOWNLOAD;
	abilities_map["Drizzle"] = DRIZZLE;
	abilities_map["Drought"] = DROUGHT;
	abilities_map["Dry Skin"] = DRY_SKIN;
	abilities_map["Early Bird"] = EARLY_BIRD;
	abilities_map["Effect Spore"] = EFFECT_SPORE;
	abilities_map["Filter"] = SOLID_ROCK;
	abilities_map["Flame Body"] = FLAME_BODY;
	abilities_map["Flash Fire"] = FLASH_FIRE;
	abilities_map["Flower Gift"] = FLOWER_GIFT;
	abilities_map["Forecast"] = FORECAST;
	abilities_map["Forewarn"] = FOREWARN;
	abilities_map["Frisk"] = FRISK;
	abilities_map["Gluttony"] = GLUTTONY;
	abilities_map["Guts"] = GUTS;
	abilities_map["Heatproof"] = HEATPROOF;
	abilities_map["Honey Gather"] = STENCH;
	abilities_map["Huge Power"] = PURE_POWER;
	abilities_map["Hustle"] = HUSTLE;
	abilities_map["Hydration"] = HYDRATION;
	abilities_map["Hyper Cutter"] = HYPER_CUTTER;
	abilities_map["Ice Body"] = ICE_BODY;
	abilities_map["Illuminate"] = STENCH;
	abilities_map["Immunity"] = IMMUNITY;
	abilities_map["Inner Focus"] = INNER_FOCUS;
	abilities_map["Insomnia"] = INSOMNIA;
	abilities_map["Intimidate"] = INTIMIDATE;
	abilities_map["Iron Fist"] = IRON_FIST;
	abilities_map["Keen Eye"] = KEEN_EYE;
	abilities_map["Klutz"] = KLUTZ;
	abilities_map["Leaf Guard"] = LEAF_GUARD;
	abilities_map["Levitate"] = LEVITATE;
	abilities_map["Lightningrod"] = LIGHTNINGROD;
	abilities_map["Limber"] = LIMBER;
	abilities_map["Liquid Ooze"] = LIQUID_OOZE;
	abilities_map["Magic Guard"] = MAGIC_GUARD;
	abilities_map["Magma Armor"] = MAGMA_ARMOR;
	abilities_map["Magnet Pull"] = MAGNET_PULL;
	abilities_map["Marvel Scale"] = MARVEL_SCALE;
	abilities_map["Minus"] = MINUS;
	abilities_map["Mold Breaker"] = MOLD_BREAKER;
	abilities_map["Motor Drive"] = MOTOR_DRIVE;
	abilities_map["Multitype"] = MULTITYPE;
	abilities_map["Natural Cure"] = NATURAL_CURE;
	abilities_map["No Guard"] = NO_GUARD;
	abilities_map["Normalize"] = NORMALIZE;
	abilities_map["Oblivious"] = OBLIVIOUS;
	abilities_map["Overgrow"] = OVERGROW;
	abilities_map["Own Tempo"] = OWN_TEMPO;
	abilities_map["Pickup"] = STENCH;
	abilities_map["Plus"] = PLUS;
	abilities_map["Poison Heal"] = POISON_HEAL;
	abilities_map["Poison Point"] = POISON_POINT;
	abilities_map["Pressure"] = PRESSURE;
	abilities_map["Pure Power"] = PURE_POWER;
	abilities_map["Quick Feet"] = QUICK_FEET;
	abilities_map["Rain Dish"] = RAIN_DISH;
	abilities_map["Reckless"] = RECKLESS;
	abilities_map["Rivalry"] = RIVALRY;
	abilities_map["Rock Head"] = ROCK_HEAD;
	abilities_map["Rough Skin"] = ROUGH_SKIN;
	abilities_map["Run Away"] = STENCH;
	abilities_map["Sand Stream"] = SAND_STREAM;
	abilities_map["Sand Veil"] = SAND_VEIL;
	abilities_map["Scrappy"] = SCRAPPY;
	abilities_map["Serene Grace"] = SERENE_GRACE;
	abilities_map["Shadow Tag"] = SHADOW_TAG;
	abilities_map["Shed Skin"] = SHED_SKIN;
	abilities_map["Shell Armor"] = BATTLE_ARMOR;
	abilities_map["Shield Dust"] = SHIELD_DUST;
	abilities_map["Simple"] = SIMPLE;
	abilities_map["Skill Link"] = SKILL_LINK;
	abilities_map["Slow Start"] = SLOW_START;
	abilities_map["Sniper"] = SNIPER;
	abilities_map["Snow Cloak"] = SNOW_CLOAK;
	abilities_map["Snow Warning"] = SNOW_WARNING;
	abilities_map["Solar Power"] = SOLAR_POWER;
	abilities_map["Solid Rock"] = SOLID_ROCK;
	abilities_map["Soundproof"] = SOUNDPROOF;
	abilities_map["Speed Boost"] = SPEED_BOOST;
	abilities_map["Stall"] = STALL;
	abilities_map["Static"] = STATIC;
	abilities_map["Steadfast"] = STEADFAST;
	abilities_map["Stench"] = STENCH;
	abilities_map["Sticky Hold"] = STICKY_HOLD;
	abilities_map["Storm Drain"] = STORM_DRAIN;
	abilities_map["Sturdy"] = STURDY;
	abilities_map["Suction Cups"] = SUCTION_CUPS;
	abilities_map["Super Luck"] = SUPER_LUCK;
	abilities_map["Swarm"] = SWARM;
	abilities_map["Swift Swim"] = SWIFT_SWIM;
	abilities_map["Synchronize"] = SYNCHRONIZE;
	abilities_map["Tangled Feet"] = TANGLED_FEET;
	abilities_map["Technician"] = TECHNICIAN;
	abilities_map["Thick Fat"] = THICK_FAT;
	abilities_map["Tinted Lens"] = TINTED_LENS;
	abilities_map["Torrent"] = TORRENT;
	abilities_map["Trace"] = TRACE;
	abilities_map["Truant"] = TRUANT;
	abilities_map["Unaware"] = UNAWARE;
	abilities_map["Unburden"] = UNBURDEN;
	abilities_map["Vital Spirit"] = INSOMNIA;
	abilities_map["Volt Absorb"] = VOLT_ABSORB;
	abilities_map["Water Absorb"] = WATER_ABSORB;
	abilities_map["Water Veil"] = WATER_VEIL;
	abilities_map["White Smoke"] = CLEAR_BODY;
	abilities_map["Wonder Guard"] = WONDER_GUARD;
}
