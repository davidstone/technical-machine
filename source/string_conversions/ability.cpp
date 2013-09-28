// Ability string conversions
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

#include "ability.hpp"

#include <map>
#include <string>

#include "invalid_string_conversion.hpp"

#include "../ability.hpp"

namespace technicalmachine {

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
		{ "Adaptability", Ability::Adaptability },
		{ "Aftermath", Ability::Aftermath },
		{ "Air Lock", Ability::Air_Lock },
		{ "Anger Point", Ability::Anger_Point },
		{ "Anticipation", Ability::Anticipation },
		{ "Arena Trap", Ability::Arena_Trap },
		{ "Bad Dreams", Ability::Bad_Dreams },
		{ "Battle Armor", Ability::Battle_Armor },
		{ "Blaze", Ability::Blaze },
		{ "Chlorophyll", Ability::Chlorophyll },
		{ "Clear Body", Ability::Clear_Body },
		{ "Cloud Nine", Ability::Cloud_Nine },
		{ "Color Change", Ability::Color_Change },
		{ "Compoundeyes", Ability::Compoundeyes },
		{ "Cute Charm", Ability::Cute_Charm },
		{ "Damp", Ability::Damp },
		{ "Download", Ability::Download },
		{ "Drizzle", Ability::Drizzle },
		{ "Drought", Ability::Drought },
		{ "Dry Skin", Ability::Dry_Skin },
		{ "Early Bird", Ability::Early_Bird },
		{ "Effect Spore", Ability::Effect_Spore },
		{ "Filter", Ability::Filter },
		{ "Flame Body", Ability::Flame_Body },
		{ "Flash Fire", Ability::Flash_Fire },
		{ "Flower Gift", Ability::Flower_Gift },
		{ "Forecast", Ability::Forecast },
		{ "Forewarn", Ability::Forewarn },
		{ "Frisk", Ability::Frisk },
		{ "Gluttony", Ability::Gluttony },
		{ "Guts", Ability::Guts },
		{ "Heatproof", Ability::Heatproof },
		{ "Honey Gather", Ability::Honey_Gather },
		{ "Huge Power", Ability::Huge_Power },
		{ "Hustle", Ability::Hustle },
		{ "Hydration", Ability::Hydration },
		{ "Hyper Cutter", Ability::Hyper_Cutter },
		{ "Ice Body", Ability::Ice_Body },
		{ "Illuminate", Ability::Illuminate },
		{ "Immunity", Ability::Immunity },
		{ "Inner Focus", Ability::Inner_Focus },
		{ "Insomnia", Ability::Insomnia },
		{ "Intimidate", Ability::Intimidate },
		{ "Iron Fist", Ability::Iron_Fist },
		{ "Keen Eye", Ability::Keen_Eye },
		{ "Klutz", Ability::Klutz },
		{ "Leaf Guard", Ability::Leaf_Guard },
		{ "Levitate", Ability::Levitate },
		{ "Lightningrod", Ability::Lightningrod },
		{ "Limber", Ability::Limber },
		{ "Liquid Ooze", Ability::Liquid_Ooze },
		{ "Magic Guard", Ability::Magic_Guard },
		{ "Magma Armor", Ability::Magma_Armor },
		{ "Magnet Pull", Ability::Magnet_Pull },
		{ "Marvel Scale", Ability::Marvel_Scale },
		{ "Minus", Ability::Minus },
		{ "Mold Breaker", Ability::Mold_Breaker },
		{ "Motor Drive", Ability::Motor_Drive },
		{ "Multitype", Ability::Multitype },
		{ "Natural Cure", Ability::Natural_Cure },
		{ "No Guard", Ability::No_Guard },
		{ "Normalize", Ability::Normalize },
		{ "Oblivious", Ability::Oblivious },
		{ "Overgrow", Ability::Overgrow },
		{ "Own Tempo", Ability::Own_Tempo },
		{ "Pickup", Ability::Pickup },
		{ "Plus", Ability::Plus },
		{ "Poison Heal", Ability::Poison_Heal },
		{ "Poison Point", Ability::Poison_Point },
		{ "Pressure", Ability::Pressure },
		{ "Pure Power", Ability::Pure_Power },
		{ "Quick Feet", Ability::Quick_Feet },
		{ "Rain Dish", Ability::Rain_Dish },
		{ "Reckless", Ability::Reckless },
		{ "Rivalry", Ability::Rivalry },
		{ "Rock Head", Ability::Rock_Head },
		{ "Rough Skin", Ability::Rough_Skin },
		{ "Run Away", Ability::Run_Away },
		{ "Sand Stream", Ability::Sand_Stream },
		{ "Sand Veil", Ability::Sand_Veil },
		{ "Scrappy", Ability::Scrappy },
		{ "Serene Grace", Ability::Serene_Grace },
		{ "Shadow Tag", Ability::Shadow_Tag },
		{ "Shed Skin", Ability::Shed_Skin },
		{ "Shell Armor", Ability::Shell_Armor },
		{ "Shield Dust", Ability::Shield_Dust },
		{ "Simple", Ability::Simple },
		{ "Skill Link", Ability::Skill_Link },
		{ "Slow Start", Ability::Slow_Start },
		{ "Sniper", Ability::Sniper },
		{ "Snow Cloak", Ability::Snow_Cloak },
		{ "Snow Warning", Ability::Snow_Warning },
		{ "Solar Power", Ability::Solar_Power },
		{ "Solid Rock", Ability::Solid_Rock },
		{ "Soundproof", Ability::Soundproof },
		{ "Speed Boost", Ability::Speed_Boost },
		{ "Stall", Ability::Stall },
		{ "Static", Ability::Static },
		{ "Steadfast", Ability::Steadfast },
		{ "Stench", Ability::Stench },
		{ "Sticky Hold", Ability::Sticky_Hold },
		{ "Storm Drain", Ability::Storm_Drain },
		{ "Sturdy", Ability::Sturdy },
		{ "Suction Cups", Ability::Suction_Cups },
		{ "Super Luck", Ability::Super_Luck },
		{ "Swarm", Ability::Swarm },
		{ "Swift Swim", Ability::Swift_Swim },
		{ "Synchronize", Ability::Synchronize },
		{ "Tangled Feet", Ability::Tangled_Feet },
		{ "Technician", Ability::Technician },
		{ "Thick Fat", Ability::Thick_Fat },
		{ "Tinted Lens", Ability::Tinted_Lens },
		{ "Torrent", Ability::Torrent },
		{ "Trace", Ability::Trace },
		{ "Truant", Ability::Truant },
		{ "Unaware", Ability::Unaware },
		{ "Unburden", Ability::Unburden },
		{ "Vital Spirit", Ability::Vital_Spirit },
		{ "Volt Absorb", Ability::Volt_Absorb },
		{ "Water Absorb", Ability::Water_Absorb },
		{ "Water Veil", Ability::Water_Veil },
		{ "White Smoke", Ability::White_Smoke },
		{ "Wonder Guard", Ability::Wonder_Guard }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Ability", str);
}

}	// namespace technicalmachine
