// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.ability;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.ability;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace containers::string_literals;

export constexpr auto to_string(Ability const ability) -> containers::string_view {
	switch (ability) {
		case Ability::Stench: return "Stench"_s;
		case Ability::Drizzle: return "Drizzle"_s;
		case Ability::Speed_Boost: return "Speed Boost"_s;
		case Ability::Battle_Armor: return "Battle Armor"_s;
		case Ability::Sturdy: return "Sturdy"_s;
		case Ability::Damp: return "Damp"_s;
		case Ability::Limber: return "Limber"_s;
		case Ability::Sand_Veil: return "Sand Veil"_s;
		case Ability::Static: return "Static"_s;
		case Ability::Volt_Absorb: return "Volt Absorb"_s;
		case Ability::Water_Absorb: return "Water Absorb"_s;
		case Ability::Oblivious: return "Oblivious"_s;
		case Ability::Cloud_Nine: return "Cloud Nine"_s;
		case Ability::Compound_Eyes: return "Compound Eyes"_s;
		case Ability::Insomnia: return "Insomnia"_s;
		case Ability::Color_Change: return "Color Change"_s;
		case Ability::Immunity: return "Immunity"_s;
		case Ability::Flash_Fire: return "Flash Fire"_s;
		case Ability::Shield_Dust: return "Shield Dust"_s;
		case Ability::Own_Tempo: return "Own Tempo"_s;
		case Ability::Suction_Cups: return "Suction Cups"_s;
		case Ability::Intimidate: return "Intimidate"_s;
		case Ability::Shadow_Tag: return "Shadow Tag"_s;
		case Ability::Rough_Skin: return "Rough Skin"_s;
		case Ability::Wonder_Guard: return "Wonder Guard"_s;
		case Ability::Levitate: return "Levitate"_s;
		case Ability::Effect_Spore: return "Effect Spore"_s;
		case Ability::Synchronize: return "Synchronize"_s;
		case Ability::Clear_Body: return "Clear Body"_s;
		case Ability::Natural_Cure: return "Natural Cure"_s;
		case Ability::Lightning_Rod: return "Lightning Rod"_s;
		case Ability::Serene_Grace: return "Serene Grace"_s;
		case Ability::Swift_Swim: return "Swift Swim"_s;
		case Ability::Chlorophyll: return "Chlorophyll"_s;
		case Ability::Illuminate: return "Illuminate"_s;
		case Ability::Trace: return "Trace"_s;
		case Ability::Huge_Power: return "Huge Power"_s;
		case Ability::Poison_Point: return "Poison Point"_s;
		case Ability::Inner_Focus: return "Inner Focus"_s;
		case Ability::Magma_Armor: return "Magma Armor"_s;
		case Ability::Water_Veil: return "Water Veil"_s;
		case Ability::Magnet_Pull: return "Magnet Pull"_s;
		case Ability::Soundproof: return "Soundproof"_s;
		case Ability::Rain_Dish: return "Rain Dish"_s;
		case Ability::Sand_Stream: return "Sand Stream"_s;
		case Ability::Pressure: return "Pressure"_s;
		case Ability::Thick_Fat: return "Thick Fat"_s;
		case Ability::Early_Bird: return "Early Bird"_s;
		case Ability::Flame_Body: return "Flame Body"_s;
		case Ability::Run_Away: return "Run Away"_s;
		case Ability::Keen_Eye: return "Keen Eye"_s;
		case Ability::Hyper_Cutter: return "Hyper Cutter"_s;
		case Ability::Pickup: return "Pickup"_s;
		case Ability::Truant: return "Truant"_s;
		case Ability::Hustle: return "Hustle"_s;
		case Ability::Cute_Charm: return "Cute Charm"_s;
		case Ability::Plus: return "Plus"_s;
		case Ability::Minus: return "Minus"_s;
		case Ability::Forecast: return "Forecast"_s;
		case Ability::Sticky_Hold: return "Sticky Hold"_s;
		case Ability::Shed_Skin: return "Shed Skin"_s;
		case Ability::Guts: return "Guts"_s;
		case Ability::Marvel_Scale: return "Marvel Scale"_s;
		case Ability::Liquid_Ooze: return "Liquid Ooze"_s;
		case Ability::Overgrow: return "Overgrow"_s;
		case Ability::Blaze: return "Blaze"_s;
		case Ability::Torrent: return "Torrent"_s;
		case Ability::Swarm: return "Swarm"_s;
		case Ability::Rock_Head: return "Rock Head"_s;
		case Ability::Drought: return "Drought"_s;
		case Ability::Arena_Trap: return "Arena Trap"_s;
		case Ability::Vital_Spirit: return "Vital Spirit"_s;
		case Ability::White_Smoke: return "White Smoke"_s;
		case Ability::Pure_Power: return "Pure Power"_s;
		case Ability::Shell_Armor: return "Shell Armor"_s;
		case Ability::Air_Lock: return "Air Lock"_s;

		// Generation 4
		case Ability::Tangled_Feet: return "Tangled Feet"_s;
		case Ability::Motor_Drive: return "Motor Drive"_s;
		case Ability::Rivalry: return "Rivalry"_s;
		case Ability::Steadfast: return "Steadfast"_s;
		case Ability::Snow_Cloak: return "Snow Cloak"_s;
		case Ability::Gluttony: return "Gluttony"_s;
		case Ability::Anger_Point: return "Anger Point"_s;
		case Ability::Unburden: return "Unburden"_s;
		case Ability::Heatproof: return "Heatproof"_s;
		case Ability::Simple: return "Simple"_s;
		case Ability::Dry_Skin: return "Dry Skin"_s;
		case Ability::Download: return "Download"_s;
		case Ability::Iron_Fist: return "Iron Fist"_s;
		case Ability::Poison_Heal: return "Poison Heal"_s;
		case Ability::Adaptability: return "Adaptability"_s;
		case Ability::Skill_Link: return "Skill Link"_s;
		case Ability::Hydration: return "Hydration"_s;
		case Ability::Solar_Power: return "Solar Power"_s;
		case Ability::Quick_Feet: return "Quick Feet"_s;
		case Ability::Normalize: return "Normalize"_s;
		case Ability::Sniper: return "Sniper"_s;
		case Ability::Magic_Guard: return "Magic Guard"_s;
		case Ability::No_Guard: return "No Guard"_s;
		case Ability::Stall: return "Stall"_s;
		case Ability::Technician: return "Technician"_s;
		case Ability::Leaf_Guard: return "Leaf Guard"_s;
		case Ability::Klutz: return "Klutz"_s;
		case Ability::Mold_Breaker: return "Mold Breaker"_s;
		case Ability::Super_Luck: return "Super Luck"_s;
		case Ability::Aftermath: return "Aftermath"_s;
		case Ability::Anticipation: return "Anticipation"_s;
		case Ability::Forewarn: return "Forewarn"_s;
		case Ability::Unaware: return "Unaware"_s;
		case Ability::Tinted_Lens: return "Tinted Lens"_s;
		case Ability::Filter: return "Filter"_s;
		case Ability::Slow_Start: return "Slow Start"_s;
		case Ability::Scrappy: return "Scrappy"_s;
		case Ability::Storm_Drain: return "Storm Drain"_s;
		case Ability::Ice_Body: return "Ice Body"_s;
		case Ability::Solid_Rock: return "Solid Rock"_s;
		case Ability::Snow_Warning: return "Snow Warning"_s;
		case Ability::Honey_Gather: return "Honey Gather"_s;
		case Ability::Frisk: return "Frisk"_s;
		case Ability::Reckless: return "Reckless"_s;
		case Ability::Multitype: return "Multitype"_s;
		case Ability::Flower_Gift: return "Flower Gift"_s;
		case Ability::Bad_Dreams: return "Bad Dreams"_s;

		// Generation 5
		case Ability::Pickpocket: return "Pickpocket"_s;
		case Ability::Sheer_Force: return "Sheer Force"_s;
		case Ability::Contrary: return "Contrary"_s;
		case Ability::Unnerve: return "Unnerve"_s;
		case Ability::Defiant: return "Defiant"_s;
		case Ability::Defeatist: return "Defeatist"_s;
		case Ability::Cursed_Body: return "Cursed Body"_s;
		case Ability::Healer: return "Healer"_s;
		case Ability::Friend_Guard: return "Friend Guard"_s;
		case Ability::Weak_Armor: return "Weak Armor"_s;
		case Ability::Heavy_Metal: return "Heavy Metal"_s;
		case Ability::Light_Metal: return "Light Metal"_s;
		case Ability::Multiscale: return "Multiscale"_s;
		case Ability::Toxic_Boost: return "Toxic Boost"_s;
		case Ability::Flare_Boost: return "Flare Boost"_s;
		case Ability::Harvest: return "Harvest"_s;
		case Ability::Telepathy: return "Telepathy"_s;
		case Ability::Moody: return "Moody"_s;
		case Ability::Overcoat: return "Overcoat"_s;
		case Ability::Poison_Touch: return "Poison Touch"_s;
		case Ability::Regenerator: return "Regenerator"_s;
		case Ability::Big_Pecks: return "Big Pecks"_s;
		case Ability::Sand_Rush: return "Sand Rush"_s;
		case Ability::Wonder_Skin: return "Wonder Skin"_s;
		case Ability::Analytic: return "Analytic"_s;
		case Ability::Illusion: return "Illusion"_s;
		case Ability::Imposter: return "Imposter"_s;
		case Ability::Infiltrator: return "Infiltrator"_s;
		case Ability::Mummy: return "Mummy"_s;
		case Ability::Moxie: return "Moxie"_s;
		case Ability::Justified: return "Justified"_s;
		case Ability::Rattled: return "Rattled"_s;
		case Ability::Magic_Bounce: return "Magic Bounce"_s;
		case Ability::Sap_Sipper: return "Sap Sipper"_s;
		case Ability::Prankster: return "Prankster"_s;
		case Ability::Sand_Force: return "Sand Force"_s;
		case Ability::Iron_Barbs: return "Iron Barbs"_s;
		case Ability::Zen_Mode: return "Zen Mode"_s;
		case Ability::Victory_Star: return "Victory Star"_s;
		case Ability::Turboblaze: return "Turboblaze"_s;
		case Ability::Teravolt: return "Teravolt"_s;

		// Generation 6
		case Ability::Aroma_Veil: return "Aroma Veil"_s;
		case Ability::Flower_Veil: return "Flower Veil"_s;
		case Ability::Cheek_Pouch: return "Cheek Pouch"_s;
		case Ability::Protean: return "Protean"_s;
		case Ability::Fur_Coat: return "Fur Coat"_s;
		case Ability::Magician: return "Magician"_s;
		case Ability::Bulletproof: return "Bulletproof"_s;
		case Ability::Competitive: return "Competitive"_s;
		case Ability::Strong_Jaw: return "Strong Jaw"_s;
		case Ability::Refrigerate: return "Refrigerate"_s;
		case Ability::Sweet_Veil: return "Sweet Veil"_s;
		case Ability::Stance_Change: return "Stance Change"_s;
		case Ability::Gale_Wings: return "Gale Wings"_s;
		case Ability::Mega_Launcher: return "Mega Launcher"_s;
		case Ability::Grass_Pelt: return "Grass Pelt"_s;
		case Ability::Symbiosis: return "Symbiosis"_s;
		case Ability::Tough_Claws: return "Tough Claws"_s;
		case Ability::Pixilate: return "Pixilate"_s;
		case Ability::Gooey: return "Gooey"_s;
		case Ability::Aerilate: return "Aerilate"_s;
		case Ability::Parental_Bond: return "Parental Bond"_s;
		case Ability::Dark_Aura: return "Dark Aura"_s;
		case Ability::Fairy_Aura: return "Fairy Aura"_s;
		case Ability::Aura_Break: return "Aura Break"_s;
		case Ability::Primordial_Sea: return "Primordial Sea"_s;
		case Ability::Desolate_Land: return "Desolate Land"_s;
		case Ability::Delta_Stream: return "Delta Stream"_s;

		// Generation 7
		case Ability::Stamina: return "Stamina"_s;
		case Ability::Wimp_Out: return "Wimp Out"_s;
		case Ability::Emergency_Exit: return "Emergency Exit"_s;
		case Ability::Water_Compaction: return "Water Compaction"_s;
		case Ability::Merciless: return "Merciless"_s;
		case Ability::Shields_Down: return "Shields Down"_s;
		case Ability::Stakeout: return "Stakeout"_s;
		case Ability::Water_Bubble: return "Water Bubble"_s;
		case Ability::Steelworker: return "Steelworker"_s;
		case Ability::Berserk: return "Berserk"_s;
		case Ability::Slush_Rush: return "Slush Rush"_s;
		case Ability::Long_Reach: return "Long Reach"_s;
		case Ability::Liquid_Voice: return "Liquid Voice"_s;
		case Ability::Triage: return "Triage"_s;
		case Ability::Galvanize: return "Galvanize"_s;
		case Ability::Surge_Surfer: return "Surge Surfer"_s;
		case Ability::Schooling: return "Schooling"_s;
		case Ability::Disguise: return "Disguise"_s;
		case Ability::Battle_Bond: return "Battle Bond"_s;
		case Ability::Power_Construct: return "Power Construct"_s;
		case Ability::Corrosion: return "Corrosion"_s;
		case Ability::Comatose: return "Comatose"_s;
		case Ability::Queenly_Majesty: return "Queenly Majesty"_s;
		case Ability::Innards_Out: return "Innards Out"_s;
		case Ability::Dancer: return "Dancer"_s;
		case Ability::Battery: return "Battery"_s;
		case Ability::Fluffy: return "Fluffy"_s;
		case Ability::Dazzling: return "Dazzling"_s;
		case Ability::Soul_Heart: return "Soul-Heart"_s;
		case Ability::Tangling_Hair: return "Tangling Hair"_s;
		case Ability::Receiver: return "Receiver"_s;
		case Ability::Power_of_Alchemy: return "Power of Alchemy"_s;
		case Ability::Beast_Boost: return "Beast Boost"_s;
		case Ability::RKS_System: return "RKS System"_s;
		case Ability::Electric_Surge: return "Electric Surge"_s;
		case Ability::Psychic_Surge: return "Psychic Surge"_s;
		case Ability::Misty_Surge: return "Misty Surge"_s;
		case Ability::Grassy_Surge: return "Grassy Surge"_s;
		case Ability::Full_Metal_Body: return "Full Metal Body"_s;
		case Ability::Shadow_Shield: return "Shadow Shield"_s;
		case Ability::Prism_Armor: return "Prism Armor"_s;
		case Ability::Neuroforce: return "Neuroforce"_s;

		// Generation 8
		case Ability::Intrepid_Sword: return "Intrepid Sword"_s;
		case Ability::Dauntless_Shield: return "Dauntless Shield"_s;
		case Ability::Libero: return "Libero"_s;
		case Ability::Ball_Fetch: return "Ball Fetch"_s;
		case Ability::Cotton_Down: return "Cotton Down"_s;
		case Ability::Propeller_Tail: return "Propeller Tail"_s;
		case Ability::Mirror_Armor: return "Mirror Armor"_s;
		case Ability::Gulp_Missile: return "Gulp Missile"_s;
		case Ability::Stalwart: return "Stalwart"_s;
		case Ability::Steam_Engine: return "Steam Engine"_s;
		case Ability::Punk_Rock: return "Punk Rock"_s;
		case Ability::Sand_Spit: return "Sand Spit"_s;
		case Ability::Ice_Scales: return "Ice Scales"_s;
		case Ability::Ripen: return "Ripen"_s;
		case Ability::Ice_Face: return "Ice Face"_s;
		case Ability::Power_Spot: return "Power Spot"_s;
		case Ability::Mimicry: return "Mimicry"_s;
		case Ability::Screen_Cleaner: return "Screen Cleaner"_s;
		case Ability::Steely_Spirit: return "Steely Spirit"_s;
		case Ability::Perish_Body: return "Perish Body"_s;
		case Ability::Wandering_Spirit: return "Wandering Spirit"_s;
		case Ability::Gorilla_Tactics: return "Gorilla Tactics"_s;
		case Ability::Neutralizing_Gas: return "Neutralizing Gas"_s;
		case Ability::Pastel_Veil: return "Pastel Veil"_s;
		case Ability::Hunger_Switch: return "Hunger Switch"_s;
	}
}

