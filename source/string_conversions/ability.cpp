// Ability string conversions
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

#include "conversion.hpp"

#include <map>
#include <string>

#include "invalid_string_conversion.hpp"

#include "../ability.hpp"

namespace technicalmachine {

template<>
std::string to_string(Ability::Abilities const name) {
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

template<>
Ability::Abilities from_string<Ability::Abilities>(std::string const & str) {
	static std::map <std::string, Ability::Abilities> const converter {
		{ "Adaptability", Ability::ADAPTABILITY },
		{ "Aftermath", Ability::AFTERMATH },
		{ "Air Lock", Ability::AIR_LOCK },
		{ "Anger Point", Ability::ANGER_POINT },
		{ "Anticipation", Ability::ANTICIPATION },
		{ "Arena Trap", Ability::ARENA_TRAP },
		{ "Bad Dreams", Ability::BAD_DREAMS },
		{ "Battle Armor", Ability::BATTLE_ARMOR },
		{ "Blaze", Ability::BLAZE },
		{ "Chlorophyll", Ability::CHLOROPHYLL },
		{ "Clear Body", Ability::CLEAR_BODY },
		{ "Cloud Nine", Ability::CLOUD_NINE },
		{ "Color Change", Ability::COLOR_CHANGE },
		{ "Compoundeyes", Ability::COMPOUNDEYES },
		{ "Cute Charm", Ability::CUTE_CHARM },
		{ "Damp", Ability::DAMP },
		{ "Download", Ability::DOWNLOAD },
		{ "Drizzle", Ability::DRIZZLE },
		{ "Drought", Ability::DROUGHT },
		{ "Dry Skin", Ability::DRY_SKIN },
		{ "Early Bird", Ability::EARLY_BIRD },
		{ "Effect Spore", Ability::EFFECT_SPORE },
		{ "Filter", Ability::FILTER },
		{ "Flame Body", Ability::FLAME_BODY },
		{ "Flash Fire", Ability::FLASH_FIRE },
		{ "Flower Gift", Ability::FLOWER_GIFT },
		{ "Forecast", Ability::FORECAST },
		{ "Forewarn", Ability::FOREWARN },
		{ "Frisk", Ability::FRISK },
		{ "Gluttony", Ability::GLUTTONY },
		{ "Guts", Ability::GUTS },
		{ "Heatproof", Ability::HEATPROOF },
		{ "Honey Gather", Ability::HONEY_GATHER },
		{ "Huge Power", Ability::HUGE_POWER },
		{ "Hustle", Ability::HUSTLE },
		{ "Hydration", Ability::HYDRATION },
		{ "Hyper Cutter", Ability::HYPER_CUTTER },
		{ "Ice Body", Ability::ICE_BODY },
		{ "Illuminate", Ability::ILLUMINATE },
		{ "Immunity", Ability::IMMUNITY },
		{ "Inner Focus", Ability::INNER_FOCUS },
		{ "Insomnia", Ability::INSOMNIA },
		{ "Intimidate", Ability::INTIMIDATE },
		{ "Iron Fist", Ability::IRON_FIST },
		{ "Keen Eye", Ability::KEEN_EYE },
		{ "Klutz", Ability::KLUTZ },
		{ "Leaf Guard", Ability::LEAF_GUARD },
		{ "Levitate", Ability::LEVITATE },
		{ "Lightningrod", Ability::LIGHTNINGROD },
		{ "Limber", Ability::LIMBER },
		{ "Liquid Ooze", Ability::LIQUID_OOZE },
		{ "Magic Guard", Ability::MAGIC_GUARD },
		{ "Magma Armor", Ability::MAGMA_ARMOR },
		{ "Magnet Pull", Ability::MAGNET_PULL },
		{ "Marvel Scale", Ability::MARVEL_SCALE },
		{ "Minus", Ability::MINUS },
		{ "Mold Breaker", Ability::MOLD_BREAKER },
		{ "Motor Drive", Ability::MOTOR_DRIVE },
		{ "Multitype", Ability::MULTITYPE },
		{ "Natural Cure", Ability::NATURAL_CURE },
		{ "No Guard", Ability::NO_GUARD },
		{ "Normalize", Ability::NORMALIZE },
		{ "Oblivious", Ability::OBLIVIOUS },
		{ "Overgrow", Ability::OVERGROW },
		{ "Own Tempo", Ability::OWN_TEMPO },
		{ "Pickup", Ability::PICKUP },
		{ "Plus", Ability::PLUS },
		{ "Poison Heal", Ability::POISON_HEAL },
		{ "Poison Point", Ability::POISON_POINT },
		{ "Pressure", Ability::PRESSURE },
		{ "Pure Power", Ability::PURE_POWER },
		{ "Quick Feet", Ability::QUICK_FEET },
		{ "Rain Dish", Ability::RAIN_DISH },
		{ "Reckless", Ability::RECKLESS },
		{ "Rivalry", Ability::RIVALRY },
		{ "Rock Head", Ability::ROCK_HEAD },
		{ "Rough Skin", Ability::ROUGH_SKIN },
		{ "Run Away", Ability::RUN_AWAY },
		{ "Sand Stream", Ability::SAND_STREAM },
		{ "Sand Veil", Ability::SAND_VEIL },
		{ "Scrappy", Ability::SCRAPPY },
		{ "Serene Grace", Ability::SERENE_GRACE },
		{ "Shadow Tag", Ability::SHADOW_TAG },
		{ "Shed Skin", Ability::SHED_SKIN },
		{ "Shell Armor", Ability::SHELL_ARMOR },
		{ "Shield Dust", Ability::SHIELD_DUST },
		{ "Simple", Ability::SIMPLE },
		{ "Skill Link", Ability::SKILL_LINK },
		{ "Slow Start", Ability::SLOW_START },
		{ "Sniper", Ability::SNIPER },
		{ "Snow Cloak", Ability::SNOW_CLOAK },
		{ "Snow Warning", Ability::SNOW_WARNING },
		{ "Solar Power", Ability::SOLAR_POWER },
		{ "Solid Rock", Ability::SOLID_ROCK },
		{ "Soundproof", Ability::SOUNDPROOF },
		{ "Speed Boost", Ability::SPEED_BOOST },
		{ "Stall", Ability::STALL },
		{ "Static", Ability::STATIC },
		{ "Steadfast", Ability::STEADFAST },
		{ "Stench", Ability::STENCH },
		{ "Sticky Hold", Ability::STICKY_HOLD },
		{ "Storm Drain", Ability::STORM_DRAIN },
		{ "Sturdy", Ability::STURDY },
		{ "Suction Cups", Ability::SUCTION_CUPS },
		{ "Super Luck", Ability::SUPER_LUCK },
		{ "Swarm", Ability::SWARM },
		{ "Swift Swim", Ability::SWIFT_SWIM },
		{ "Synchronize", Ability::SYNCHRONIZE },
		{ "Tangled Feet", Ability::TANGLED_FEET },
		{ "Technician", Ability::TECHNICIAN },
		{ "Thick Fat", Ability::THICK_FAT },
		{ "Tinted Lens", Ability::TINTED_LENS },
		{ "Torrent", Ability::TORRENT },
		{ "Trace", Ability::TRACE },
		{ "Truant", Ability::TRUANT },
		{ "Unaware", Ability::UNAWARE },
		{ "Unburden", Ability::UNBURDEN },
		{ "Vital Spirit", Ability::VITAL_SPIRIT },
		{ "Volt Absorb", Ability::VOLT_ABSORB },
		{ "Water Absorb", Ability::WATER_ABSORB },
		{ "Water Veil", Ability::WATER_VEIL },
		{ "White Smoke", Ability::WHITE_SMOKE },
		{ "Wonder Guard", Ability::WONDER_GUARD }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Ability", str);
}

}	// namespace technicalmachine
