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
		case Ability::Stench: return "Stench"_sv;
		case Ability::Drizzle: return "Drizzle"_sv;
		case Ability::Speed_Boost: return "Speed Boost"_sv;
		case Ability::Battle_Armor: return "Battle Armor"_sv;
		case Ability::Sturdy: return "Sturdy"_sv;
		case Ability::Damp: return "Damp"_sv;
		case Ability::Limber: return "Limber"_sv;
		case Ability::Sand_Veil: return "Sand Veil"_sv;
		case Ability::Static: return "Static"_sv;
		case Ability::Volt_Absorb: return "Volt Absorb"_sv;
		case Ability::Water_Absorb: return "Water Absorb"_sv;
		case Ability::Oblivious: return "Oblivious"_sv;
		case Ability::Cloud_Nine: return "Cloud Nine"_sv;
		case Ability::Compound_Eyes: return "Compound Eyes"_sv;
		case Ability::Insomnia: return "Insomnia"_sv;
		case Ability::Color_Change: return "Color Change"_sv;
		case Ability::Immunity: return "Immunity"_sv;
		case Ability::Flash_Fire: return "Flash Fire"_sv;
		case Ability::Shield_Dust: return "Shield Dust"_sv;
		case Ability::Own_Tempo: return "Own Tempo"_sv;
		case Ability::Suction_Cups: return "Suction Cups"_sv;
		case Ability::Intimidate: return "Intimidate"_sv;
		case Ability::Shadow_Tag: return "Shadow Tag"_sv;
		case Ability::Rough_Skin: return "Rough Skin"_sv;
		case Ability::Wonder_Guard: return "Wonder Guard"_sv;
		case Ability::Levitate: return "Levitate"_sv;
		case Ability::Effect_Spore: return "Effect Spore"_sv;
		case Ability::Synchronize: return "Synchronize"_sv;
		case Ability::Clear_Body: return "Clear Body"_sv;
		case Ability::Natural_Cure: return "Natural Cure"_sv;
		case Ability::Lightning_Rod: return "Lightning Rod"_sv;
		case Ability::Serene_Grace: return "Serene Grace"_sv;
		case Ability::Swift_Swim: return "Swift Swim"_sv;
		case Ability::Chlorophyll: return "Chlorophyll"_sv;
		case Ability::Illuminate: return "Illuminate"_sv;
		case Ability::Trace: return "Trace"_sv;
		case Ability::Huge_Power: return "Huge Power"_sv;
		case Ability::Poison_Point: return "Poison Point"_sv;
		case Ability::Inner_Focus: return "Inner Focus"_sv;
		case Ability::Magma_Armor: return "Magma Armor"_sv;
		case Ability::Water_Veil: return "Water Veil"_sv;
		case Ability::Magnet_Pull: return "Magnet Pull"_sv;
		case Ability::Soundproof: return "Soundproof"_sv;
		case Ability::Rain_Dish: return "Rain Dish"_sv;
		case Ability::Sand_Stream: return "Sand Stream"_sv;
		case Ability::Pressure: return "Pressure"_sv;
		case Ability::Thick_Fat: return "Thick Fat"_sv;
		case Ability::Early_Bird: return "Early Bird"_sv;
		case Ability::Flame_Body: return "Flame Body"_sv;
		case Ability::Run_Away: return "Run Away"_sv;
		case Ability::Keen_Eye: return "Keen Eye"_sv;
		case Ability::Hyper_Cutter: return "Hyper Cutter"_sv;
		case Ability::Pickup: return "Pickup"_sv;
		case Ability::Truant: return "Truant"_sv;
		case Ability::Hustle: return "Hustle"_sv;
		case Ability::Cute_Charm: return "Cute Charm"_sv;
		case Ability::Plus: return "Plus"_sv;
		case Ability::Minus: return "Minus"_sv;
		case Ability::Forecast: return "Forecast"_sv;
		case Ability::Sticky_Hold: return "Sticky Hold"_sv;
		case Ability::Shed_Skin: return "Shed Skin"_sv;
		case Ability::Guts: return "Guts"_sv;
		case Ability::Marvel_Scale: return "Marvel Scale"_sv;
		case Ability::Liquid_Ooze: return "Liquid Ooze"_sv;
		case Ability::Overgrow: return "Overgrow"_sv;
		case Ability::Blaze: return "Blaze"_sv;
		case Ability::Torrent: return "Torrent"_sv;
		case Ability::Swarm: return "Swarm"_sv;
		case Ability::Rock_Head: return "Rock Head"_sv;
		case Ability::Drought: return "Drought"_sv;
		case Ability::Arena_Trap: return "Arena Trap"_sv;
		case Ability::Vital_Spirit: return "Vital Spirit"_sv;
		case Ability::White_Smoke: return "White Smoke"_sv;
		case Ability::Pure_Power: return "Pure Power"_sv;
		case Ability::Shell_Armor: return "Shell Armor"_sv;
		case Ability::Air_Lock: return "Air Lock"_sv;

		// Generation 4
		case Ability::Tangled_Feet: return "Tangled Feet"_sv;
		case Ability::Motor_Drive: return "Motor Drive"_sv;
		case Ability::Rivalry: return "Rivalry"_sv;
		case Ability::Steadfast: return "Steadfast"_sv;
		case Ability::Snow_Cloak: return "Snow Cloak"_sv;
		case Ability::Gluttony: return "Gluttony"_sv;
		case Ability::Anger_Point: return "Anger Point"_sv;
		case Ability::Unburden: return "Unburden"_sv;
		case Ability::Heatproof: return "Heatproof"_sv;
		case Ability::Simple: return "Simple"_sv;
		case Ability::Dry_Skin: return "Dry Skin"_sv;
		case Ability::Download: return "Download"_sv;
		case Ability::Iron_Fist: return "Iron Fist"_sv;
		case Ability::Poison_Heal: return "Poison Heal"_sv;
		case Ability::Adaptability: return "Adaptability"_sv;
		case Ability::Skill_Link: return "Skill Link"_sv;
		case Ability::Hydration: return "Hydration"_sv;
		case Ability::Solar_Power: return "Solar Power"_sv;
		case Ability::Quick_Feet: return "Quick Feet"_sv;
		case Ability::Normalize: return "Normalize"_sv;
		case Ability::Sniper: return "Sniper"_sv;
		case Ability::Magic_Guard: return "Magic Guard"_sv;
		case Ability::No_Guard: return "No Guard"_sv;
		case Ability::Stall: return "Stall"_sv;
		case Ability::Technician: return "Technician"_sv;
		case Ability::Leaf_Guard: return "Leaf Guard"_sv;
		case Ability::Klutz: return "Klutz"_sv;
		case Ability::Mold_Breaker: return "Mold Breaker"_sv;
		case Ability::Super_Luck: return "Super Luck"_sv;
		case Ability::Aftermath: return "Aftermath"_sv;
		case Ability::Anticipation: return "Anticipation"_sv;
		case Ability::Forewarn: return "Forewarn"_sv;
		case Ability::Unaware: return "Unaware"_sv;
		case Ability::Tinted_Lens: return "Tinted Lens"_sv;
		case Ability::Filter: return "Filter"_sv;
		case Ability::Slow_Start: return "Slow Start"_sv;
		case Ability::Scrappy: return "Scrappy"_sv;
		case Ability::Storm_Drain: return "Storm Drain"_sv;
		case Ability::Ice_Body: return "Ice Body"_sv;
		case Ability::Solid_Rock: return "Solid Rock"_sv;
		case Ability::Snow_Warning: return "Snow Warning"_sv;
		case Ability::Honey_Gather: return "Honey Gather"_sv;
		case Ability::Frisk: return "Frisk"_sv;
		case Ability::Reckless: return "Reckless"_sv;
		case Ability::Multitype: return "Multitype"_sv;
		case Ability::Flower_Gift: return "Flower Gift"_sv;
		case Ability::Bad_Dreams: return "Bad Dreams"_sv;

		// Generation 5
		case Ability::Pickpocket: return "Pickpocket"_sv;
		case Ability::Sheer_Force: return "Sheer Force"_sv;
		case Ability::Contrary: return "Contrary"_sv;
		case Ability::Unnerve: return "Unnerve"_sv;
		case Ability::Defiant: return "Defiant"_sv;
		case Ability::Defeatist: return "Defeatist"_sv;
		case Ability::Cursed_Body: return "Cursed Body"_sv;
		case Ability::Healer: return "Healer"_sv;
		case Ability::Friend_Guard: return "Friend Guard"_sv;
		case Ability::Weak_Armor: return "Weak Armor"_sv;
		case Ability::Heavy_Metal: return "Heavy Metal"_sv;
		case Ability::Light_Metal: return "Light Metal"_sv;
		case Ability::Multiscale: return "Multiscale"_sv;
		case Ability::Toxic_Boost: return "Toxic Boost"_sv;
		case Ability::Flare_Boost: return "Flare Boost"_sv;
		case Ability::Harvest: return "Harvest"_sv;
		case Ability::Telepathy: return "Telepathy"_sv;
		case Ability::Moody: return "Moody"_sv;
		case Ability::Overcoat: return "Overcoat"_sv;
		case Ability::Poison_Touch: return "Poison Touch"_sv;
		case Ability::Regenerator: return "Regenerator"_sv;
		case Ability::Big_Pecks: return "Big Pecks"_sv;
		case Ability::Sand_Rush: return "Sand Rush"_sv;
		case Ability::Wonder_Skin: return "Wonder Skin"_sv;
		case Ability::Analytic: return "Analytic"_sv;
		case Ability::Illusion: return "Illusion"_sv;
		case Ability::Imposter: return "Imposter"_sv;
		case Ability::Infiltrator: return "Infiltrator"_sv;
		case Ability::Mummy: return "Mummy"_sv;
		case Ability::Moxie: return "Moxie"_sv;
		case Ability::Justified: return "Justified"_sv;
		case Ability::Rattled: return "Rattled"_sv;
		case Ability::Magic_Bounce: return "Magic Bounce"_sv;
		case Ability::Sap_Sipper: return "Sap Sipper"_sv;
		case Ability::Prankster: return "Prankster"_sv;
		case Ability::Sand_Force: return "Sand Force"_sv;
		case Ability::Iron_Barbs: return "Iron Barbs"_sv;
		case Ability::Zen_Mode: return "Zen Mode"_sv;
		case Ability::Victory_Star: return "Victory Star"_sv;
		case Ability::Turboblaze: return "Turboblaze"_sv;
		case Ability::Teravolt: return "Teravolt"_sv;

		// Generation 6
		case Ability::Aroma_Veil: return "Aroma Veil"_sv;
		case Ability::Flower_Veil: return "Flower Veil"_sv;
		case Ability::Cheek_Pouch: return "Cheek Pouch"_sv;
		case Ability::Protean: return "Protean"_sv;
		case Ability::Fur_Coat: return "Fur Coat"_sv;
		case Ability::Magician: return "Magician"_sv;
		case Ability::Bulletproof: return "Bulletproof"_sv;
		case Ability::Competitive: return "Competitive"_sv;
		case Ability::Strong_Jaw: return "Strong Jaw"_sv;
		case Ability::Refrigerate: return "Refrigerate"_sv;
		case Ability::Sweet_Veil: return "Sweet Veil"_sv;
		case Ability::Stance_Change: return "Stance Change"_sv;
		case Ability::Gale_Wings: return "Gale Wings"_sv;
		case Ability::Mega_Launcher: return "Mega Launcher"_sv;
		case Ability::Grass_Pelt: return "Grass Pelt"_sv;
		case Ability::Symbiosis: return "Symbiosis"_sv;
		case Ability::Tough_Claws: return "Tough Claws"_sv;
		case Ability::Pixilate: return "Pixilate"_sv;
		case Ability::Gooey: return "Gooey"_sv;
		case Ability::Aerilate: return "Aerilate"_sv;
		case Ability::Parental_Bond: return "Parental Bond"_sv;
		case Ability::Dark_Aura: return "Dark Aura"_sv;
		case Ability::Fairy_Aura: return "Fairy Aura"_sv;
		case Ability::Aura_Break: return "Aura Break"_sv;
		case Ability::Primordial_Sea: return "Primordial Sea"_sv;
		case Ability::Desolate_Land: return "Desolate Land"_sv;
		case Ability::Delta_Stream: return "Delta Stream"_sv;

		// Generation 7
		case Ability::Stamina: return "Stamina"_sv;
		case Ability::Wimp_Out: return "Wimp Out"_sv;
		case Ability::Emergency_Exit: return "Emergency Exit"_sv;
		case Ability::Water_Compaction: return "Water Compaction"_sv;
		case Ability::Merciless: return "Merciless"_sv;
		case Ability::Shields_Down: return "Shields Down"_sv;
		case Ability::Stakeout: return "Stakeout"_sv;
		case Ability::Water_Bubble: return "Water Bubble"_sv;
		case Ability::Steelworker: return "Steelworker"_sv;
		case Ability::Berserk: return "Berserk"_sv;
		case Ability::Slush_Rush: return "Slush Rush"_sv;
		case Ability::Long_Reach: return "Long Reach"_sv;
		case Ability::Liquid_Voice: return "Liquid Voice"_sv;
		case Ability::Triage: return "Triage"_sv;
		case Ability::Galvanize: return "Galvanize"_sv;
		case Ability::Surge_Surfer: return "Surge Surfer"_sv;
		case Ability::Schooling: return "Schooling"_sv;
		case Ability::Disguise: return "Disguise"_sv;
		case Ability::Battle_Bond: return "Battle Bond"_sv;
		case Ability::Power_Construct: return "Power Construct"_sv;
		case Ability::Corrosion: return "Corrosion"_sv;
		case Ability::Comatose: return "Comatose"_sv;
		case Ability::Queenly_Majesty: return "Queenly Majesty"_sv;
		case Ability::Innards_Out: return "Innards Out"_sv;
		case Ability::Dancer: return "Dancer"_sv;
		case Ability::Battery: return "Battery"_sv;
		case Ability::Fluffy: return "Fluffy"_sv;
		case Ability::Dazzling: return "Dazzling"_sv;
		case Ability::Soul_Heart: return "Soul-Heart"_sv;
		case Ability::Tangling_Hair: return "Tangling Hair"_sv;
		case Ability::Receiver: return "Receiver"_sv;
		case Ability::Power_of_Alchemy: return "Power of Alchemy"_sv;
		case Ability::Beast_Boost: return "Beast Boost"_sv;
		case Ability::RKS_System: return "RKS System"_sv;
		case Ability::Electric_Surge: return "Electric Surge"_sv;
		case Ability::Psychic_Surge: return "Psychic Surge"_sv;
		case Ability::Misty_Surge: return "Misty Surge"_sv;
		case Ability::Grassy_Surge: return "Grassy Surge"_sv;
		case Ability::Full_Metal_Body: return "Full Metal Body"_sv;
		case Ability::Shadow_Shield: return "Shadow Shield"_sv;
		case Ability::Prism_Armor: return "Prism Armor"_sv;
		case Ability::Neuroforce: return "Neuroforce"_sv;

		// Generation 8
		case Ability::Intrepid_Sword: return "Intrepid Sword"_sv;
		case Ability::Dauntless_Shield: return "Dauntless Shield"_sv;
		case Ability::Libero: return "Libero"_sv;
		case Ability::Ball_Fetch: return "Ball Fetch"_sv;
		case Ability::Cotton_Down: return "Cotton Down"_sv;
		case Ability::Propeller_Tail: return "Propeller Tail"_sv;
		case Ability::Mirror_Armor: return "Mirror Armor"_sv;
		case Ability::Gulp_Missile: return "Gulp Missile"_sv;
		case Ability::Stalwart: return "Stalwart"_sv;
		case Ability::Steam_Engine: return "Steam Engine"_sv;
		case Ability::Punk_Rock: return "Punk Rock"_sv;
		case Ability::Sand_Spit: return "Sand Spit"_sv;
		case Ability::Ice_Scales: return "Ice Scales"_sv;
		case Ability::Ripen: return "Ripen"_sv;
		case Ability::Ice_Face: return "Ice Face"_sv;
		case Ability::Power_Spot: return "Power Spot"_sv;
		case Ability::Mimicry: return "Mimicry"_sv;
		case Ability::Screen_Cleaner: return "Screen Cleaner"_sv;
		case Ability::Steely_Spirit: return "Steely Spirit"_sv;
		case Ability::Perish_Body: return "Perish Body"_sv;
		case Ability::Wandering_Spirit: return "Wandering Spirit"_sv;
		case Ability::Gorilla_Tactics: return "Gorilla Tactics"_sv;
		case Ability::Neutralizing_Gas: return "Neutralizing Gas"_sv;
		case Ability::Pastel_Veil: return "Pastel Veil"_sv;
		case Ability::Hunger_Switch: return "Hunger Switch"_sv;
	}
}

