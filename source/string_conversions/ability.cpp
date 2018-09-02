// Ability string conversions
// Copyright (C) 2018 David Stone
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

#include "invalid_string_conversion.hpp"

#include "../ability.hpp"

#include <map>

namespace technicalmachine {

std::string_view to_string(Ability const ability) {
	// The longest string here is 15 characters, which fits in the SSO buffer
	switch (ability) {
		case Ability::Adaptability: return "Adaptability";
		case Ability::Aftermath: return "Aftermath";
		case Ability::Air_Lock: return "Air Lock";
		case Ability::Anger_Point: return "Anger Point";
		case Ability::Anticipation: return "Anticipation";
		case Ability::Arena_Trap: return "Arena Trap";
		case Ability::Bad_Dreams: return "Bad Dreams";
		case Ability::Battle_Armor: return "Battle Armor";
		case Ability::Blaze: return "Blaze";
		case Ability::Chlorophyll: return "Chlorophyll";
		case Ability::Clear_Body: return "Clear Body";
		case Ability::Cloud_Nine: return "Cloud Nine";
		case Ability::Color_Change: return "Color Change";
		case Ability::Compoundeyes: return "Compoundeyes";
		case Ability::Cute_Charm: return "Cute Charm";
		case Ability::Damp: return "Damp";
		case Ability::Download: return "Download";
		case Ability::Drizzle: return "Drizzle";
		case Ability::Drought: return "Drought";
		case Ability::Dry_Skin: return "Dry Skin";
		case Ability::Early_Bird: return "Early Bird";
		case Ability::Effect_Spore: return "Effect Spore";
		case Ability::Filter: return "Filter";
		case Ability::Flame_Body: return "Flame Body";
		case Ability::Flash_Fire: return "Flash Fire";
		case Ability::Flower_Gift: return "Flower Gift";
		case Ability::Forecast: return "Forecast";
		case Ability::Forewarn: return "Forewarn";
		case Ability::Frisk: return "Frisk";
		case Ability::Gluttony: return "Gluttony";
		case Ability::Guts: return "Guts";
		case Ability::Heatproof: return "Heatproof";
		case Ability::Honey_Gather: return "Honey Gather";
		case Ability::Huge_Power: return "Huge Power";
		case Ability::Hustle: return "Hustle";
		case Ability::Hydration: return "Hydration";
		case Ability::Hyper_Cutter: return "Hyper Cutter";
		case Ability::Ice_Body: return "Ice Body";
		case Ability::Illuminate: return "Illuminate";
		case Ability::Immunity: return "Immunity";
		case Ability::Inner_Focus: return "Inner Focus";
		case Ability::Insomnia: return "Insomnia";
		case Ability::Intimidate: return "Intimidate";
		case Ability::Iron_Fist: return "Iron Fist";
		case Ability::Keen_Eye: return "Keen Eye";
		case Ability::Klutz: return "Klutz";
		case Ability::Leaf_Guard: return "Leaf Guard";
		case Ability::Levitate: return "Levitate";
		case Ability::Lightningrod: return "Lightningrod";
		case Ability::Limber: return "Limber";
		case Ability::Liquid_Ooze: return "Liquid Ooze";
		case Ability::Magic_Guard: return "Magic Guard";
		case Ability::Magma_Armor: return "Magma Armor";
		case Ability::Magnet_Pull: return "Magnet Pull";
		case Ability::Marvel_Scale: return "Marvel Scale";
		case Ability::Minus: return "Minus";
		case Ability::Mold_Breaker: return "Mold Breaker";
		case Ability::Motor_Drive: return "Motor Drive";
		case Ability::Multitype: return "Multitype";
		case Ability::Natural_Cure: return "Natural Cure";
		case Ability::No_Guard: return "No Guard";
		case Ability::Normalize: return "Normalize";
		case Ability::Oblivious: return "Oblivious";
		case Ability::Overgrow: return "Overgrow";
		case Ability::Own_Tempo: return "Own Tempo";
		case Ability::Pickup: return "Pickup";
		case Ability::Plus: return "Plus";
		case Ability::Poison_Heal: return "Poison Heal";
		case Ability::Poison_Point: return "Poison Point";
		case Ability::Pressure: return "Pressure";
		case Ability::Pure_Power: return "Pure Power";
		case Ability::Quick_Feet: return "Quick Feet";
		case Ability::Rain_Dish: return "Rain Dish";
		case Ability::Reckless: return "Reckless";
		case Ability::Rivalry: return "Rivalry";
		case Ability::Rock_Head: return "Rock Head";
		case Ability::Rough_Skin: return "Rough Skin";
		case Ability::Run_Away: return "Run Away";
		case Ability::Sand_Stream: return "Sand Stream";
		case Ability::Sand_Veil: return "Sand Veil";
		case Ability::Scrappy: return "Scrappy";
		case Ability::Serene_Grace: return "Serene Grace";
		case Ability::Shadow_Tag: return "Shadow Tag";
		case Ability::Shed_Skin: return "Shed Skin";
		case Ability::Shell_Armor: return "Shell Armor";
		case Ability::Shield_Dust: return "Shield Dust";
		case Ability::Simple: return "Simple";
		case Ability::Skill_Link: return "Skill Link";
		case Ability::Slow_Start: return "Slow Start";
		case Ability::Sniper: return "Sniper";
		case Ability::Snow_Cloak: return "Snow Cloak";
		case Ability::Snow_Warning: return "Snow Warning";
		case Ability::Solar_Power: return "Solar Power";
		case Ability::Solid_Rock: return "Solid Rock";
		case Ability::Soundproof: return "Soundproof";
		case Ability::Speed_Boost: return "Speed Boost";
		case Ability::Stall: return "Stall";
		case Ability::Static: return "Static";
		case Ability::Steadfast: return "Steadfast";
		case Ability::Stench: return "Stench";
		case Ability::Sticky_Hold: return "Sticky Hold";
		case Ability::Storm_Drain: return "Storm Drain";
		case Ability::Sturdy: return "Sturdy";
		case Ability::Suction_Cups: return "Suction Cups";
		case Ability::Super_Luck: return "Super Luck";
		case Ability::Swarm: return "Swarm";
		case Ability::Swift_Swim: return "Swift Swim";
		case Ability::Synchronize: return "Synchronize";
		case Ability::Tangled_Feet: return "Tangled Feet";
		case Ability::Technician: return "Technician";
		case Ability::Thick_Fat: return "Thick Fat";
		case Ability::Tinted_Lens: return "Tinted Lens";
		case Ability::Torrent: return "Torrent";
		case Ability::Trace: return "Trace";
		case Ability::Truant: return "Truant";
		case Ability::Unaware: return "Unaware";
		case Ability::Unburden: return "Unburden";
		case Ability::Vital_Spirit: return "Vital Spirit";
		case Ability::Volt_Absorb: return "Volt Absorb";
		case Ability::Water_Absorb: return "Water Absorb";
		case Ability::Water_Veil: return "Water Veil";
		case Ability::White_Smoke: return "White Smoke";
		case Ability::Wonder_Guard: return "Wonder Guard";
	}
}

template<>
Ability from_string<Ability>(std::string_view const str) {
	static std::map<std::string_view, Ability, lowercase_ordering> const converter {
		{ "Adaptability", Ability::Adaptability },
		{ "Aftermath", Ability::Aftermath },
		{ "Air Lock", Ability::Air_Lock },
		{ "AirLock", Ability::Air_Lock },
		{ "Anger Point", Ability::Anger_Point },
		{ "AngerPoint", Ability::Anger_Point },
		{ "Anticipation", Ability::Anticipation },
		{ "Arena Trap", Ability::Arena_Trap },
		{ "ArenaTrap", Ability::Arena_Trap },
		{ "Bad Dreams", Ability::Bad_Dreams },
		{ "BadDreams", Ability::Bad_Dreams },
		{ "Battle Armor", Ability::Battle_Armor },
		{ "BattleArmor", Ability::Battle_Armor },
		{ "Blaze", Ability::Blaze },
		{ "Chlorophyll", Ability::Chlorophyll },
		{ "Clear Body", Ability::Clear_Body },
		{ "ClearBody", Ability::Clear_Body },
		{ "Cloud Nine", Ability::Cloud_Nine },
		{ "CloudNine", Ability::Cloud_Nine },
		{ "Color Change", Ability::Color_Change },
		{ "ColorChange", Ability::Color_Change },
		{ "Compoundeyes", Ability::Compoundeyes },
		{ "Cute Charm", Ability::Cute_Charm },
		{ "CuteCharm", Ability::Cute_Charm },
		{ "Damp", Ability::Damp },
		{ "Download", Ability::Download },
		{ "Drizzle", Ability::Drizzle },
		{ "Drought", Ability::Drought },
		{ "Dry Skin", Ability::Dry_Skin },
		{ "DrySkin", Ability::Dry_Skin },
		{ "Early Bird", Ability::Early_Bird },
		{ "EarlyBird", Ability::Early_Bird },
		{ "Effect Spore", Ability::Effect_Spore },
		{ "EffectSpore", Ability::Effect_Spore },
		{ "Filter", Ability::Filter },
		{ "Flame Body", Ability::Flame_Body },
		{ "FlameBody", Ability::Flame_Body },
		{ "Flash Fire", Ability::Flash_Fire },
		{ "FlashFire", Ability::Flash_Fire },
		{ "Flower Gift", Ability::Flower_Gift },
		{ "FlowerGift", Ability::Flower_Gift },
		{ "Forecast", Ability::Forecast },
		{ "Forewarn", Ability::Forewarn },
		{ "Frisk", Ability::Frisk },
		{ "Gluttony", Ability::Gluttony },
		{ "Guts", Ability::Guts },
		{ "Heatproof", Ability::Heatproof },
		{ "Honey Gather", Ability::Honey_Gather },
		{ "HoneyGather", Ability::Honey_Gather },
		{ "Huge Power", Ability::Huge_Power },
		{ "HugePower", Ability::Huge_Power },
		{ "Hustle", Ability::Hustle },
		{ "Hydration", Ability::Hydration },
		{ "Hyper Cutter", Ability::Hyper_Cutter },
		{ "HyperCutter", Ability::Hyper_Cutter },
		{ "Ice Body", Ability::Ice_Body },
		{ "IceBody", Ability::Ice_Body },
		{ "Illuminate", Ability::Illuminate },
		{ "Immunity", Ability::Immunity },
		{ "Inner Focus", Ability::Inner_Focus },
		{ "InnerFocus", Ability::Inner_Focus },
		{ "Insomnia", Ability::Insomnia },
		{ "Intimidate", Ability::Intimidate },
		{ "Iron Fist", Ability::Iron_Fist },
		{ "IronFist", Ability::Iron_Fist },
		{ "Keen Eye", Ability::Keen_Eye },
		{ "KeenEye", Ability::Keen_Eye },
		{ "Klutz", Ability::Klutz },
		{ "Leaf Guard", Ability::Leaf_Guard },
		{ "LeafGuard", Ability::Leaf_Guard },
		{ "Levitate", Ability::Levitate },
		{ "Lightningrod", Ability::Lightningrod },
		{ "Limber", Ability::Limber },
		{ "Liquid Ooze", Ability::Liquid_Ooze },
		{ "LiquidOoze", Ability::Liquid_Ooze },
		{ "Magic Guard", Ability::Magic_Guard },
		{ "MagicGuard", Ability::Magic_Guard },
		{ "Magma Armor", Ability::Magma_Armor },
		{ "MagmaArmor", Ability::Magma_Armor },
		{ "Magnet Pull", Ability::Magnet_Pull },
		{ "MagnetPull", Ability::Magnet_Pull },
		{ "Marvel Scale", Ability::Marvel_Scale },
		{ "MarvelScale", Ability::Marvel_Scale },
		{ "Minus", Ability::Minus },
		{ "Mold Breaker", Ability::Mold_Breaker },
		{ "MoldBreaker", Ability::Mold_Breaker },
		{ "Motor Drive", Ability::Motor_Drive },
		{ "MotorDrive", Ability::Motor_Drive },
		{ "Multitype", Ability::Multitype },
		{ "Natural Cure", Ability::Natural_Cure },
		{ "NaturalCure", Ability::Natural_Cure },
		{ "No Guard", Ability::No_Guard },
		{ "NoGuard", Ability::No_Guard },
		{ "Normalize", Ability::Normalize },
		{ "Oblivious", Ability::Oblivious },
		{ "Overgrow", Ability::Overgrow },
		{ "Own Tempo", Ability::Own_Tempo },
		{ "OwnTempo", Ability::Own_Tempo },
		{ "Pickup", Ability::Pickup },
		{ "Plus", Ability::Plus },
		{ "Poison Heal", Ability::Poison_Heal },
		{ "PoisonHeal", Ability::Poison_Heal },
		{ "Poison Point", Ability::Poison_Point },
		{ "PoisonPoint", Ability::Poison_Point },
		{ "Pressure", Ability::Pressure },
		{ "Pure Power", Ability::Pure_Power },
		{ "PurePower", Ability::Pure_Power },
		{ "Quick Feet", Ability::Quick_Feet },
		{ "QuickFeet", Ability::Quick_Feet },
		{ "Rain Dish", Ability::Rain_Dish },
		{ "RainDish", Ability::Rain_Dish },
		{ "Reckless", Ability::Reckless },
		{ "Rivalry", Ability::Rivalry },
		{ "Rock Head", Ability::Rock_Head },
		{ "RockHead", Ability::Rock_Head },
		{ "Rough Skin", Ability::Rough_Skin },
		{ "RoughSkin", Ability::Rough_Skin },
		{ "Run Away", Ability::Run_Away },
		{ "RunAway", Ability::Run_Away },
		{ "Sand Stream", Ability::Sand_Stream },
		{ "SandStream", Ability::Sand_Stream },
		{ "Sand Veil", Ability::Sand_Veil },
		{ "SandVeil", Ability::Sand_Veil },
		{ "Scrappy", Ability::Scrappy },
		{ "Serene Grace", Ability::Serene_Grace },
		{ "SereneGrace", Ability::Serene_Grace },
		{ "Shadow Tag", Ability::Shadow_Tag },
		{ "ShadowTag", Ability::Shadow_Tag },
		{ "Shed Skin", Ability::Shed_Skin },
		{ "ShedSkin", Ability::Shed_Skin },
		{ "Shell Armor", Ability::Shell_Armor },
		{ "ShellArmor", Ability::Shell_Armor },
		{ "Shield Dust", Ability::Shield_Dust },
		{ "ShieldDust", Ability::Shield_Dust },
		{ "Simple", Ability::Simple },
		{ "Skill Link", Ability::Skill_Link },
		{ "SkillLink", Ability::Skill_Link },
		{ "Slow Start", Ability::Slow_Start },
		{ "SlowStart", Ability::Slow_Start },
		{ "Sniper", Ability::Sniper },
		{ "Snow Cloak", Ability::Snow_Cloak },
		{ "SnowCloak", Ability::Snow_Cloak },
		{ "Snow Warning", Ability::Snow_Warning },
		{ "SnowWarning", Ability::Snow_Warning },
		{ "Solar Power", Ability::Solar_Power },
		{ "SolarPower", Ability::Solar_Power },
		{ "Solid Rock", Ability::Solid_Rock },
		{ "SolidRock", Ability::Solid_Rock },
		{ "Soundproof", Ability::Soundproof },
		{ "Speed Boost", Ability::Speed_Boost },
		{ "SpeedBoost", Ability::Speed_Boost },
		{ "Stall", Ability::Stall },
		{ "Static", Ability::Static },
		{ "Steadfast", Ability::Steadfast },
		{ "Stench", Ability::Stench },
		{ "Sticky Hold", Ability::Sticky_Hold },
		{ "StickyHold", Ability::Sticky_Hold },
		{ "Storm Drain", Ability::Storm_Drain },
		{ "StormDrain", Ability::Storm_Drain },
		{ "Sturdy", Ability::Sturdy },
		{ "Suction Cups", Ability::Suction_Cups },
		{ "SuctionCups", Ability::Suction_Cups },
		{ "Super Luck", Ability::Super_Luck },
		{ "SuperLuck", Ability::Super_Luck },
		{ "Swarm", Ability::Swarm },
		{ "Swift Swim", Ability::Swift_Swim },
		{ "SwiftSwim", Ability::Swift_Swim },
		{ "Synchronize", Ability::Synchronize },
		{ "Tangled Feet", Ability::Tangled_Feet },
		{ "TangledFeet", Ability::Tangled_Feet },
		{ "Technician", Ability::Technician },
		{ "Thick Fat", Ability::Thick_Fat },
		{ "ThickFat", Ability::Thick_Fat },
		{ "Tinted Lens", Ability::Tinted_Lens },
		{ "TintedLens", Ability::Tinted_Lens },
		{ "Torrent", Ability::Torrent },
		{ "Trace", Ability::Trace },
		{ "Truant", Ability::Truant },
		{ "Unaware", Ability::Unaware },
		{ "Unburden", Ability::Unburden },
		{ "Vital Spirit", Ability::Vital_Spirit },
		{ "VitalSpirit", Ability::Vital_Spirit },
		{ "Volt Absorb", Ability::Volt_Absorb },
		{ "VoltAbsorb", Ability::Volt_Absorb },
		{ "Water Absorb", Ability::Water_Absorb },
		{ "WaterAbsorb", Ability::Water_Absorb },
		{ "Water Veil", Ability::Water_Veil },
		{ "WaterVeil", Ability::Water_Veil },
		{ "White Smoke", Ability::White_Smoke },
		{ "WhiteSmoke", Ability::White_Smoke },
		{ "Wonder Guard", Ability::Wonder_Guard }
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Ability", str);
	}
}

}	// namespace technicalmachine