export template<>
constexpr auto from_string<Ability>(containers::string_view const str) -> Ability {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Ability>>({
			{"adaptability"_s, Ability::Adaptability},
			{"aerilate"_s, Ability::Aerilate},
			{"aftermath"_s, Ability::Aftermath},
			{"airlock"_s, Ability::Air_Lock},
			{"analytic"_s, Ability::Analytic},
			{"angerpoint"_s, Ability::Anger_Point},
			{"anticipation"_s, Ability::Anticipation},
			{"arenatrap"_s, Ability::Arena_Trap},
			{"aromaveil"_s, Ability::Aroma_Veil},
			{"aurabreak"_s, Ability::Aura_Break},
			{"baddreams"_s, Ability::Bad_Dreams},
			{"ballfetch"_s, Ability::Ball_Fetch},
			{"battery"_s, Ability::Battery},
			{"battlearmor"_s, Ability::Battle_Armor},
			{"battlebond"_s, Ability::Battle_Bond},
			{"beastboost"_s, Ability::Beast_Boost},
			{"berserk"_s, Ability::Berserk},
			{"bigpecks"_s, Ability::Big_Pecks},
			{"blaze"_s, Ability::Blaze},
			{"bulletproof"_s, Ability::Bulletproof},
			{"cheekpouch"_s, Ability::Cheek_Pouch},
			{"chlorophyll"_s, Ability::Chlorophyll},
			{"clearbody"_s, Ability::Clear_Body},
			{"cloudnine"_s, Ability::Cloud_Nine},
			{"colorchange"_s, Ability::Color_Change},
			{"comatose"_s, Ability::Comatose},
			{"competitive"_s, Ability::Competitive},
			{"compoundeyes"_s, Ability::Compound_Eyes},
			{"contrary"_s, Ability::Contrary},
			{"corrosion"_s, Ability::Corrosion},
			{"cottondown"_s, Ability::Cotton_Down},
			{"cursedbody"_s, Ability::Cursed_Body},
			{"cutecharm"_s, Ability::Cute_Charm},
			{"damp"_s, Ability::Damp},
			{"dancer"_s, Ability::Dancer},
			{"darkaura"_s, Ability::Dark_Aura},
			{"dauntlessshield"_s, Ability::Dauntless_Shield},
			{"dazzling"_s, Ability::Dazzling},
			{"defeatist"_s, Ability::Defeatist},
			{"defiant"_s, Ability::Defiant},
			{"deltastream"_s, Ability::Delta_Stream},
			{"desolateland"_s, Ability::Desolate_Land},
			{"disguise"_s, Ability::Disguise},
			{"download"_s, Ability::Download},
			{"drizzle"_s, Ability::Drizzle},
			{"drought"_s, Ability::Drought},
			{"dryskin"_s, Ability::Dry_Skin},
			{"earlybird"_s, Ability::Early_Bird},
			{"effectspore"_s, Ability::Effect_Spore},
			{"electricsurge"_s, Ability::Electric_Surge},
			{"emergencyexit"_s, Ability::Emergency_Exit},
			{"fairyaura"_s, Ability::Fairy_Aura},
			{"filter"_s, Ability::Filter},
			{"flamebody"_s, Ability::Flame_Body},
			{"flareboost"_s, Ability::Flare_Boost},
			{"flashfire"_s, Ability::Flash_Fire},
			{"flowergift"_s, Ability::Flower_Gift},
			{"flowerveil"_s, Ability::Flower_Veil},
			{"fluffy"_s, Ability::Fluffy},
			{"forecast"_s, Ability::Forecast},
			{"forewarn"_s, Ability::Forewarn},
			{"friendguard"_s, Ability::Friend_Guard},
			{"frisk"_s, Ability::Frisk},
			{"fullmetalbody"_s, Ability::Full_Metal_Body},
			{"furcoat"_s, Ability::Fur_Coat},
			{"galewings"_s, Ability::Gale_Wings},
			{"galvanize"_s, Ability::Galvanize},
			{"gluttony"_s, Ability::Gluttony},
			{"gooey"_s, Ability::Gooey},
			{"gorillatactics"_s, Ability::Gorilla_Tactics},
			{"grasspelt"_s, Ability::Grass_Pelt},
			{"grassysurge"_s, Ability::Grassy_Surge},
			{"gulpmissile"_s, Ability::Gulp_Missile},
			{"guts"_s, Ability::Guts},
			{"harvest"_s, Ability::Harvest},
			{"healer"_s, Ability::Healer},
			{"heatproof"_s, Ability::Heatproof},
			{"heavymetal"_s, Ability::Heavy_Metal},
			{"honeygather"_s, Ability::Honey_Gather},
			{"hugepower"_s, Ability::Huge_Power},
			{"hungerswitch"_s, Ability::Hunger_Switch},
			{"hustle"_s, Ability::Hustle},
			{"hydration"_s, Ability::Hydration},
			{"hypercutter"_s, Ability::Hyper_Cutter},
			{"icebody"_s, Ability::Ice_Body},
			{"iceface"_s, Ability::Ice_Face},
			{"icescales"_s, Ability::Ice_Scales},
			{"illuminate"_s, Ability::Illuminate},
			{"illusion"_s, Ability::Illusion},
			{"immunity"_s, Ability::Immunity},
			{"imposter"_s, Ability::Imposter},
			{"infiltrator"_s, Ability::Infiltrator},
			{"innardsout"_s, Ability::Innards_Out},
			{"innerfocus"_s, Ability::Inner_Focus},
			{"insomnia"_s, Ability::Insomnia},
			{"intimidate"_s, Ability::Intimidate},
			{"intrepidsword"_s, Ability::Intrepid_Sword},
			{"ironbarbs"_s, Ability::Iron_Barbs},
			{"ironfist"_s, Ability::Iron_Fist},
			{"justified"_s, Ability::Justified},
			{"keeneye"_s, Ability::Keen_Eye},
			{"klutz"_s, Ability::Klutz},
			{"leafguard"_s, Ability::Leaf_Guard},
			{"levitate"_s, Ability::Levitate},
			{"libero"_s, Ability::Libero},
			{"lightmetal"_s, Ability::Light_Metal},
			{"lightningrod"_s, Ability::Lightning_Rod},
			{"limber"_s, Ability::Limber},
			{"liquidooze"_s, Ability::Liquid_Ooze},
			{"liquidvoice"_s, Ability::Liquid_Voice},
			{"longreach"_s, Ability::Long_Reach},
			{"magicbounce"_s, Ability::Magic_Bounce},
			{"magicguard"_s, Ability::Magic_Guard},
			{"magician"_s, Ability::Magician},
			{"magmaarmor"_s, Ability::Magma_Armor},
			{"magnetpull"_s, Ability::Magnet_Pull},
			{"marvelscale"_s, Ability::Marvel_Scale},
			{"megalauncher"_s, Ability::Mega_Launcher},
			{"merciless"_s, Ability::Merciless},
			{"mimicry"_s, Ability::Mimicry},
			{"minus"_s, Ability::Minus},
			{"mirrorarmor"_s, Ability::Mirror_Armor},
			{"mistysurge"_s, Ability::Misty_Surge},
			{"moldbreaker"_s, Ability::Mold_Breaker},
			{"moody"_s, Ability::Moody},
			{"motordrive"_s, Ability::Motor_Drive},
			{"moxie"_s, Ability::Moxie},
			{"multiscale"_s, Ability::Multiscale},
			{"multitype"_s, Ability::Multitype},
			{"mummy"_s, Ability::Mummy},
			{"naturalcure"_s, Ability::Natural_Cure},
			{"neuroforce"_s, Ability::Neuroforce},
			{"neutralizinggas"_s, Ability::Neutralizing_Gas},
			{"noability"_s, Ability::Honey_Gather}, // TODO: ???
			{"noguard"_s, Ability::No_Guard},
			{"none"_s, Ability::Honey_Gather}, // TODO: ???
			{"normalize"_s, Ability::Normalize},
			{"oblivious"_s, Ability::Oblivious},
			{"overcoat"_s, Ability::Overcoat},
			{"overgrow"_s, Ability::Overgrow},
			{"owntempo"_s, Ability::Own_Tempo},
			{"parentalbond"_s, Ability::Parental_Bond},
			{"pastelveil"_s, Ability::Pastel_Veil},
			{"perishbody"_s, Ability::Perish_Body},
			{"pickpocket"_s, Ability::Pickpocket},
			{"pickup"_s, Ability::Pickup},
			{"pixilate"_s, Ability::Pixilate},
			{"plus"_s, Ability::Plus},
			{"poisonheal"_s, Ability::Poison_Heal},
			{"poisonpoint"_s, Ability::Poison_Point},
			{"poisontouch"_s, Ability::Poison_Touch},
			{"powerconstruct"_s, Ability::Power_Construct},
			{"powerofalchemy"_s, Ability::Power_of_Alchemy},
			{"powerspot"_s, Ability::Power_Spot},
			{"prankster"_s, Ability::Prankster},
			{"pressure"_s, Ability::Pressure},
			{"primordialsea"_s, Ability::Primordial_Sea},
			{"prismarmor"_s, Ability::Prism_Armor},
			{"propellertail"_s, Ability::Propeller_Tail},
			{"protean"_s, Ability::Protean},
			{"psychicsurge"_s, Ability::Psychic_Surge},
			{"punkrock"_s, Ability::Punk_Rock},
			{"purepower"_s, Ability::Pure_Power},
			{"queenlymajesty"_s, Ability::Queenly_Majesty},
			{"quickfeet"_s, Ability::Quick_Feet},
			{"raindish"_s, Ability::Rain_Dish},
			{"rattled"_s, Ability::Rattled},
			{"receiver"_s, Ability::Receiver},
			{"reckless"_s, Ability::Reckless},
			{"refrigerate"_s, Ability::Refrigerate},
			{"regenerator"_s, Ability::Regenerator},
			{"ripen"_s, Ability::Ripen},
			{"rivalry"_s, Ability::Rivalry},
			{"rkssystem"_s, Ability::RKS_System},
			{"rockhead"_s, Ability::Rock_Head},
			{"roughskin"_s, Ability::Rough_Skin},
			{"runaway"_s, Ability::Run_Away},
			{"sandforce"_s, Ability::Sand_Force},
			{"sandrush"_s, Ability::Sand_Rush},
			{"sandspit"_s, Ability::Sand_Spit},
			{"sandstream"_s, Ability::Sand_Stream},
			{"sandveil"_s, Ability::Sand_Veil},
			{"sapsipper"_s, Ability::Sap_Sipper},
			{"schooling"_s, Ability::Schooling},
			{"scrappy"_s, Ability::Scrappy},
			{"screencleaner"_s, Ability::Screen_Cleaner},
			{"serenegrace"_s, Ability::Serene_Grace},
			{"shadowshield"_s, Ability::Shadow_Shield},
			{"shadowtag"_s, Ability::Shadow_Tag},
			{"shedskin"_s, Ability::Shed_Skin},
			{"sheerforce"_s, Ability::Sheer_Force},
			{"shellarmor"_s, Ability::Shell_Armor},
			{"shielddust"_s, Ability::Shield_Dust},
			{"shieldsdown"_s, Ability::Shields_Down},
			{"simple"_s, Ability::Simple},
			{"skilllink"_s, Ability::Skill_Link},
			{"slowstart"_s, Ability::Slow_Start},
			{"slushrush"_s, Ability::Slush_Rush},
			{"sniper"_s, Ability::Sniper},
			{"snowcloak"_s, Ability::Snow_Cloak},
			{"snowwarning"_s, Ability::Snow_Warning},
			{"solarpower"_s, Ability::Solar_Power},
			{"solidrock"_s, Ability::Solid_Rock},
			{"soulheart"_s, Ability::Soul_Heart},
			{"soundproof"_s, Ability::Soundproof},
			{"speedboost"_s, Ability::Speed_Boost},
			{"stakeout"_s, Ability::Stakeout},
			{"stall"_s, Ability::Stall},
			{"stalwart"_s, Ability::Stalwart},
			{"stamina"_s, Ability::Stamina},
			{"stancechange"_s, Ability::Stance_Change},
			{"static"_s, Ability::Static},
			{"steadfast"_s, Ability::Steadfast},
			{"steamengine"_s, Ability::Steam_Engine},
			{"steelworker"_s, Ability::Steelworker},
			{"steelyspirit"_s, Ability::Steely_Spirit},
			{"stench"_s, Ability::Stench},
			{"stickyhold"_s, Ability::Sticky_Hold},
			{"stormdrain"_s, Ability::Storm_Drain},
			{"strongjaw"_s, Ability::Strong_Jaw},
			{"sturdy"_s, Ability::Sturdy},
			{"suctioncups"_s, Ability::Suction_Cups},
			{"superluck"_s, Ability::Super_Luck},
			{"surgesurfer"_s, Ability::Surge_Surfer},
			{"swarm"_s, Ability::Swarm},
			{"sweetveil"_s, Ability::Sweet_Veil},
			{"swiftswim"_s, Ability::Swift_Swim},
			{"symbiosis"_s, Ability::Symbiosis},
			{"synchronize"_s, Ability::Synchronize},
			{"tangledfeet"_s, Ability::Tangled_Feet},
			{"tanglinghair"_s, Ability::Tangling_Hair},
			{"technician"_s, Ability::Technician},
			{"telepathy"_s, Ability::Telepathy},
			{"teravolt"_s, Ability::Teravolt},
			{"thickfat"_s, Ability::Thick_Fat},
			{"tintedlens"_s, Ability::Tinted_Lens},
			{"torrent"_s, Ability::Torrent},
			{"toughclaws"_s, Ability::Tough_Claws},
			{"toxicboost"_s, Ability::Toxic_Boost},
			{"trace"_s, Ability::Trace},
			{"triage"_s, Ability::Triage},
			{"truant"_s, Ability::Truant},
			{"turboblaze"_s, Ability::Turboblaze},
			{"unaware"_s, Ability::Unaware},
			{"unburden"_s, Ability::Unburden},
			{"unnerve"_s, Ability::Unnerve},
			{"victorystar"_s, Ability::Victory_Star},
			{"vitalspirit"_s, Ability::Vital_Spirit},
			{"voltabsorb"_s, Ability::Volt_Absorb},
			{"wanderingspirit"_s, Ability::Wandering_Spirit},
			{"waterabsorb"_s, Ability::Water_Absorb},
			{"waterbubble"_s, Ability::Water_Bubble},
			{"watercompaction"_s, Ability::Water_Compaction},
			{"waterveil"_s, Ability::Water_Veil},
			{"weakarmor"_s, Ability::Weak_Armor},
			{"whitesmoke"_s, Ability::White_Smoke},
			{"wimpout"_s, Ability::Wimp_Out},
			{"wonderguard"_s, Ability::Wonder_Guard},
			{"wonderskin"_s, Ability::Wonder_Skin},
			{"zenmode"_s, Ability::Zen_Mode},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<15_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Ability"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
