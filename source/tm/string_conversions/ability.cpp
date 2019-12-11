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

#include <tm/string_conversions/ability.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/ability.hpp>

#include <containers/array/array.hpp>
#include <containers/flat_map.hpp>

namespace technicalmachine {

std::string_view to_string(Ability const ability) {
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
		case Ability::Compound_Eyes: return "Compound Eyes";
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
	using Storage = containers::array<containers::map_value_type<std::string_view, Ability>, 123>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "adaptability", Ability::Adaptability },
			{ "aftermath", Ability::Aftermath },
			{ "airlock", Ability::Air_Lock },
			{ "angerpoint", Ability::Anger_Point },
			{ "anticipation", Ability::Anticipation },
			{ "arenatrap", Ability::Arena_Trap },
			{ "baddreams", Ability::Bad_Dreams },
			{ "battlearmor", Ability::Battle_Armor },
			{ "blaze", Ability::Blaze },
			{ "chlorophyll", Ability::Chlorophyll },
			{ "clearbody", Ability::Clear_Body },
			{ "cloudnine", Ability::Cloud_Nine },
			{ "colorchange", Ability::Color_Change },
			{ "compoundeyes", Ability::Compound_Eyes },
			{ "cutecharm", Ability::Cute_Charm },
			{ "damp", Ability::Damp },
			{ "download", Ability::Download },
			{ "drizzle", Ability::Drizzle },
			{ "drought", Ability::Drought },
			{ "dryskin", Ability::Dry_Skin },
			{ "earlybird", Ability::Early_Bird },
			{ "effectspore", Ability::Effect_Spore },
			{ "filter", Ability::Filter },
			{ "flamebody", Ability::Flame_Body },
			{ "flashfire", Ability::Flash_Fire },
			{ "flowergift", Ability::Flower_Gift },
			{ "forecast", Ability::Forecast },
			{ "forewarn", Ability::Forewarn },
			{ "frisk", Ability::Frisk },
			{ "gluttony", Ability::Gluttony },
			{ "guts", Ability::Guts },
			{ "heatproof", Ability::Heatproof },
			{ "honeygather", Ability::Honey_Gather },
			{ "hugepower", Ability::Huge_Power },
			{ "hustle", Ability::Hustle },
			{ "hydration", Ability::Hydration },
			{ "hypercutter", Ability::Hyper_Cutter },
			{ "icebody", Ability::Ice_Body },
			{ "illuminate", Ability::Illuminate },
			{ "immunity", Ability::Immunity },
			{ "innerfocus", Ability::Inner_Focus },
			{ "insomnia", Ability::Insomnia },
			{ "intimidate", Ability::Intimidate },
			{ "ironfist", Ability::Iron_Fist },
			{ "keeneye", Ability::Keen_Eye },
			{ "klutz", Ability::Klutz },
			{ "leafguard", Ability::Leaf_Guard },
			{ "levitate", Ability::Levitate },
			{ "lightningrod", Ability::Lightningrod },
			{ "limber", Ability::Limber },
			{ "liquidooze", Ability::Liquid_Ooze },
			{ "magicguard", Ability::Magic_Guard },
			{ "magmaarmor", Ability::Magma_Armor },
			{ "magnetpull", Ability::Magnet_Pull },
			{ "marvelscale", Ability::Marvel_Scale },
			{ "minus", Ability::Minus },
			{ "moldbreaker", Ability::Mold_Breaker },
			{ "motordrive", Ability::Motor_Drive },
			{ "multitype", Ability::Multitype },
			{ "naturalcure", Ability::Natural_Cure },
			{ "noguard", Ability::No_Guard },
			{ "normalize", Ability::Normalize },
			{ "oblivious", Ability::Oblivious },
			{ "overgrow", Ability::Overgrow },
			{ "owntempo", Ability::Own_Tempo },
			{ "pickup", Ability::Pickup },
			{ "plus", Ability::Plus },
			{ "poisonheal", Ability::Poison_Heal },
			{ "poisonpoint", Ability::Poison_Point },
			{ "pressure", Ability::Pressure },
			{ "purepower", Ability::Pure_Power },
			{ "quickfeet", Ability::Quick_Feet },
			{ "raindish", Ability::Rain_Dish },
			{ "reckless", Ability::Reckless },
			{ "rivalry", Ability::Rivalry },
			{ "rockhead", Ability::Rock_Head },
			{ "roughskin", Ability::Rough_Skin },
			{ "runaway", Ability::Run_Away },
			{ "sandstream", Ability::Sand_Stream },
			{ "sandveil", Ability::Sand_Veil },
			{ "scrappy", Ability::Scrappy },
			{ "serenegrace", Ability::Serene_Grace },
			{ "shadowtag", Ability::Shadow_Tag },
			{ "shedskin", Ability::Shed_Skin },
			{ "shellarmor", Ability::Shell_Armor },
			{ "shielddust", Ability::Shield_Dust },
			{ "simple", Ability::Simple },
			{ "skilllink", Ability::Skill_Link },
			{ "slowstart", Ability::Slow_Start },
			{ "sniper", Ability::Sniper },
			{ "snowcloak", Ability::Snow_Cloak },
			{ "snowwarning", Ability::Snow_Warning },
			{ "solarpower", Ability::Solar_Power },
			{ "solidrock", Ability::Solid_Rock },
			{ "soundproof", Ability::Soundproof },
			{ "speedboost", Ability::Speed_Boost },
			{ "stall", Ability::Stall },
			{ "static", Ability::Static },
			{ "steadfast", Ability::Steadfast },
			{ "stench", Ability::Stench },
			{ "stickyhold", Ability::Sticky_Hold },
			{ "stormdrain", Ability::Storm_Drain },
			{ "sturdy", Ability::Sturdy },
			{ "suctioncups", Ability::Suction_Cups },
			{ "superluck", Ability::Super_Luck },
			{ "swarm", Ability::Swarm },
			{ "swiftswim", Ability::Swift_Swim },
			{ "synchronize", Ability::Synchronize },
			{ "tangledfeet", Ability::Tangled_Feet },
			{ "technician", Ability::Technician },
			{ "thickfat", Ability::Thick_Fat },
			{ "tintedlens", Ability::Tinted_Lens },
			{ "torrent", Ability::Torrent },
			{ "trace", Ability::Trace },
			{ "truant", Ability::Truant },
			{ "unaware", Ability::Unaware },
			{ "unburden", Ability::Unburden },
			{ "vitalspirit", Ability::Vital_Spirit },
			{ "voltabsorb", Ability::Volt_Absorb },
			{ "waterabsorb", Ability::Water_Absorb },
			{ "waterveil", Ability::Water_Veil },
			{ "whitesmoke", Ability::White_Smoke },
			{ "wonderguard", Ability::Wonder_Guard },
		}}
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<12>(str);
	auto const it = converter.find(converted);
	if (it != end(converter)) {
		return it->mapped();
	} else {
		throw InvalidFromStringConversion("Ability", str);
	}
}

}	// namespace technicalmachine
