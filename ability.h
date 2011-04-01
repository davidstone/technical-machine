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

#ifndef ABILITY_H_
#define ABILITY_H_

#include <map>
#include <string>

namespace technicalmachine {

enum abilities { ADAPTABILITY, AFTERMATH, AIR_LOCK, ANGER_POINT, ANTICIPATION, ARENA_TRAP, BAD_DREAMS, BATTLE_ARMOR, BLAZE, CHLOROPHYLL, CLEAR_BODY, COLOR_CHANGE, COMPOUNDEYES, CUTE_CHARM, DAMP, DOWNLOAD, DRIZZLE, DROUGHT, DRY_SKIN, EARLY_BIRD, EFFECT_SPORE, FLAME_BODY, FLASH_FIRE, FLOWER_GIFT, FORECAST, FOREWARN, FRISK, GLUTTONY, GUTS, HEATPROOF, HUSTLE, HYDRATION, HYPER_CUTTER, ICE_BODY, IMMUNITY, INNER_FOCUS, INSOMNIA, INTIMIDATE, IRON_FIST, KEEN_EYE, KLUTZ, LEAF_GUARD, LEVITATE, LIGHTNINGROD, LIMBER, LIQUID_OOZE, MAGIC_GUARD, MAGMA_ARMOR, MAGNET_PULL, MARVEL_SCALE, MINUS, MOLD_BREAKER, MOTOR_DRIVE, MULTITYPE, NATURAL_CURE, NO_GUARD, NORMALIZE, OBLIVIOUS, OVERGROW, OWN_TEMPO, PLUS, POISON_HEAL, POISON_POINT, PRESSURE, PURE_POWER, QUICK_FEET, RAIN_DISH, RECKLESS, RIVALRY, ROCK_HEAD, ROUGH_SKIN, SAND_STREAM, SAND_VEIL, SCRAPPY, SERENE_GRACE, SHADOW_TAG, SHED_SKIN, SHIELD_DUST, SIMPLE, SKILL_LINK, SLOW_START, SNIPER, SNOW_CLOAK, SNOW_WARNING, SOLAR_POWER, SOLID_ROCK, SOUNDPROOF, SPEED_BOOST, STALL, STATIC, STEADFAST, STENCH, STICKY_HOLD, STORM_DRAIN, STURDY, SUCTION_CUPS, SUPER_LUCK, SWARM, SWIFT_SWIM, SYNCHRONIZE, TANGLED_FEET, TECHNICIAN, THICK_FAT, TINTED_LENS, TORRENT, TRACE, TRUANT, UNAWARE, UNBURDEN, VOLT_ABSORB, WATER_ABSORB, WATER_VEIL, WONDER_GUARD, END_ABILITY };


/*
Stench replaces Honey Gather, Illuminate, Pickup, and Run Away.

Air Lock replaces Cloud Nine.

Pure Power replaces Huge Power.

Battle Armor replaces Shell Armor.

Insomnia replaces Vital Spirit.

Clear Body replaces White Smoke.

Solid Rock replaces Filter.
*/

const std::string ability_name [] = { "Adaptability", "Aftermath", "Air Lock", "Anger Point", "Anticipation", "Arena Trap", "Bad Dreams", "Battle Armor", "Blaze", "Chlorophyll", "Clear Body", "Color Change", "Compoundeyes", "Cute Charm", "Damp", "Download", "Drizzle", "Drought", "Dry Skin", "Early Bird", "Effect Spore", "Flame Body", "Flash Fire", "Flower Gift", "Forecast", "Forewarn", "Frisk", "Gluttony", "Guts", "Heatproof", "Hustle", "Hydration", "Hyper Cutter", "Ice Body", "Immunity", "Inner Focus", "Insomnia", "Intimidate", "Iron Fist", "Keen Eye", "Klutz", "Leaf Guard", "Levitate", "Lightningrod", "Limber", "Liquid Ooze", "Magic Guard", "Magma Armor", "Magnet Pull", "Marvel Scale", "Minus", "Mold Breaker", "Motor Drive", "Multitype", "Natural Cure", "No Guard", "Normalize", "Oblivious", "Overgrow", "Own Tempo", "Plus", "Poison Heal", "Poison Point", "Pressure", "Pure Power", "Quick Feet", "Rain Dish", "Reckless", "Rivalry", "Rock Head", "Rough Skin", "Sand Stream", "Sand Veil", "Scrappy", "Serene Grace", "Shadow Tag", "Shed Skin", "Shield Dust", "Simple", "Skill Link", "Slow Start", "Sniper", "Snow Cloak", "Snow Warning", "Solar Power", "Solid Rock", "Soundproof", "Speed Boost", "Stall", "Static", "Steadfast", "Stench", "Sticky Hold", "Storm Drain", "Sturdy", "Suction Cups", "Super Luck", "Swarm", "Swift Swim", "Synchronize", "Tangled Feet", "Technician", "Thick Fat", "Tinted Lens", "Torrent", "Trace", "Truant", "Unaware", "Unburden", "Volt Absorb", "Water Absorb", "Water Veil", "Wonder Guard" };

void set_ability_map (std::map <std::string, abilities> &abilities_map);

}
#endif
