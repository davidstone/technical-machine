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
	static std::map<std::string_view, Ability, lowercase_alphanumeric> const converter {
		{ "adaptability", Ability::Adaptability },
		{ "aftermath", Ability::Aftermath },
		{ "AirLock", Ability::Air_Lock },
		{ "AngerPoint", Ability::Anger_Point },
		{ "Anticipation", Ability::Anticipation },
		{ "ArenaTrap", Ability::Arena_Trap },
		{ "BadDreams", Ability::Bad_Dreams },
		{ "BattleArmor", Ability::Battle_Armor },
		{ "Blaze", Ability::Blaze },
		{ "Chlorophyll", Ability::Chlorophyll },
		{ "ClearBody", Ability::Clear_Body },
		{ "CloudNine", Ability::Cloud_Nine },
		{ "ColorChange", Ability::Color_Change },
		{ "Compoundeyes", Ability::Compoundeyes },
		{ "CuteCharm", Ability::Cute_Charm },
		{ "Damp", Ability::Damp },
		{ "Download", Ability::Download },
		{ "Drizzle", Ability::Drizzle },
		{ "Drought", Ability::Drought },
		{ "DrySkin", Ability::Dry_Skin },
		{ "EarlyBird", Ability::Early_Bird },
		{ "EffectSpore", Ability::Effect_Spore },
		{ "Filter", Ability::Filter },
		{ "FlameBody", Ability::Flame_Body },
		{ "FlashFire", Ability::Flash_Fire },
		{ "FlowerGift", Ability::Flower_Gift },
		{ "Forecast", Ability::Forecast },
		{ "Forewarn", Ability::Forewarn },
		{ "Frisk", Ability::Frisk },
		{ "Gluttony", Ability::Gluttony },
		{ "Guts", Ability::Guts },
		{ "Heatproof", Ability::Heatproof },
		{ "HoneyGather", Ability::Honey_Gather },
		{ "HugePower", Ability::Huge_Power },
		{ "Hustle", Ability::Hustle },
		{ "Hydration", Ability::Hydration },
		{ "HyperCutter", Ability::Hyper_Cutter },
		{ "IceBody", Ability::Ice_Body },
		{ "Illuminate", Ability::Illuminate },
		{ "Immunity", Ability::Immunity },
		{ "InnerFocus", Ability::Inner_Focus },
		{ "Insomnia", Ability::Insomnia },
		{ "Intimidate", Ability::Intimidate },
		{ "IronFist", Ability::Iron_Fist },
		{ "KeenEye", Ability::Keen_Eye },
		{ "Klutz", Ability::Klutz },
		{ "LeafGuard", Ability::Leaf_Guard },
		{ "Levitate", Ability::Levitate },
		{ "Lightningrod", Ability::Lightningrod },
		{ "Limber", Ability::Limber },
		{ "LiquidOoze", Ability::Liquid_Ooze },
		{ "MagicGuard", Ability::Magic_Guard },
		{ "MagmaArmor", Ability::Magma_Armor },
		{ "MagnetPull", Ability::Magnet_Pull },
		{ "MarvelScale", Ability::Marvel_Scale },
		{ "Minus", Ability::Minus },
		{ "MoldBreaker", Ability::Mold_Breaker },
		{ "MotorDrive", Ability::Motor_Drive },
		{ "Multitype", Ability::Multitype },
		{ "NaturalCure", Ability::Natural_Cure },
		{ "NoGuard", Ability::No_Guard },
		{ "Normalize", Ability::Normalize },
		{ "Oblivious", Ability::Oblivious },
		{ "Overgrow", Ability::Overgrow },
		{ "OwnTempo", Ability::Own_Tempo },
		{ "Pickup", Ability::Pickup },
		{ "Plus", Ability::Plus },
		{ "PoisonHeal", Ability::Poison_Heal },
		{ "PoisonPoint", Ability::Poison_Point },
		{ "Pressure", Ability::Pressure },
		{ "PurePower", Ability::Pure_Power },
		{ "QuickFeet", Ability::Quick_Feet },
		{ "RainDish", Ability::Rain_Dish },
		{ "Reckless", Ability::Reckless },
		{ "Rivalry", Ability::Rivalry },
		{ "RockHead", Ability::Rock_Head },
		{ "RoughSkin", Ability::Rough_Skin },
		{ "RunAway", Ability::Run_Away },
		{ "SandStream", Ability::Sand_Stream },
		{ "SandVeil", Ability::Sand_Veil },
		{ "Scrappy", Ability::Scrappy },
		{ "SereneGrace", Ability::Serene_Grace },
		{ "ShadowTag", Ability::Shadow_Tag },
		{ "ShedSkin", Ability::Shed_Skin },
		{ "ShellArmor", Ability::Shell_Armor },
		{ "ShieldDust", Ability::Shield_Dust },
		{ "Simple", Ability::Simple },
		{ "SkillLink", Ability::Skill_Link },
		{ "SlowStart", Ability::Slow_Start },
		{ "Sniper", Ability::Sniper },
		{ "SnowCloak", Ability::Snow_Cloak },
		{ "SnowWarning", Ability::Snow_Warning },
		{ "SolarPower", Ability::Solar_Power },
		{ "SolidRock", Ability::Solid_Rock },
		{ "Soundproof", Ability::Soundproof },
		{ "SpeedBoost", Ability::Speed_Boost },
		{ "Stall", Ability::Stall },
		{ "Static", Ability::Static },
		{ "Steadfast", Ability::Steadfast },
		{ "Stench", Ability::Stench },
		{ "StickyHold", Ability::Sticky_Hold },
		{ "StormDrain", Ability::Storm_Drain },
		{ "Sturdy", Ability::Sturdy },
		{ "SuctionCups", Ability::Suction_Cups },
		{ "SuperLuck", Ability::Super_Luck },
		{ "Swarm", Ability::Swarm },
		{ "SwiftSwim", Ability::Swift_Swim },
		{ "Synchronize", Ability::Synchronize },
		{ "TangledFeet", Ability::Tangled_Feet },
		{ "Technician", Ability::Technician },
		{ "ThickFat", Ability::Thick_Fat },
		{ "TintedLens", Ability::Tinted_Lens },
		{ "Torrent", Ability::Torrent },
		{ "Trace", Ability::Trace },
		{ "Truant", Ability::Truant },
		{ "Unaware", Ability::Unaware },
		{ "Unburden", Ability::Unburden },
		{ "VitalSpirit", Ability::Vital_Spirit },
		{ "VoltAbsorb", Ability::Volt_Absorb },
		{ "WaterAbsorb", Ability::Water_Absorb },
		{ "WaterVeil", Ability::Water_Veil },
		{ "WhiteSmoke", Ability::White_Smoke },
		{ "WonderGuard", Ability::Wonder_Guard },
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Ability", str);
	}
}

}	// namespace technicalmachine