export template<>
constexpr auto from_string<Ability>(containers::string_view const str) -> Ability {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Ability>>({
			{"adaptability"_sv, Ability::Adaptability},
			{"aerilate"_sv, Ability::Aerilate},
			{"aftermath"_sv, Ability::Aftermath},
			{"airlock"_sv, Ability::Air_Lock},
			{"analytic"_sv, Ability::Analytic},
			{"angerpoint"_sv, Ability::Anger_Point},
			{"anticipation"_sv, Ability::Anticipation},
			{"arenatrap"_sv, Ability::Arena_Trap},
			{"aromaveil"_sv, Ability::Aroma_Veil},
			{"aurabreak"_sv, Ability::Aura_Break},
			{"baddreams"_sv, Ability::Bad_Dreams},
			{"ballfetch"_sv, Ability::Ball_Fetch},
			{"battery"_sv, Ability::Battery},
			{"battlearmor"_sv, Ability::Battle_Armor},
			{"battlebond"_sv, Ability::Battle_Bond},
			{"beastboost"_sv, Ability::Beast_Boost},
			{"berserk"_sv, Ability::Berserk},
			{"bigpecks"_sv, Ability::Big_Pecks},
			{"blaze"_sv, Ability::Blaze},
			{"bulletproof"_sv, Ability::Bulletproof},
			{"cheekpouch"_sv, Ability::Cheek_Pouch},
			{"chlorophyll"_sv, Ability::Chlorophyll},
			{"clearbody"_sv, Ability::Clear_Body},
			{"cloudnine"_sv, Ability::Cloud_Nine},
			{"colorchange"_sv, Ability::Color_Change},
			{"comatose"_sv, Ability::Comatose},
			{"competitive"_sv, Ability::Competitive},
			{"compoundeyes"_sv, Ability::Compound_Eyes},
			{"contrary"_sv, Ability::Contrary},
			{"corrosion"_sv, Ability::Corrosion},
			{"cottondown"_sv, Ability::Cotton_Down},
			{"cursedbody"_sv, Ability::Cursed_Body},
			{"cutecharm"_sv, Ability::Cute_Charm},
			{"damp"_sv, Ability::Damp},
			{"dancer"_sv, Ability::Dancer},
			{"darkaura"_sv, Ability::Dark_Aura},
			{"dauntlessshield"_sv, Ability::Dauntless_Shield},
			{"dazzling"_sv, Ability::Dazzling},
			{"defeatist"_sv, Ability::Defeatist},
			{"defiant"_sv, Ability::Defiant},
			{"deltastream"_sv, Ability::Delta_Stream},
			{"desolateland"_sv, Ability::Desolate_Land},
			{"disguise"_sv, Ability::Disguise},
			{"download"_sv, Ability::Download},
			{"drizzle"_sv, Ability::Drizzle},
			{"drought"_sv, Ability::Drought},
			{"dryskin"_sv, Ability::Dry_Skin},
			{"earlybird"_sv, Ability::Early_Bird},
			{"effectspore"_sv, Ability::Effect_Spore},
			{"electricsurge"_sv, Ability::Electric_Surge},
			{"emergencyexit"_sv, Ability::Emergency_Exit},
			{"fairyaura"_sv, Ability::Fairy_Aura},
			{"filter"_sv, Ability::Filter},
			{"flamebody"_sv, Ability::Flame_Body},
			{"flareboost"_sv, Ability::Flare_Boost},
			{"flashfire"_sv, Ability::Flash_Fire},
			{"flowergift"_sv, Ability::Flower_Gift},
			{"flowerveil"_sv, Ability::Flower_Veil},
			{"fluffy"_sv, Ability::Fluffy},
			{"forecast"_sv, Ability::Forecast},
			{"forewarn"_sv, Ability::Forewarn},
			{"friendguard"_sv, Ability::Friend_Guard},
			{"frisk"_sv, Ability::Frisk},
			{"fullmetalbody"_sv, Ability::Full_Metal_Body},
			{"furcoat"_sv, Ability::Fur_Coat},
			{"galewings"_sv, Ability::Gale_Wings},
			{"galvanize"_sv, Ability::Galvanize},
			{"gluttony"_sv, Ability::Gluttony},
			{"gooey"_sv, Ability::Gooey},
			{"gorillatactics"_sv, Ability::Gorilla_Tactics},
			{"grasspelt"_sv, Ability::Grass_Pelt},
			{"grassysurge"_sv, Ability::Grassy_Surge},
			{"gulpmissile"_sv, Ability::Gulp_Missile},
			{"guts"_sv, Ability::Guts},
			{"harvest"_sv, Ability::Harvest},
			{"healer"_sv, Ability::Healer},
			{"heatproof"_sv, Ability::Heatproof},
			{"heavymetal"_sv, Ability::Heavy_Metal},
			{"honeygather"_sv, Ability::Honey_Gather},
			{"hugepower"_sv, Ability::Huge_Power},
			{"hungerswitch"_sv, Ability::Hunger_Switch},
			{"hustle"_sv, Ability::Hustle},
			{"hydration"_sv, Ability::Hydration},
			{"hypercutter"_sv, Ability::Hyper_Cutter},
			{"icebody"_sv, Ability::Ice_Body},
			{"iceface"_sv, Ability::Ice_Face},
			{"icescales"_sv, Ability::Ice_Scales},
			{"illuminate"_sv, Ability::Illuminate},
			{"illusion"_sv, Ability::Illusion},
			{"immunity"_sv, Ability::Immunity},
			{"imposter"_sv, Ability::Imposter},
			{"infiltrator"_sv, Ability::Infiltrator},
			{"innardsout"_sv, Ability::Innards_Out},
			{"innerfocus"_sv, Ability::Inner_Focus},
			{"insomnia"_sv, Ability::Insomnia},
			{"intimidate"_sv, Ability::Intimidate},
			{"intrepidsword"_sv, Ability::Intrepid_Sword},
			{"ironbarbs"_sv, Ability::Iron_Barbs},
			{"ironfist"_sv, Ability::Iron_Fist},
			{"justified"_sv, Ability::Justified},
			{"keeneye"_sv, Ability::Keen_Eye},
			{"klutz"_sv, Ability::Klutz},
			{"leafguard"_sv, Ability::Leaf_Guard},
			{"levitate"_sv, Ability::Levitate},
			{"libero"_sv, Ability::Libero},
			{"lightmetal"_sv, Ability::Light_Metal},
			{"lightningrod"_sv, Ability::Lightning_Rod},
			{"limber"_sv, Ability::Limber},
			{"liquidooze"_sv, Ability::Liquid_Ooze},
			{"liquidvoice"_sv, Ability::Liquid_Voice},
			{"longreach"_sv, Ability::Long_Reach},
			{"magicbounce"_sv, Ability::Magic_Bounce},
			{"magicguard"_sv, Ability::Magic_Guard},
			{"magician"_sv, Ability::Magician},
			{"magmaarmor"_sv, Ability::Magma_Armor},
			{"magnetpull"_sv, Ability::Magnet_Pull},
			{"marvelscale"_sv, Ability::Marvel_Scale},
			{"megalauncher"_sv, Ability::Mega_Launcher},
			{"merciless"_sv, Ability::Merciless},
			{"mimicry"_sv, Ability::Mimicry},
			{"minus"_sv, Ability::Minus},
			{"mirrorarmor"_sv, Ability::Mirror_Armor},
			{"mistysurge"_sv, Ability::Misty_Surge},
			{"moldbreaker"_sv, Ability::Mold_Breaker},
			{"moody"_sv, Ability::Moody},
			{"motordrive"_sv, Ability::Motor_Drive},
			{"moxie"_sv, Ability::Moxie},
			{"multiscale"_sv, Ability::Multiscale},
			{"multitype"_sv, Ability::Multitype},
			{"mummy"_sv, Ability::Mummy},
			{"naturalcure"_sv, Ability::Natural_Cure},
			{"neuroforce"_sv, Ability::Neuroforce},
			{"neutralizinggas"_sv, Ability::Neutralizing_Gas},
			{"noability"_sv, Ability::Honey_Gather}, // TODO: ???
			{"noguard"_sv, Ability::No_Guard},
			{"none"_sv, Ability::Honey_Gather}, // TODO: ???
			{"normalize"_sv, Ability::Normalize},
			{"oblivious"_sv, Ability::Oblivious},
			{"overcoat"_sv, Ability::Overcoat},
			{"overgrow"_sv, Ability::Overgrow},
			{"owntempo"_sv, Ability::Own_Tempo},
			{"parentalbond"_sv, Ability::Parental_Bond},
			{"pastelveil"_sv, Ability::Pastel_Veil},
			{"perishbody"_sv, Ability::Perish_Body},
			{"pickpocket"_sv, Ability::Pickpocket},
			{"pickup"_sv, Ability::Pickup},
			{"pixilate"_sv, Ability::Pixilate},
			{"plus"_sv, Ability::Plus},
			{"poisonheal"_sv, Ability::Poison_Heal},
			{"poisonpoint"_sv, Ability::Poison_Point},
			{"poisontouch"_sv, Ability::Poison_Touch},
			{"powerconstruct"_sv, Ability::Power_Construct},
			{"powerofalchemy"_sv, Ability::Power_of_Alchemy},
			{"powerspot"_sv, Ability::Power_Spot},
			{"prankster"_sv, Ability::Prankster},
			{"pressure"_sv, Ability::Pressure},
			{"primordialsea"_sv, Ability::Primordial_Sea},
			{"prismarmor"_sv, Ability::Prism_Armor},
			{"propellertail"_sv, Ability::Propeller_Tail},
			{"protean"_sv, Ability::Protean},
			{"psychicsurge"_sv, Ability::Psychic_Surge},
			{"punkrock"_sv, Ability::Punk_Rock},
			{"purepower"_sv, Ability::Pure_Power},
			{"queenlymajesty"_sv, Ability::Queenly_Majesty},
			{"quickfeet"_sv, Ability::Quick_Feet},
			{"raindish"_sv, Ability::Rain_Dish},
			{"rattled"_sv, Ability::Rattled},
			{"receiver"_sv, Ability::Receiver},
			{"reckless"_sv, Ability::Reckless},
			{"refrigerate"_sv, Ability::Refrigerate},
			{"regenerator"_sv, Ability::Regenerator},
			{"ripen"_sv, Ability::Ripen},
			{"rivalry"_sv, Ability::Rivalry},
			{"rkssystem"_sv, Ability::RKS_System},
			{"rockhead"_sv, Ability::Rock_Head},
			{"roughskin"_sv, Ability::Rough_Skin},
			{"runaway"_sv, Ability::Run_Away},
			{"sandforce"_sv, Ability::Sand_Force},
			{"sandrush"_sv, Ability::Sand_Rush},
			{"sandspit"_sv, Ability::Sand_Spit},
			{"sandstream"_sv, Ability::Sand_Stream},
			{"sandveil"_sv, Ability::Sand_Veil},
			{"sapsipper"_sv, Ability::Sap_Sipper},
			{"schooling"_sv, Ability::Schooling},
			{"scrappy"_sv, Ability::Scrappy},
			{"screencleaner"_sv, Ability::Screen_Cleaner},
			{"serenegrace"_sv, Ability::Serene_Grace},
			{"shadowshield"_sv, Ability::Shadow_Shield},
			{"shadowtag"_sv, Ability::Shadow_Tag},
			{"shedskin"_sv, Ability::Shed_Skin},
			{"sheerforce"_sv, Ability::Sheer_Force},
			{"shellarmor"_sv, Ability::Shell_Armor},
			{"shielddust"_sv, Ability::Shield_Dust},
			{"shieldsdown"_sv, Ability::Shields_Down},
			{"simple"_sv, Ability::Simple},
			{"skilllink"_sv, Ability::Skill_Link},
			{"slowstart"_sv, Ability::Slow_Start},
			{"slushrush"_sv, Ability::Slush_Rush},
			{"sniper"_sv, Ability::Sniper},
			{"snowcloak"_sv, Ability::Snow_Cloak},
			{"snowwarning"_sv, Ability::Snow_Warning},
			{"solarpower"_sv, Ability::Solar_Power},
			{"solidrock"_sv, Ability::Solid_Rock},
			{"soulheart"_sv, Ability::Soul_Heart},
			{"soundproof"_sv, Ability::Soundproof},
			{"speedboost"_sv, Ability::Speed_Boost},
			{"stakeout"_sv, Ability::Stakeout},
			{"stall"_sv, Ability::Stall},
			{"stalwart"_sv, Ability::Stalwart},
			{"stamina"_sv, Ability::Stamina},
			{"stancechange"_sv, Ability::Stance_Change},
			{"static"_sv, Ability::Static},
			{"steadfast"_sv, Ability::Steadfast},
			{"steamengine"_sv, Ability::Steam_Engine},
			{"steelworker"_sv, Ability::Steelworker},
			{"steelyspirit"_sv, Ability::Steely_Spirit},
			{"stench"_sv, Ability::Stench},
			{"stickyhold"_sv, Ability::Sticky_Hold},
			{"stormdrain"_sv, Ability::Storm_Drain},
			{"strongjaw"_sv, Ability::Strong_Jaw},
			{"sturdy"_sv, Ability::Sturdy},
			{"suctioncups"_sv, Ability::Suction_Cups},
			{"superluck"_sv, Ability::Super_Luck},
			{"surgesurfer"_sv, Ability::Surge_Surfer},
			{"swarm"_sv, Ability::Swarm},
			{"sweetveil"_sv, Ability::Sweet_Veil},
			{"swiftswim"_sv, Ability::Swift_Swim},
			{"symbiosis"_sv, Ability::Symbiosis},
			{"synchronize"_sv, Ability::Synchronize},
			{"tangledfeet"_sv, Ability::Tangled_Feet},
			{"tanglinghair"_sv, Ability::Tangling_Hair},
			{"technician"_sv, Ability::Technician},
			{"telepathy"_sv, Ability::Telepathy},
			{"teravolt"_sv, Ability::Teravolt},
			{"thickfat"_sv, Ability::Thick_Fat},
			{"tintedlens"_sv, Ability::Tinted_Lens},
			{"torrent"_sv, Ability::Torrent},
			{"toughclaws"_sv, Ability::Tough_Claws},
			{"toxicboost"_sv, Ability::Toxic_Boost},
			{"trace"_sv, Ability::Trace},
			{"triage"_sv, Ability::Triage},
			{"truant"_sv, Ability::Truant},
			{"turboblaze"_sv, Ability::Turboblaze},
			{"unaware"_sv, Ability::Unaware},
			{"unburden"_sv, Ability::Unburden},
			{"unnerve"_sv, Ability::Unnerve},
			{"victorystar"_sv, Ability::Victory_Star},
			{"vitalspirit"_sv, Ability::Vital_Spirit},
			{"voltabsorb"_sv, Ability::Volt_Absorb},
			{"wanderingspirit"_sv, Ability::Wandering_Spirit},
			{"waterabsorb"_sv, Ability::Water_Absorb},
			{"waterbubble"_sv, Ability::Water_Bubble},
			{"watercompaction"_sv, Ability::Water_Compaction},
			{"waterveil"_sv, Ability::Water_Veil},
			{"weakarmor"_sv, Ability::Weak_Armor},
			{"whitesmoke"_sv, Ability::White_Smoke},
			{"wimpout"_sv, Ability::Wimp_Out},
			{"wonderguard"_sv, Ability::Wonder_Guard},
			{"wonderskin"_sv, Ability::Wonder_Skin},
			{"zenmode"_sv, Ability::Zen_Mode},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<15_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Ability"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
