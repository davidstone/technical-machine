// Ability string conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/ability.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/ability.hpp>

#include <containers/array/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {

auto to_string(Ability const ability) -> std::string_view {
	switch (ability) {
		case Ability::Stench: return "Stench";
		case Ability::Drizzle: return "Drizzle";
		case Ability::Speed_Boost: return "Speed Boost";
		case Ability::Battle_Armor: return "Battle Armor";
		case Ability::Sturdy: return "Sturdy";
		case Ability::Damp: return "Damp";
		case Ability::Limber: return "Limber";
		case Ability::Sand_Veil: return "Sand Veil";
		case Ability::Static: return "Static";
		case Ability::Volt_Absorb: return "Volt Absorb";
		case Ability::Water_Absorb: return "Water Absorb";
		case Ability::Oblivious: return "Oblivious";
		case Ability::Cloud_Nine: return "Cloud Nine";
		case Ability::Compound_Eyes: return "Compound Eyes";
		case Ability::Insomnia: return "Insomnia";
		case Ability::Color_Change: return "Color Change";
		case Ability::Immunity: return "Immunity";
		case Ability::Flash_Fire: return "Flash Fire";
		case Ability::Shield_Dust: return "Shield Dust";
		case Ability::Own_Tempo: return "Own Tempo";
		case Ability::Suction_Cups: return "Suction Cups";
		case Ability::Intimidate: return "Intimidate";
		case Ability::Shadow_Tag: return "Shadow Tag";
		case Ability::Rough_Skin: return "Rough Skin";
		case Ability::Wonder_Guard: return "Wonder Guard";
		case Ability::Levitate: return "Levitate";
		case Ability::Effect_Spore: return "Effect Spore";
		case Ability::Synchronize: return "Synchronize";
		case Ability::Clear_Body: return "Clear Body";
		case Ability::Natural_Cure: return "Natural Cure";
		case Ability::Lightning_Rod: return "Lightning Rod";
		case Ability::Serene_Grace: return "Serene Grace";
		case Ability::Swift_Swim: return "Swift Swim";
		case Ability::Chlorophyll: return "Chlorophyll";
		case Ability::Illuminate: return "Illuminate";
		case Ability::Trace: return "Trace";
		case Ability::Huge_Power: return "Huge Power";
		case Ability::Poison_Point: return "Poison Point";
		case Ability::Inner_Focus: return "Inner Focus";
		case Ability::Magma_Armor: return "Magma Armor";
		case Ability::Water_Veil: return "Water Veil";
		case Ability::Magnet_Pull: return "Magnet Pull";
		case Ability::Soundproof: return "Soundproof";
		case Ability::Rain_Dish: return "Rain Dish";
		case Ability::Sand_Stream: return "Sand Stream";
		case Ability::Pressure: return "Pressure";
		case Ability::Thick_Fat: return "Thick Fat";
		case Ability::Early_Bird: return "Early Bird";
		case Ability::Flame_Body: return "Flame Body";
		case Ability::Run_Away: return "Run Away";
		case Ability::Keen_Eye: return "Keen Eye";
		case Ability::Hyper_Cutter: return "Hyper Cutter";
		case Ability::Pickup: return "Pickup";
		case Ability::Truant: return "Truant";
		case Ability::Hustle: return "Hustle";
		case Ability::Cute_Charm: return "Cute Charm";
		case Ability::Plus: return "Plus";
		case Ability::Minus: return "Minus";
		case Ability::Forecast: return "Forecast";
		case Ability::Sticky_Hold: return "Sticky Hold";
		case Ability::Shed_Skin: return "Shed Skin";
		case Ability::Guts: return "Guts";
		case Ability::Marvel_Scale: return "Marvel Scale";
		case Ability::Liquid_Ooze: return "Liquid Ooze";
		case Ability::Overgrow: return "Overgrow";
		case Ability::Blaze: return "Blaze";
		case Ability::Torrent: return "Torrent";
		case Ability::Swarm: return "Swarm";
		case Ability::Rock_Head: return "Rock Head";
		case Ability::Drought: return "Drought";
		case Ability::Arena_Trap: return "Arena Trap";
		case Ability::Vital_Spirit: return "Vital Spirit";
		case Ability::White_Smoke: return "White Smoke";
		case Ability::Pure_Power: return "Pure Power";
		case Ability::Shell_Armor: return "Shell Armor";
		case Ability::Air_Lock: return "Air Lock";
	
		// Generation 4
		case Ability::Tangled_Feet: return "Tangled Feet";
		case Ability::Motor_Drive: return "Motor Drive";
		case Ability::Rivalry: return "Rivalry";
		case Ability::Steadfast: return "Steadfast";
		case Ability::Snow_Cloak: return "Snow Cloak";
		case Ability::Gluttony: return "Gluttony";
		case Ability::Anger_Point: return "Anger Point";
		case Ability::Unburden: return "Unburden";
		case Ability::Heatproof: return "Heatproof";
		case Ability::Simple: return "Simple";
		case Ability::Dry_Skin: return "Dry Skin";
		case Ability::Download: return "Download";
		case Ability::Iron_Fist: return "Iron Fist";
		case Ability::Poison_Heal: return "Poison Heal";
		case Ability::Adaptability: return "Adaptability";
		case Ability::Skill_Link: return "Skill Link";
		case Ability::Hydration: return "Hydration";
		case Ability::Solar_Power: return "Solar Power";
		case Ability::Quick_Feet: return "Quick Feet";
		case Ability::Normalize: return "Normalize";
		case Ability::Sniper: return "Sniper";
		case Ability::Magic_Guard: return "Magic Guard";
		case Ability::No_Guard: return "No Guard";
		case Ability::Stall: return "Stall";
		case Ability::Technician: return "Technician";
		case Ability::Leaf_Guard: return "Leaf Guard";
		case Ability::Klutz: return "Klutz";
		case Ability::Mold_Breaker: return "Mold Breaker";
		case Ability::Super_Luck: return "Super Luck";
		case Ability::Aftermath: return "Aftermath";
		case Ability::Anticipation: return "Anticipation";
		case Ability::Forewarn: return "Forewarn";
		case Ability::Unaware: return "Unaware";
		case Ability::Tinted_Lens: return "Tinted Lens";
		case Ability::Filter: return "Filter";
		case Ability::Slow_Start: return "Slow Start";
		case Ability::Scrappy: return "Scrappy";
		case Ability::Storm_Drain: return "Storm Drain";
		case Ability::Ice_Body: return "Ice Body";
		case Ability::Solid_Rock: return "Solid Rock";
		case Ability::Snow_Warning: return "Snow Warning";
		case Ability::Honey_Gather: return "Honey Gather";
		case Ability::Frisk: return "Frisk";
		case Ability::Reckless: return "Reckless";
		case Ability::Multitype: return "Multitype";
		case Ability::Flower_Gift: return "Flower Gift";
		case Ability::Bad_Dreams: return "Bad Dreams";
	
		// Generation 5
		case Ability::Pickpocket: return "Pickpocket";
		case Ability::Sheer_Force: return "Sheer Force";
		case Ability::Contrary: return "Contrary";
		case Ability::Unnerve: return "Unnerve";
		case Ability::Defiant: return "Defiant";
		case Ability::Defeatist: return "Defeatist";
		case Ability::Cursed_Body: return "Cursed Body";
		case Ability::Healer: return "Healer";
		case Ability::Friend_Guard: return "Friend Guard";
		case Ability::Weak_Armor: return "Weak Armor";
		case Ability::Heavy_Metal: return "Heavy Metal";
		case Ability::Light_Metal: return "Light Metal";
		case Ability::Multiscale: return "Multiscale";
		case Ability::Toxic_Boost: return "Toxic Boost";
		case Ability::Flare_Boost: return "Flare Boost";
		case Ability::Harvest: return "Harvest";
		case Ability::Telepathy: return "Telepathy";
		case Ability::Moody: return "Moody";
		case Ability::Overcoat: return "Overcoat";
		case Ability::Poison_Touch: return "Poison Touch";
		case Ability::Regenerator: return "Regenerator";
		case Ability::Big_Pecks: return "Big Pecks";
		case Ability::Sand_Rush: return "Sand Rush";
		case Ability::Wonder_Skin: return "Wonder Skin";
		case Ability::Analytic: return "Analytic";
		case Ability::Illusion: return "Illusion";
		case Ability::Imposter: return "Imposter";
		case Ability::Infiltrator: return "Infiltrator";
		case Ability::Mummy: return "Mummy";
		case Ability::Moxie: return "Moxie";
		case Ability::Justified: return "Justified";
		case Ability::Rattled: return "Rattled";
		case Ability::Magic_Bounce: return "Magic Bounce";
		case Ability::Sap_Sipper: return "Sap Sipper";
		case Ability::Prankster: return "Prankster";
		case Ability::Sand_Force: return "Sand Force";
		case Ability::Iron_Barbs: return "Iron Barbs";
		case Ability::Zen_Mode: return "Zen Mode";
		case Ability::Victory_Star: return "Victory Star";
		case Ability::Turboblaze: return "Turboblaze";
		case Ability::Teravolt: return "Teravolt";
	
		// Generation 6
		case Ability::Aroma_Veil: return "Aroma Veil";
		case Ability::Flower_Veil: return "Flower Veil";
		case Ability::Cheek_Pouch: return "Cheek Pouch";
		case Ability::Protean: return "Protean";
		case Ability::Fur_Coat: return "Fur Coat";
		case Ability::Magician: return "Magician";
		case Ability::Bulletproof: return "Bulletproof";
		case Ability::Competitive: return "Competitive";
		case Ability::Strong_Jaw: return "Strong Jaw";
		case Ability::Refrigerate: return "Refrigerate";
		case Ability::Sweet_Veil: return "Sweet Veil";
		case Ability::Stance_Change: return "Stance Change";
		case Ability::Gale_Wings: return "Gale Wings";
		case Ability::Mega_Launcher: return "Mega Launcher";
		case Ability::Grass_Pelt: return "Grass Pelt";
		case Ability::Symbiosis: return "Symbiosis";
		case Ability::Tough_Claws: return "Tough Claws";
		case Ability::Pixilate: return "Pixilate";
		case Ability::Gooey: return "Gooey";
		case Ability::Aerilate: return "Aerilate";
		case Ability::Parental_Bond: return "Parental Bond";
		case Ability::Dark_Aura: return "Dark Aura";
		case Ability::Fairy_Aura: return "Fairy Aura";
		case Ability::Aura_Break: return "Aura Break";
		case Ability::Primordial_Sea: return "Primordial Sea";
		case Ability::Desolate_Land: return "Desolate Land";
		case Ability::Delta_Stream: return "Delta Stream";
	
		// Generation 7
		case Ability::Stamina: return "Stamina";
		case Ability::Wimp_Out: return "Wimp Out";
		case Ability::Emergency_Exit: return "Emergency Exit";
		case Ability::Water_Compaction: return "Water Compaction";
		case Ability::Merciless: return "Merciless";
		case Ability::Shields_Down: return "Shields Down";
		case Ability::Stakeout: return "Stakeout";
		case Ability::Water_Bubble: return "Water Bubble";
		case Ability::Steelworker: return "Steelworker";
		case Ability::Berserk: return "Berserk";
		case Ability::Slush_Rush: return "Slush Rush";
		case Ability::Long_Reach: return "Long Reach";
		case Ability::Liquid_Voice: return "Liquid Voice";
		case Ability::Triage: return "Triage";
		case Ability::Galvanize: return "Galvanize";
		case Ability::Surge_Surfer: return "Surge Surfer";
		case Ability::Schooling: return "Schooling";
		case Ability::Disguise: return "Disguise";
		case Ability::Battle_Bond: return "Battle Bond";
		case Ability::Power_Construct: return "Power Construct";
		case Ability::Corrosion: return "Corrosion";
		case Ability::Comatose: return "Comatose";
		case Ability::Queenly_Majesty: return "Queenly Majesty";
		case Ability::Innards_Out: return "Innards Out";
		case Ability::Dancer: return "Dancer";
		case Ability::Battery: return "Battery";
		case Ability::Fluffy: return "Fluffy";
		case Ability::Dazzling: return "Dazzling";
		case Ability::Soul_Heart: return "Soul-Heart";
		case Ability::Tangling_Hair: return "Tangling Hair";
		case Ability::Receiver: return "Receiver";
		case Ability::Power_of_Alchemy: return "Power of Alchemy";
		case Ability::Beast_Boost: return "Beast Boost";
		case Ability::RKS_System: return "RKS System";
		case Ability::Electric_Surge: return "Electric Surge";
		case Ability::Psychic_Surge: return "Psychic Surge";
		case Ability::Misty_Surge: return "Misty Surge";
		case Ability::Grassy_Surge: return "Grassy Surge";
		case Ability::Full_Metal_Body: return "Full Metal Body";
		case Ability::Shadow_Shield: return "Shadow Shield";
		case Ability::Prism_Armor: return "Prism Armor";
		case Ability::Neuroforce: return "Neuroforce";
	
		// Generation 8
		case Ability::Intrepid_Sword: return "Intrepid Sword";
		case Ability::Dauntless_Shield: return "Dauntless Shield";
		case Ability::Libero: return "Libero";
		case Ability::Ball_Fetch: return "Ball Fetch";
		case Ability::Cotton_Down: return "Cotton Down";
		case Ability::Propeller_Tail: return "Propeller Tail";
		case Ability::Mirror_Armor: return "Mirror Armor";
		case Ability::Gulp_Missile: return "Gulp Missile";
		case Ability::Stalwart: return "Stalwart";
		case Ability::Steam_Engine: return "Steam Engine";
		case Ability::Punk_Rock: return "Punk Rock";
		case Ability::Sand_Spit: return "Sand Spit";
		case Ability::Ice_Scales: return "Ice Scales";
		case Ability::Ripen: return "Ripen";
		case Ability::Ice_Face: return "Ice Face";
		case Ability::Power_Spot: return "Power Spot";
		case Ability::Mimicry: return "Mimicry";
		case Ability::Screen_Cleaner: return "Screen Cleaner";
		case Ability::Steely_Spirit: return "Steely Spirit";
		case Ability::Perish_Body: return "Perish Body";
		case Ability::Wandering_Spirit: return "Wandering Spirit";
		case Ability::Gorilla_Tactics: return "Gorilla Tactics";
		case Ability::Neutralizing_Gas: return "Neutralizing Gas";
		case Ability::Pastel_Veil: return "Pastel Veil";
		case Ability::Hunger_Switch: return "Hunger Switch";
	}
}

