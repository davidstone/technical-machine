// Ability string conversions
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

#include "ability.hpp"

#include <map>
#include <string>

#include "invalid_string_conversion.hpp"

namespace technicalmachine {

std::string Ability::to_string (Abilities name) {
	static std::string const ability_name [] = {
		"Adaptability", "Aftermath", "Air Lock", "Anger Point",
		"Anticipation", "Arena Trap", "Bad Dreams", "Battle Armor",
		"Blaze", "Chlorophyll", "Clear Body", "Cloud Nine",
		"Color Change", "Compoundeyes", "Cute Charm", "Damp",
		"Download", "Drizzle", "Drought", "Dry Skin",
		"Early Bird", "Effect Spore", "Filter", "Flame Body",
		"Flash Fire", "Flower Gift", "Forecast", "Forewarn",
		"Frisk", "Gluttony", "Guts", "Heatproof",
		"Honey Gather", "Huge Power", "Hustle", "Hydration",
		"Hyper Cutter", "Ice Body", "Illuminate", "Immunity",
		"Inner Focus", "Insomnia", "Intimidate", "Iron Fist",
		"Keen Eye", "Klutz", "Leaf Guard", "Levitate",
		"Lightningrod", "Limber", "Liquid Ooze", "Magic Guard",
		"Magma Armor", "Magnet Pull", "Marvel Scale", "Minus",
		"Mold Breaker", "Motor Drive", "Multitype", "Natural Cure",
		"No Guard", "Normalize", "Oblivious", "Overgrow",
		"Own Tempo", "Pickup", "Plus", "Poison Heal",
		"Poison Point", "Pressure", "Pure Power", "Quick Feet",
		"Rain Dish", "Reckless", "Rivalry", "Rock Head",
		"Rough Skin", "Run Away", "Sand Stream", "Sand Veil",
		"Scrappy", "Serene Grace", "Shadow Tag", "Shed Skin",
		"Shell Armor", "Shield Dust", "Simple", "Skill Link",
		"Slow Start", "Sniper", "Snow Cloak", "Snow Warning",
		"Solar Power", "Solid Rock", "Soundproof", "Speed Boost",
		"Stall", "Static", "Steadfast", "Stench",
		"Sticky Hold", "Storm Drain", "Sturdy", "Suction Cups",
		"Super Luck", "Swarm", "Swift Swim", "Synchronize",
		"Tangled Feet", "Technician", "Thick Fat", "Tinted Lens",
		"Torrent", "Trace", "Truant", "Unaware",
		"Unburden", "Vital Spirit", "Volt Absorb", "Water Absorb",
		"Water Veil", "White Smoke", "Wonder Guard", "END_ABILITY"
	};
	return ability_name [name];
}

std::string Ability::to_string () const {
	return to_string (name);
}

Ability::Abilities Ability::from_string (std::string const & str) {
	static std::map <std::string, Abilities> const converter {
		{ "Adaptability", ADAPTABILITY },
		{ "Aftermath", AFTERMATH },
		{ "Air Lock", AIR_LOCK },
		{ "Anger Point", ANGER_POINT },
		{ "Anticipation", ANTICIPATION },
		{ "Arena Trap", ARENA_TRAP },
		{ "Bad Dreams", BAD_DREAMS },
		{ "Battle Armor", BATTLE_ARMOR },
		{ "Blaze", BLAZE },
		{ "Chlorophyll", CHLOROPHYLL },
		{ "Clear Body", CLEAR_BODY },
		{ "Cloud Nine", CLOUD_NINE },
		{ "Color Change", COLOR_CHANGE },
		{ "Compoundeyes", COMPOUNDEYES },
		{ "Cute Charm", CUTE_CHARM },
		{ "Damp", DAMP },
		{ "Download", DOWNLOAD },
		{ "Drizzle", DRIZZLE },
		{ "Drought", DROUGHT },
		{ "Dry Skin", DRY_SKIN },
		{ "Early Bird", EARLY_BIRD },
		{ "Effect Spore", EFFECT_SPORE },
		{ "Filter", FILTER },
		{ "Flame Body", FLAME_BODY },
		{ "Flash Fire", FLASH_FIRE },
		{ "Flower Gift", FLOWER_GIFT },
		{ "Forecast", FORECAST },
		{ "Forewarn", FOREWARN },
		{ "Frisk", FRISK },
		{ "Gluttony", GLUTTONY },
		{ "Guts", GUTS },
		{ "Heatproof", HEATPROOF },
		{ "Honey Gather", HONEY_GATHER },
		{ "Huge Power", HUGE_POWER },
		{ "Hustle", HUSTLE },
		{ "Hydration", HYDRATION },
		{ "Hyper Cutter", HYPER_CUTTER },
		{ "Ice Body", ICE_BODY },
		{ "Illuminate", ILLUMINATE },
		{ "Immunity", IMMUNITY },
		{ "Inner Focus", INNER_FOCUS },
		{ "Insomnia", INSOMNIA },
		{ "Intimidate", INTIMIDATE },
		{ "Iron Fist", IRON_FIST },
		{ "Keen Eye", KEEN_EYE },
		{ "Klutz", KLUTZ },
		{ "Leaf Guard", LEAF_GUARD },
		{ "Levitate", LEVITATE },
		{ "Lightningrod", LIGHTNINGROD },
		{ "Limber", LIMBER },
		{ "Liquid Ooze", LIQUID_OOZE },
		{ "Magic Guard", MAGIC_GUARD },
		{ "Magma Armor", MAGMA_ARMOR },
		{ "Magnet Pull", MAGNET_PULL },
		{ "Marvel Scale", MARVEL_SCALE },
		{ "Minus", MINUS },
		{ "Mold Breaker", MOLD_BREAKER },
		{ "Motor Drive", MOTOR_DRIVE },
		{ "Multitype", MULTITYPE },
		{ "Natural Cure", NATURAL_CURE },
		{ "No Guard", NO_GUARD },
		{ "Normalize", NORMALIZE },
		{ "Oblivious", OBLIVIOUS },
		{ "Overgrow", OVERGROW },
		{ "Own Tempo", OWN_TEMPO },
		{ "Pickup", PICKUP },
		{ "Plus", PLUS },
		{ "Poison Heal", POISON_HEAL },
		{ "Poison Point", POISON_POINT },
		{ "Pressure", PRESSURE },
		{ "Pure Power", PURE_POWER },
		{ "Quick Feet", QUICK_FEET },
		{ "Rain Dish", RAIN_DISH },
		{ "Reckless", RECKLESS },
		{ "Rivalry", RIVALRY },
		{ "Rock Head", ROCK_HEAD },
		{ "Rough Skin", ROUGH_SKIN },
		{ "Run Away", RUN_AWAY },
		{ "Sand Stream", SAND_STREAM },
		{ "Sand Veil", SAND_VEIL },
		{ "Scrappy", SCRAPPY },
		{ "Serene Grace", SERENE_GRACE },
		{ "Shadow Tag", SHADOW_TAG },
		{ "Shed Skin", SHED_SKIN },
		{ "Shell Armor", SHELL_ARMOR },
		{ "Shield Dust", SHIELD_DUST },
		{ "Simple", SIMPLE },
		{ "Skill Link", SKILL_LINK },
		{ "Slow Start", SLOW_START },
		{ "Sniper", SNIPER },
		{ "Snow Cloak", SNOW_CLOAK },
		{ "Snow Warning", SNOW_WARNING },
		{ "Solar Power", SOLAR_POWER },
		{ "Solid Rock", SOLID_ROCK },
		{ "Soundproof", SOUNDPROOF },
		{ "Speed Boost", SPEED_BOOST },
		{ "Stall", STALL },
		{ "Static", STATIC },
		{ "Steadfast", STEADFAST },
		{ "Stench", STENCH },
		{ "Sticky Hold", STICKY_HOLD },
		{ "Storm Drain", STORM_DRAIN },
		{ "Sturdy", STURDY },
		{ "Suction Cups", SUCTION_CUPS },
		{ "Super Luck", SUPER_LUCK },
		{ "Swarm", SWARM },
		{ "Swift Swim", SWIFT_SWIM },
		{ "Synchronize", SYNCHRONIZE },
		{ "Tangled Feet", TANGLED_FEET },
		{ "Technician", TECHNICIAN },
		{ "Thick Fat", THICK_FAT },
		{ "Tinted Lens", TINTED_LENS },
		{ "Torrent", TORRENT },
		{ "Trace", TRACE },
		{ "Truant", TRUANT },
		{ "Unaware", UNAWARE },
		{ "Unburden", UNBURDEN },
		{ "Vital Spirit", VITAL_SPIRIT },
		{ "Volt Absorb", VOLT_ABSORB },
		{ "Water Absorb", WATER_ABSORB },
		{ "Water Veil", WATER_VEIL },
		{ "White Smoke", WHITE_SMOKE },
		{ "Wonder Guard", WONDER_GUARD }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Ability", str);
}

void Ability::set_name_from_string (std::string const & str) {
	name = from_string (str);
}

}	// namespace technicalmachine