template<>
auto from_string<Ability>(std::string_view const str) -> Ability {
	using Storage = containers::array<containers::map_value_type<std::string_view, Ability>, 260>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{"adaptability", Ability::Adaptability},
			{"aerilate", Ability::Aerilate},
			{"aftermath", Ability::Aftermath},
			{"airlock", Ability::Air_Lock},
			{"analytic", Ability::Analytic},
			{"angerpoint", Ability::Anger_Point},
			{"anticipation", Ability::Anticipation},
			{"arenatrap", Ability::Arena_Trap},
			{"aromaveil", Ability::Aroma_Veil},
			{"aurabreak", Ability::Aura_Break},
			{"baddreams", Ability::Bad_Dreams},
			{"ballfetch", Ability::Ball_Fetch},
			{"battery", Ability::Battery},
			{"battlearmor", Ability::Battle_Armor},
			{"battlebond", Ability::Battle_Bond},
			{"beastboost", Ability::Beast_Boost},
			{"berserk", Ability::Berserk},
			{"bigpecks", Ability::Big_Pecks},
			{"blaze", Ability::Blaze},
			{"bulletproof", Ability::Bulletproof},
			{"cheekpouch", Ability::Cheek_Pouch},
			{"chlorophyll", Ability::Chlorophyll},
			{"clearbody", Ability::Clear_Body},
			{"cloudnine", Ability::Cloud_Nine},
			{"colorchange", Ability::Color_Change},
			{"comatose", Ability::Comatose},
			{"competitive", Ability::Competitive},
			{"compoundeyes", Ability::Compound_Eyes},
			{"contrary", Ability::Contrary},
			{"corrosion", Ability::Corrosion},
			{"cottondown", Ability::Cotton_Down},
			{"cursedbody", Ability::Cursed_Body},
			{"cutecharm", Ability::Cute_Charm},
			{"damp", Ability::Damp},
			{"dancer", Ability::Dancer},
			{"darkaura", Ability::Dark_Aura},
			{"dauntlessshield", Ability::Dauntless_Shield},
			{"dazzling", Ability::Dazzling},
			{"defeatist", Ability::Defeatist},
			{"defiant", Ability::Defiant},
			{"deltastream", Ability::Delta_Stream},
			{"desolateland", Ability::Desolate_Land},
			{"disguise", Ability::Disguise},
			{"download", Ability::Download},
			{"drizzle", Ability::Drizzle},
			{"drought", Ability::Drought},
			{"dryskin", Ability::Dry_Skin},
			{"earlybird", Ability::Early_Bird},
			{"effectspore", Ability::Effect_Spore},
			{"electricsurge", Ability::Electric_Surge},
			{"emergencyexit", Ability::Emergency_Exit},
			{"fairyaura", Ability::Fairy_Aura},
			{"filter", Ability::Filter},
			{"flamebody", Ability::Flame_Body},
			{"flareboost", Ability::Flare_Boost},
			{"flashfire", Ability::Flash_Fire},
			{"flowergift", Ability::Flower_Gift},
			{"flowerveil", Ability::Flower_Veil},
			{"fluffy", Ability::Fluffy},
			{"forecast", Ability::Forecast},
			{"forewarn", Ability::Forewarn},
			{"friendguard", Ability::Friend_Guard},
			{"frisk", Ability::Frisk},
			{"fullmetalbody", Ability::Full_Metal_Body},
			{"furcoat", Ability::Fur_Coat},
			{"galewings", Ability::Gale_Wings},
			{"galvanize", Ability::Galvanize},
			{"gluttony", Ability::Gluttony},
			{"gooey", Ability::Gooey},
			{"gorillatactics", Ability::Gorilla_Tactics},
			{"grasspelt", Ability::Grass_Pelt},
			{"grassysurge", Ability::Grassy_Surge},
			{"gulpmissile", Ability::Gulp_Missile},
			{"guts", Ability::Guts},
			{"harvest", Ability::Harvest},
			{"healer", Ability::Healer},
			{"heatproof", Ability::Heatproof},
			{"heavymetal", Ability::Heavy_Metal},
			{"honeygather", Ability::Honey_Gather},
			{"hugepower", Ability::Huge_Power},
			{"hungerswitch", Ability::Hunger_Switch},
			{"hustle", Ability::Hustle},
			{"hydration", Ability::Hydration},
			{"hypercutter", Ability::Hyper_Cutter},
			{"icebody", Ability::Ice_Body},
			{"iceface", Ability::Ice_Face},
			{"icescales", Ability::Ice_Scales},
			{"illuminate", Ability::Illuminate},
			{"illusion", Ability::Illusion},
			{"immunity", Ability::Immunity},
			{"imposter", Ability::Imposter},
			{"infiltrator", Ability::Infiltrator},
			{"innardsout", Ability::Innards_Out},
			{"innerfocus", Ability::Inner_Focus},
			{"insomnia", Ability::Insomnia},
			{"intimidate", Ability::Intimidate},
			{"intrepidsword", Ability::Intrepid_Sword},
			{"ironbarbs", Ability::Iron_Barbs},
			{"ironfist", Ability::Iron_Fist},
			{"justified", Ability::Justified},
			{"keeneye", Ability::Keen_Eye},
			{"klutz", Ability::Klutz},
			{"leafguard", Ability::Leaf_Guard},
			{"levitate", Ability::Levitate},
			{"libero", Ability::Libero},
			{"lightmetal", Ability::Light_Metal},
			{"lightningrod", Ability::Lightning_Rod},
			{"limber", Ability::Limber},
			{"liquidooze", Ability::Liquid_Ooze},
			{"liquidvoice", Ability::Liquid_Voice},
			{"longreach", Ability::Long_Reach},
			{"magicbounce", Ability::Magic_Bounce},
			{"magicguard", Ability::Magic_Guard},
			{"magician", Ability::Magician},
			{"magmaarmor", Ability::Magma_Armor},
			{"magnetpull", Ability::Magnet_Pull},
			{"marvelscale", Ability::Marvel_Scale},
			{"megalauncher", Ability::Mega_Launcher},
			{"merciless", Ability::Merciless},
			{"mimicry", Ability::Mimicry},
			{"minus", Ability::Minus},
			{"mirrorarmor", Ability::Mirror_Armor},
			{"mistysurge", Ability::Misty_Surge},
			{"moldbreaker", Ability::Mold_Breaker},
			{"moody", Ability::Moody},
			{"motordrive", Ability::Motor_Drive},
			{"moxie", Ability::Moxie},
			{"multiscale", Ability::Multiscale},
			{"multitype", Ability::Multitype},
			{"mummy", Ability::Mummy},
			{"naturalcure", Ability::Natural_Cure},
			{"neuroforce", Ability::Neuroforce},
			{"neutralizinggas", Ability::Neutralizing_Gas},
			{"noability", Ability::Honey_Gather}, // TODO: ???
			{"noguard", Ability::No_Guard},
			{"none", Ability::Honey_Gather}, // TODO: ???
			{"normalize", Ability::Normalize},
			{"oblivious", Ability::Oblivious},
			{"overcoat", Ability::Overcoat},
			{"overgrow", Ability::Overgrow},
			{"owntempo", Ability::Own_Tempo},
			{"parentalbond", Ability::Parental_Bond},
			{"pastelveil", Ability::Pastel_Veil},
			{"perishbody", Ability::Perish_Body},
			{"pickpocket", Ability::Pickpocket},
			{"pickup", Ability::Pickup},
			{"pixilate", Ability::Pixilate},
			{"plus", Ability::Plus},
			{"poisonheal", Ability::Poison_Heal},
			{"poisonpoint", Ability::Poison_Point},
			{"poisontouch", Ability::Poison_Touch},
			{"powerconstruct", Ability::Power_Construct},
			{"powerofalchemy", Ability::Power_of_Alchemy},
			{"powerspot", Ability::Power_Spot},
			{"prankster", Ability::Prankster},
			{"pressure", Ability::Pressure},
			{"primordialsea", Ability::Primordial_Sea},
			{"prismarmor", Ability::Prism_Armor},
			{"propellertail", Ability::Propeller_Tail},
			{"protean", Ability::Protean},
			{"psychicsurge", Ability::Psychic_Surge},
			{"punkrock", Ability::Punk_Rock},
			{"purepower", Ability::Pure_Power},
			{"queenlymajesty", Ability::Queenly_Majesty},
			{"quickfeet", Ability::Quick_Feet},
			{"raindish", Ability::Rain_Dish},
			{"rattled", Ability::Rattled},
			{"receiver", Ability::Receiver},
			{"reckless", Ability::Reckless},
			{"refrigerate", Ability::Refrigerate},
			{"regenerator", Ability::Regenerator},
			{"ripen", Ability::Ripen},
			{"rivalry", Ability::Rivalry},
			{"rkssystem", Ability::RKS_System},
			{"rockhead", Ability::Rock_Head},
			{"roughskin", Ability::Rough_Skin},
			{"runaway", Ability::Run_Away},
			{"sandforce", Ability::Sand_Force},
			{"sandrush", Ability::Sand_Rush},
			{"sandspit", Ability::Sand_Spit},
			{"sandstream", Ability::Sand_Stream},
			{"sandveil", Ability::Sand_Veil},
			{"sapsipper", Ability::Sap_Sipper},
			{"schooling", Ability::Schooling},
			{"scrappy", Ability::Scrappy},
			{"screencleaner", Ability::Screen_Cleaner},
			{"serenegrace", Ability::Serene_Grace},
			{"shadowshield", Ability::Shadow_Shield},
			{"shadowtag", Ability::Shadow_Tag},
			{"shedskin", Ability::Shed_Skin},
			{"sheerforce", Ability::Sheer_Force},
			{"shellarmor", Ability::Shell_Armor},
			{"shielddust", Ability::Shield_Dust},
			{"shieldsdown", Ability::Shields_Down},
			{"simple", Ability::Simple},
			{"skilllink", Ability::Skill_Link},
			{"slowstart", Ability::Slow_Start},
			{"slushrush", Ability::Slush_Rush},
			{"sniper", Ability::Sniper},
			{"snowcloak", Ability::Snow_Cloak},
			{"snowwarning", Ability::Snow_Warning},
			{"solarpower", Ability::Solar_Power},
			{"solidrock", Ability::Solid_Rock},
			{"soulheart", Ability::Soul_Heart},
			{"soundproof", Ability::Soundproof},
			{"speedboost", Ability::Speed_Boost},
			{"stakeout", Ability::Stakeout},
			{"stall", Ability::Stall},
			{"stalwart", Ability::Stalwart},
			{"stamina", Ability::Stamina},
			{"stancechange", Ability::Stance_Change},
			{"static", Ability::Static},
			{"steadfast", Ability::Steadfast},
			{"steamengine", Ability::Steam_Engine},
			{"steelworker", Ability::Steelworker},
			{"steelyspirit", Ability::Steely_Spirit},
			{"stench", Ability::Stench},
			{"stickyhold", Ability::Sticky_Hold},
			{"stormdrain", Ability::Storm_Drain},
			{"strongjaw", Ability::Strong_Jaw},
			{"sturdy", Ability::Sturdy},
			{"suctioncups", Ability::Suction_Cups},
			{"superluck", Ability::Super_Luck},
			{"surgesurfer", Ability::Surge_Surfer},
			{"swarm", Ability::Swarm},
			{"sweetveil", Ability::Sweet_Veil},
			{"swiftswim", Ability::Swift_Swim},
			{"symbiosis", Ability::Symbiosis},
			{"synchronize", Ability::Synchronize},
			{"tangledfeet", Ability::Tangled_Feet},
			{"tanglinghair", Ability::Tangling_Hair},
			{"technician", Ability::Technician},
			{"telepathy", Ability::Telepathy},
			{"teravolt", Ability::Teravolt},
			{"thickfat", Ability::Thick_Fat},
			{"tintedlens", Ability::Tinted_Lens},
			{"torrent", Ability::Torrent},
			{"toughclaws", Ability::Tough_Claws},
			{"toxicboost", Ability::Toxic_Boost},
			{"trace", Ability::Trace},
			{"triage", Ability::Triage},
			{"truant", Ability::Truant},
			{"turboblaze", Ability::Turboblaze},
			{"unaware", Ability::Unaware},
			{"unburden", Ability::Unburden},
			{"unnerve", Ability::Unnerve},
			{"victorystar", Ability::Victory_Star},
			{"vitalspirit", Ability::Vital_Spirit},
			{"voltabsorb", Ability::Volt_Absorb},
			{"wanderingspirit", Ability::Wandering_Spirit},
			{"waterabsorb", Ability::Water_Absorb},
			{"waterbubble", Ability::Water_Bubble},
			{"watercompaction", Ability::Water_Compaction},
			{"waterveil", Ability::Water_Veil},
			{"weakarmor", Ability::Weak_Armor},
			{"whitesmoke", Ability::White_Smoke},
			{"wimpout", Ability::Wimp_Out},
			{"wonderguard", Ability::Wonder_Guard},
			{"wonderskin", Ability::Wonder_Skin},
			{"zenmode", Ability::Zen_Mode},
		}}
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<15>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Ability", str);
	}
	return *result;
}

}	// namespace technicalmachine
