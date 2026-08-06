// Item string conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.item;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.item;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace containers::string_literals;

export constexpr auto to_string(Item const item) -> containers::string_view {
	switch (item) {
		// Generation 1
		case Item::None: return "None"_sv;

		case Item::Antidote: return "Antidote"_sv;
		case Item::Awakening: return "Awakening"_sv;
		case Item::Burn_Heal: return "Burn Heal"_sv;
		case Item::Calcium: return "Calcium"_sv;
		case Item::Carbos: return "Carbos"_sv;
		case Item::Dire_Hit: return "Dire Hit"_sv;
		case Item::Dome_Fossil: return "Dome Fossil"_sv;
		case Item::Elixir: return "Elixir"_sv;
		case Item::Escape_Rope: return "Escape Rope"_sv;
		case Item::Ether: return "Ether"_sv;
		case Item::Exp_Share: return "Exp. Share"_sv;
		case Item::Fire_Stone: return "Fire Stone"_sv;
		case Item::Fresh_Water: return "Fresh Water"_sv;
		case Item::Full_Heal: return "Full Heal"_sv;
		case Item::Full_Restore: return "Full Restore"_sv;
		case Item::Great_Ball: return "Great Ball"_sv;
		case Item::Guard_Spec: return "Guard Spec."_sv;
		case Item::Helix_Fossil: return "Helix Fossil"_sv;
		case Item::HP_Up: return "HP Up"_sv;
		case Item::Hyper_Potion: return "Hyper Potion"_sv;
		case Item::Ice_Heal: return "Ice Heal"_sv;
		case Item::Iron: return "Iron"_sv;
		case Item::Leaf_Stone: return "Leaf Stone"_sv;
		case Item::Lemonade: return "Lemonade"_sv;
		case Item::Master_Ball: return "Master Ball"_sv;
		case Item::Max_Elixir: return "Max Elixir"_sv;
		case Item::Max_Ether: return "Max Ether"_sv;
		case Item::Max_Potion: return "Max Potion"_sv;
		case Item::Max_Repel: return "Max Repel"_sv;
		case Item::Max_Revive: return "Max Revive"_sv;
		case Item::Moon_Stone: return "Moon Stone"_sv;
		case Item::Nugget: return "Nugget"_sv;
		case Item::Old_Amber: return "Old Amber"_sv;
		case Item::Paralyze_Heal: return "Paralyze Heal"_sv;
		case Item::Poke_Ball: return "Poke Ball"_sv;
		case Item::Poke_Doll: return "Poke Doll"_sv;
		case Item::Potion: return "Potion"_sv;
		case Item::PP_Up: return "PP Up"_sv;
		case Item::Protein: return "Protein"_sv;
		case Item::Rare_Candy: return "Rare Candy"_sv;
		case Item::Repel: return "Repel"_sv;
		case Item::Revive: return "Revive"_sv;
		case Item::Safari_Ball: return "Safari Ball"_sv;
		case Item::Soda_Pop: return "Soda Pop"_sv;
		case Item::Super_Potion: return "Super Potion"_sv;
		case Item::Super_Repel: return "Super Repel"_sv;
		case Item::Thunder_Stone: return "Thunder Stone"_sv;
		case Item::TM01: return "TM01"_sv;
		case Item::TM02: return "TM02"_sv;
		case Item::TM03: return "TM03"_sv;
		case Item::TM04: return "TM04"_sv;
		case Item::TM05: return "TM05"_sv;
		case Item::TM06: return "TM06"_sv;
		case Item::TM07: return "TM07"_sv;
		case Item::TM08: return "TM08"_sv;
		case Item::TM09: return "TM09"_sv;
		case Item::TM10: return "TM10"_sv;
		case Item::TM11: return "TM11"_sv;
		case Item::TM12: return "TM12"_sv;
		case Item::TM13: return "TM13"_sv;
		case Item::TM14: return "TM14"_sv;
		case Item::TM15: return "TM15"_sv;
		case Item::TM16: return "TM16"_sv;
		case Item::TM17: return "TM17"_sv;
		case Item::TM18: return "TM18"_sv;
		case Item::TM19: return "TM19"_sv;
		case Item::TM20: return "TM20"_sv;
		case Item::TM21: return "TM21"_sv;
		case Item::TM22: return "TM22"_sv;
		case Item::TM23: return "TM23"_sv;
		case Item::TM24: return "TM24"_sv;
		case Item::TM25: return "TM25"_sv;
		case Item::TM26: return "TM26"_sv;
		case Item::TM27: return "TM27"_sv;
		case Item::TM28: return "TM28"_sv;
		case Item::TM29: return "TM29"_sv;
		case Item::TM30: return "TM30"_sv;
		case Item::TM31: return "TM31"_sv;
		case Item::TM32: return "TM32"_sv;
		case Item::TM33: return "TM33"_sv;
		case Item::TM34: return "TM34"_sv;
		case Item::TM35: return "TM35"_sv;
		case Item::TM36: return "TM36"_sv;
		case Item::TM37: return "TM37"_sv;
		case Item::TM38: return "TM38"_sv;
		case Item::TM39: return "TM39"_sv;
		case Item::TM40: return "TM40"_sv;
		case Item::TM41: return "TM41"_sv;
		case Item::TM42: return "TM42"_sv;
		case Item::TM43: return "TM43"_sv;
		case Item::TM44: return "TM44"_sv;
		case Item::TM45: return "TM45"_sv;
		case Item::TM46: return "TM46"_sv;
		case Item::TM47: return "TM47"_sv;
		case Item::TM48: return "TM48"_sv;
		case Item::TM49: return "TM49"_sv;
		case Item::TM50: return "TM50"_sv;
		case Item::Ultra_Ball: return "Ultra Ball"_sv;
		case Item::Water_Stone: return "Water Stone"_sv;
		case Item::X_Accuracy: return "X Accuracy"_sv;
		case Item::X_Attack: return "X Attack"_sv;
		case Item::X_Defend: return "X Defend"_sv;
		case Item::X_Special: return "X Special"_sv;
		case Item::X_Speed: return "X Speed"_sv;

		// Generation 2
		case Item::Amulet_Coin: return "Amulet Coin"_sv;
		case Item::Berry_Juice: return "Berry Juice"_sv;
		case Item::Berry: return "Berry"_sv;
		case Item::Berserk_Gene: return "Berserk Gene"_sv;
		case Item::Big_Mushroom: return "Big Mushroom"_sv;
		case Item::Big_Pearl: return "Big Pearl"_sv;
		case Item::Bitter_Berry: return "Bitter Berry"_sv;
		case Item::Black_Apricorn: return "Black Apricorn"_sv;
		case Item::Black_Belt: return "Black Belt"_sv;
		case Item::Black_Glasses: return "Black Glasses"_sv;
		case Item::Blue_Apricorn: return "Blue Apricorn"_sv;
		case Item::Bluesky_Mail: return "Bluesky Mail"_sv;
		case Item::Brick_Piece: return "Brick Piece"_sv;
		case Item::Bright_Powder: return "Bright Powder"_sv;
		case Item::Burnt_Berry: return "Burnt Berry"_sv;
		case Item::Charcoal: return "Charcoal"_sv;
		case Item::Cleanse_Tag: return "Cleanse Tag"_sv;
		case Item::Dragon_Fang: return "Dragon Fang"_sv;
		case Item::Dragon_Scale: return "Dragon Scale"_sv;
		case Item::Energy_Powder: return "Energy Powder"_sv;
		case Item::Energy_Root: return "Energy Root"_sv;
		case Item::Eon_Mail: return "Eon Mail"_sv;
		case Item::Everstone: return "Everstone"_sv;
		case Item::Fast_Ball: return "Fast Ball"_sv;
		case Item::Flower_Mail: return "Flower Mail"_sv;
		case Item::Focus_Band: return "Focus Band"_sv;
		case Item::Friend_Ball: return "Friend Ball"_sv;
		case Item::Gold_Berry: return "Gold Berry"_sv;
		case Item::Gold_Leaf: return "Gold Leaf"_sv;
		case Item::Gorgeous_Box: return "Gorgeous Box"_sv;
		case Item::Green_Apricorn: return "Green Apricorn"_sv;
		case Item::Hard_Stone: return "Hard Stone"_sv;
		case Item::Heal_Powder: return "Heal Powder"_sv;
		case Item::Heavy_Ball: return "Heavy Ball"_sv;
		case Item::Ice_Berry: return "Ice Berry"_sv;
		case Item::Kings_Rock: return "King's Rock"_sv;
		case Item::Stick: return "Stick"_sv;
		case Item::Leftovers: return "Leftovers"_sv;
		case Item::Level_Ball: return "Level Ball"_sv;
		case Item::Light_Ball: return "Light Ball"_sv;
		case Item::Litebluemail: return "Litebluemail"_sv;
		case Item::Love_Ball: return "Love Ball"_sv;
		case Item::Lovely_Mail: return "Lovely Mail"_sv;
		case Item::Lucky_Egg: return "Lucky Egg"_sv;
		case Item::Lucky_Punch: return "Lucky Punch"_sv;
		case Item::Lure_Ball: return "Lure Ball"_sv;
		case Item::Magnet: return "Magnet"_sv;
		case Item::Metal_Coat: return "Metal Coat"_sv;
		case Item::Metal_Powder: return "Metal Powder"_sv;
		case Item::Mint_Berry: return "Mint Berry"_sv;
		case Item::Miracle_Seed: return "Miracle Seed"_sv;
		case Item::MiracleBerry: return "MiracleBerry"_sv;
		case Item::Mirage_Mail: return "Mirage Mail"_sv;
		case Item::Moomoo_Milk: return "Moomoo Milk"_sv;
		case Item::Moon_Ball: return "Moon Ball"_sv;
		case Item::Morph_Mail: return "Morph Mail"_sv;
		case Item::Music_Mail: return "Music Mail"_sv;
		case Item::MysteryBerry: return "MysteryBerry"_sv;
		case Item::Mystic_Water: return "Mystic Water"_sv;
		case Item::Never_Melt_Ice: return "Never-Melt Ice"_sv;
		case Item::Normal_Box: return "Normal Box"_sv;
		case Item::Pearl: return "Pearl"_sv;
		case Item::Pink_Apricorn: return "Pink Apricorn"_sv;
		case Item::Pink_Bow: return "Pink Bow"_sv;
		case Item::Poison_Barb: return "Poison Barb"_sv;
		case Item::Polkadot_Bow: return "Polkadot Bow"_sv;
		case Item::Portraitmail: return "Portraitmail"_sv;
		case Item::PRZCureBerry: return "PRZCureBerry"_sv;
		case Item::PSNCureBerry: return "PSNCureBerry"_sv;
		case Item::Quick_Claw: return "Quick Claw"_sv;
		case Item::Rage_Candy_Bar: return "Rage Candy Bar"_sv;
		case Item::Red_Apricorn: return "Red Apricorn"_sv;
		case Item::Revival_Herb: return "Revival Herb"_sv;
		case Item::Sacred_Ash: return "Sacred Ash"_sv;
		case Item::Scope_Lens: return "Scope Lens"_sv;
		case Item::Sharp_Beak: return "Sharp Beak"_sv;
		case Item::Silver_Leaf: return "Silver Leaf"_sv;
		case Item::Silver_Powder: return "Silver Powder"_sv;
		case Item::Slowpoke_Tail: return "Slowpoke Tail"_sv;
		case Item::Smoke_Ball: return "Smoke Ball"_sv;
		case Item::Soft_Sand: return "Soft Sand"_sv;
		case Item::Spell_Tag: return "Spell Tag"_sv;
		case Item::Sport_Ball: return "Sport Ball"_sv;
		case Item::Star_Piece: return "Star Piece"_sv;
		case Item::Stardust: return "Stardust"_sv;
		case Item::Sun_Stone: return "Sun Stone"_sv;
		case Item::Surf_Mail: return "Surf Mail"_sv;
		case Item::Thick_Club: return "Thick Club"_sv;
		case Item::Tiny_Mushroom: return "Tiny Mushroom"_sv;
		case Item::Twisted_Spoon: return "Twisted Spoon"_sv;
		case Item::Up_Grade: return "Up-Grade"_sv;
		case Item::White_Apricorn: return "White Apricorn"_sv;
		case Item::Yellow_Apricorn: return "Yellow Apricorn"_sv;

		// Generation 3
		case Item::Aguav_Berry: return "Aguav Berry"_sv;
		case Item::Apicot_Berry: return "Apicot Berry"_sv;
		case Item::Aspear_Berry: return "Aspear Berry"_sv;
		case Item::Bead_Mail: return "Bead Mail"_sv;
		case Item::Belue_Berry: return "Belue Berry"_sv;
		case Item::Black_Flute: return "Black Flute"_sv;
		case Item::Blue_Flute: return "Blue Flute"_sv;
		case Item::Blue_Orb: return "Blue Orb"_sv;
		case Item::Blue_Scarf: return "Blue Scarf"_sv;
		case Item::Blue_Shard: return "Blue Shard"_sv;
		case Item::Bluk_Berry: return "Bluk Berry"_sv;
		case Item::Cheri_Berry: return "Cheri Berry"_sv;
		case Item::Chesto_Berry: return "Chesto Berry"_sv;
		case Item::Choice_Band: return "Choice Band"_sv;
		case Item::Claw_Fossil: return "Claw Fossil"_sv;
		case Item::Cornn_Berry: return "Cornn Berry"_sv;
		case Item::Deep_Sea_Scale: return "Deep Sea Scale"_sv;
		case Item::Deep_Sea_Tooth: return "Deep Sea Tooth"_sv;
		case Item::Dive_Ball: return "Dive Ball"_sv;
		case Item::Drash_Berry: return "Drash Berry"_sv;
		case Item::Dream_Mail: return "Dream Mail"_sv;
		case Item::Durin_Berry: return "Durin Berry"_sv;
		case Item::Eggant_Berry: return "Eggant Berry"_sv;
		case Item::Enigma_Berry: return "Enigma Berry"_sv;
		case Item::Fab_Mail: return "Fab Mail"_sv;
		case Item::Figy_Berry: return "Figy Berry"_sv;
		case Item::Fluffy_Tail: return "Fluffy Tail"_sv;
		case Item::Ganlon_Berry: return "Ganlon Berry"_sv;
		case Item::Ginema_Berry: return "Ginema Berry"_sv;
		case Item::Glitter_Mail: return "Glitter Mail"_sv;
		case Item::Green_Scarf: return "Green Scarf"_sv;
		case Item::Green_Shard: return "Green Shard"_sv;
		case Item::Grepa_Berry: return "Grepa Berry"_sv;
		case Item::Harbor_Mail: return "Harbor Mail"_sv;
		case Item::Heart_Scale: return "Heart Scale"_sv;
		case Item::Hondew_Berry: return "Hondew Berry"_sv;
		case Item::Iapapa_Berry: return "Iapapa Berry"_sv;
		case Item::Kelpsy_Berry: return "Kelpsy Berry"_sv;
		case Item::Kuo_Berry: return "Kuo Berry"_sv;
		case Item::Lansat_Berry: return "Lansat Berry"_sv;
		case Item::Lava_Cookie: return "Lava Cookie"_sv;
		case Item::Lax_Incense: return "Lax Incense"_sv;
		case Item::Leppa_Berry: return "Leppa Berry"_sv;
		case Item::Liechi_Berry: return "Liechi Berry"_sv;
		case Item::Lum_Berry: return "Lum Berry"_sv;
		case Item::Luxury_Ball: return "Luxury Ball"_sv;
		case Item::Macho_Brace: return "Macho Brace"_sv;
		case Item::Mago_Berry: return "Mago Berry"_sv;
		case Item::Magost_Berry: return "Magost Berry"_sv;
		case Item::Mech_Mail: return "Mech Mail"_sv;
		case Item::Mental_Herb: return "Mental Herb"_sv;
		case Item::Nanab_Berry: return "Nanab Berry"_sv;
		case Item::Nest_Ball: return "Nest Ball"_sv;
		case Item::Net_Ball: return "Net Ball"_sv;
		case Item::Niniku_Berry: return "Niniku Berry"_sv;
		case Item::Nomel_Berry: return "Nomel Berry"_sv;
		case Item::Nutpea_Berry: return "Nutpea Berry"_sv;
		case Item::Oran_Berry: return "Oran Berry"_sv;
		case Item::Orange_Mail: return "Orange Mail"_sv;
		case Item::Pamtre_Berry: return "Pamtre Berry"_sv;
		case Item::Pecha_Berry: return "Pecha Berry"_sv;
		case Item::Persim_Berry: return "Persim Berry"_sv;
		case Item::Petaya_Berry: return "Petaya Berry"_sv;
		case Item::Pinap_Berry: return "Pinap Berry"_sv;
		case Item::Pink_Scarf: return "Pink Scarf"_sv;
		case Item::Pomeg_Berry: return "Pomeg Berry"_sv;
		case Item::PP_Max: return "PP Max"_sv;
		case Item::Premier_Ball: return "Premier Ball"_sv;
		case Item::Pumkin_Berry: return "Pumkin Berry"_sv;
		case Item::Qualot_Berry: return "Qualot Berry"_sv;
		case Item::Rabuta_Berry: return "Rabuta Berry"_sv;
		case Item::Rawst_Berry: return "Rawst Berry"_sv;
		case Item::Razz_Berry: return "Razz Berry"_sv;
		case Item::Red_Flute: return "Red Flute"_sv;
		case Item::Red_Orb: return "Red Orb"_sv;
		case Item::Red_Scarf: return "Red Scarf"_sv;
		case Item::Red_Shard: return "Red Shard"_sv;
		case Item::Repeat_Ball: return "Repeat Ball"_sv;
		case Item::Retro_Mail: return "Retro Mail"_sv;
		case Item::Root_Fossil: return "Root Fossil"_sv;
		case Item::Salac_Berry: return "Salac Berry"_sv;
		case Item::Sea_Incense: return "Sea Incense"_sv;
		case Item::Shadow_Mail: return "Shadow Mail"_sv;
		case Item::Shell_Bell: return "Shell Bell"_sv;
		case Item::Shoal_Salt: return "Shoal Salt"_sv;
		case Item::Shoal_Shell: return "Shoal Shell"_sv;
		case Item::Silk_Scarf: return "Silk Scarf"_sv;
		case Item::Sitrus_Berry: return "Sitrus Berry"_sv;
		case Item::Soothe_Bell: return "Soothe Bell"_sv;
		case Item::Soul_Dew: return "Soul Dew"_sv;
		case Item::Spelon_Berry: return "Spelon Berry"_sv;
		case Item::Starf_Berry: return "Starf Berry"_sv;
		case Item::Strib_Berry: return "Strib Berry"_sv;
		case Item::Tamato_Berry: return "Tamato Berry"_sv;
		case Item::Timer_Ball: return "Timer Ball"_sv;
		case Item::Topo_Berry: return "Topo Berry"_sv;
		case Item::Touga_Berry: return "Touga Berry"_sv;
		case Item::Tropic_Mail: return "Tropic Mail"_sv;
		case Item::Watmel_Berry: return "Watmel Berry"_sv;
		case Item::Wave_Mail: return "Wave Mail"_sv;
		case Item::Wepear_Berry: return "Wepear Berry"_sv;
		case Item::White_Flute: return "White Flute"_sv;
		case Item::White_Herb: return "White Herb"_sv;
		case Item::Wiki_Berry: return "Wiki Berry"_sv;
		case Item::Wood_Mail: return "Wood Mail"_sv;
		case Item::Yago_Berry: return "Yago Berry"_sv;
		case Item::Yellow_Flute: return "Yellow Flute"_sv;
		case Item::Yellow_Scarf: return "Yellow Scarf"_sv;
		case Item::Yellow_Shard: return "Yellow Shard"_sv;
		case Item::Zinc: return "Zinc"_sv;

		// Generation 4
		case Item::Adamant_Orb: return "Adamant Orb"_sv;
		case Item::Air_Mail: return "Air Mail"_sv;
		case Item::Armor_Fossil: return "Armor Fossil"_sv;
		case Item::Babiri_Berry: return "Babiri Berry"_sv;
		case Item::Big_Root: return "Big Root"_sv;
		case Item::Black_Sludge: return "Black Sludge"_sv;
		case Item::Bloom_Mail: return "Bloom Mail"_sv;
		case Item::Brick_Mail: return "Brick Mail"_sv;
		case Item::Bubble_Mail: return "Bubble Mail"_sv;
		case Item::Charti_Berry: return "Charti Berry"_sv;
		case Item::Cherish_Ball: return "Cherish Ball"_sv;
		case Item::Chilan_Berry: return "Chilan Berry"_sv;
		case Item::Choice_Scarf: return "Choice Scarf"_sv;
		case Item::Choice_Specs: return "Choice Specs"_sv;
		case Item::Chople_Berry: return "Chople Berry"_sv;
		case Item::Coba_Berry: return "Coba Berry"_sv;
		case Item::Colbur_Berry: return "Colbur Berry"_sv;
		case Item::Custap_Berry: return "Custap Berry"_sv;
		case Item::Damp_Mulch: return "Damp Mulch"_sv;
		case Item::Damp_Rock: return "Damp Rock"_sv;
		case Item::Dawn_Stone: return "Dawn Stone"_sv;
		case Item::Destiny_Knot: return "Destiny Knot"_sv;
		case Item::Draco_Plate: return "Draco Plate"_sv;
		case Item::Dread_Plate: return "Dread Plate"_sv;
		case Item::Dubious_Disc: return "Dubious Disc"_sv;
		case Item::Dusk_Ball: return "Dusk Ball"_sv;
		case Item::Dusk_Stone: return "Dusk Stone"_sv;
		case Item::Earth_Plate: return "Earth Plate"_sv;
		case Item::Electirizer: return "Electirizer"_sv;
		case Item::Expert_Belt: return "Expert Belt"_sv;
		case Item::Fist_Plate: return "Fist Plate"_sv;
		case Item::Flame_Mail: return "Flame Mail"_sv;
		case Item::Flame_Orb: return "Flame Orb"_sv;
		case Item::Flame_Plate: return "Flame Plate"_sv;
		case Item::Focus_Sash: return "Focus Sash"_sv;
		case Item::Full_Incense: return "Full Incense"_sv;
		case Item::Gooey_Mulch: return "Gooey Mulch"_sv;
		case Item::Grass_Mail: return "Grass Mail"_sv;
		case Item::Grip_Claw: return "Grip Claw"_sv;
		case Item::Griseous_Orb: return "Griseous Orb"_sv;
		case Item::Growth_Mulch: return "Growth Mulch"_sv;
		case Item::Haban_Berry: return "Haban Berry"_sv;
		case Item::Heal_Ball: return "Heal Ball"_sv;
		case Item::Heart_Mail: return "Heart Mail"_sv;
		case Item::Heat_Rock: return "Heat Rock"_sv;
		case Item::Honey: return "Honey"_sv;
		case Item::Icicle_Plate: return "Icicle Plate"_sv;
		case Item::Icy_Rock: return "Icy Rock"_sv;
		case Item::Insect_Plate: return "Insect Plate"_sv;
		case Item::Iron_Ball: return "Iron Ball"_sv;
		case Item::Iron_Plate: return "Iron Plate"_sv;
		case Item::Jaboca_Berry: return "Jaboca Berry"_sv;
		case Item::Kasib_Berry: return "Kasib Berry"_sv;
		case Item::Kebia_Berry: return "Kebia Berry"_sv;
		case Item::Lagging_Tail: return "Lagging Tail"_sv;
		case Item::Life_Orb: return "Life Orb"_sv;
		case Item::Light_Clay: return "Light Clay"_sv;
		case Item::Luck_Incense: return "Luck Incense"_sv;
		case Item::Lustrous_Orb: return "Lustrous Orb"_sv;
		case Item::Magmarizer: return "Magmarizer"_sv;
		case Item::Meadow_Plate: return "Meadow Plate"_sv;
		case Item::Metronome: return "Metronome"_sv;
		case Item::Micle_Berry: return "Micle Berry"_sv;
		case Item::Mind_Plate: return "Mind Plate"_sv;
		case Item::Mosaic_Mail: return "Mosaic Mail"_sv;
		case Item::Muscle_Band: return "Muscle Band"_sv;
		case Item::Occa_Berry: return "Occa Berry"_sv;
		case Item::Odd_Incense: return "Odd Incense"_sv;
		case Item::Odd_Keystone: return "Odd Keystone"_sv;
		case Item::Old_Gateau: return "Old Gateau"_sv;
		case Item::Oval_Stone: return "Oval Stone"_sv;
		case Item::Park_Ball: return "Park Ball"_sv;
		case Item::Passho_Berry: return "Passho Berry"_sv;
		case Item::Payapa_Berry: return "Payapa Berry"_sv;
		case Item::Power_Anklet: return "Power Anklet"_sv;
		case Item::Power_Band: return "Power Band"_sv;
		case Item::Power_Belt: return "Power Belt"_sv;
		case Item::Power_Bracer: return "Power Bracer"_sv;
		case Item::Power_Herb: return "Power Herb"_sv;
		case Item::Power_Lens: return "Power Lens"_sv;
		case Item::Power_Weight: return "Power Weight"_sv;
		case Item::Protector: return "Protector"_sv;
		case Item::Pure_Incense: return "Pure Incense"_sv;
		case Item::Quick_Ball: return "Quick Ball"_sv;
		case Item::Quick_Powder: return "Quick Powder"_sv;
		case Item::Rare_Bone: return "Rare Bone"_sv;
		case Item::Razor_Claw: return "Razor Claw"_sv;
		case Item::Razor_Fang: return "Razor Fang"_sv;
		case Item::Reaper_Cloth: return "Reaper Cloth"_sv;
		case Item::Rindo_Berry: return "Rindo Berry"_sv;
		case Item::Rock_Incense: return "Rock Incense"_sv;
		case Item::Rose_Incense: return "Rose Incense"_sv;
		case Item::Rowap_Berry: return "Rowap Berry"_sv;
		case Item::Shed_Shell: return "Shed Shell"_sv;
		case Item::Shiny_Stone: return "Shiny Stone"_sv;
		case Item::Shuca_Berry: return "Shuca Berry"_sv;
		case Item::Skull_Fossil: return "Skull Fossil"_sv;
		case Item::Sky_Plate: return "Sky Plate"_sv;
		case Item::Smooth_Rock: return "Smooth Rock"_sv;
		case Item::Snow_Mail: return "Snow Mail"_sv;
		case Item::Space_Mail: return "Space Mail"_sv;
		case Item::Splash_Plate: return "Splash Plate"_sv;
		case Item::Spooky_Plate: return "Spooky Plate"_sv;
		case Item::Stable_Mulch: return "Stable Mulch"_sv;
		case Item::Steel_Mail: return "Steel Mail"_sv;
		case Item::Sticky_Barb: return "Sticky Barb"_sv;
		case Item::Stone_Plate: return "Stone Plate"_sv;
		case Item::Tanga_Berry: return "Tanga Berry"_sv;
		case Item::TM51: return "TM51"_sv;
		case Item::TM52: return "TM52"_sv;
		case Item::TM53: return "TM53"_sv;
		case Item::TM54: return "TM54"_sv;
		case Item::TM55: return "TM55"_sv;
		case Item::TM56: return "TM56"_sv;
		case Item::TM57: return "TM57"_sv;
		case Item::TM58: return "TM58"_sv;
		case Item::TM59: return "TM59"_sv;
		case Item::TM60: return "TM60"_sv;
		case Item::TM61: return "TM61"_sv;
		case Item::TM62: return "TM62"_sv;
		case Item::TM63: return "TM63"_sv;
		case Item::TM64: return "TM64"_sv;
		case Item::TM65: return "TM65"_sv;
		case Item::TM66: return "TM66"_sv;
		case Item::TM67: return "TM67"_sv;
		case Item::TM68: return "TM68"_sv;
		case Item::TM69: return "TM69"_sv;
		case Item::TM70: return "TM70"_sv;
		case Item::TM71: return "TM71"_sv;
		case Item::TM72: return "TM72"_sv;
		case Item::TM73: return "TM73"_sv;
		case Item::TM74: return "TM74"_sv;
		case Item::TM75: return "TM75"_sv;
		case Item::TM76: return "TM76"_sv;
		case Item::TM77: return "TM77"_sv;
		case Item::TM78: return "TM78"_sv;
		case Item::TM79: return "TM79"_sv;
		case Item::TM80: return "TM80"_sv;
		case Item::TM81: return "TM81"_sv;
		case Item::TM82: return "TM82"_sv;
		case Item::TM83: return "TM83"_sv;
		case Item::TM84: return "TM84"_sv;
		case Item::TM85: return "TM85"_sv;
		case Item::TM86: return "TM86"_sv;
		case Item::TM87: return "TM87"_sv;
		case Item::TM88: return "TM88"_sv;
		case Item::TM89: return "TM89"_sv;
		case Item::TM90: return "TM90"_sv;
		case Item::TM91: return "TM91"_sv;
		case Item::TM92: return "TM92"_sv;
		case Item::Toxic_Orb: return "Toxic Orb"_sv;
		case Item::Toxic_Plate: return "Toxic Plate"_sv;
		case Item::Tunnel_Mail: return "Tunnel Mail"_sv;
		case Item::Wacan_Berry: return "Wacan Berry"_sv;
		case Item::Wave_Incense: return "Wave Incense"_sv;
		case Item::Wide_Lens: return "Wide Lens"_sv;
		case Item::Wise_Glasses: return "Wise Glasses"_sv;
		case Item::X_Sp_Def: return "X Sp. Def"_sv;
		case Item::Yache_Berry: return "Yache Berry"_sv;
		case Item::Zap_Plate: return "Zap Plate"_sv;
		case Item::Zoom_Lens: return "Zoom Lens"_sv;

		// Generation 5
		case Item::Absorb_Bulb: return "Absorb Bulb"_sv;
		case Item::Air_Balloon: return "Air Balloon"_sv;
		case Item::Balm_Mushroom: return "Balm Mushroom"_sv;
		case Item::Big_Nugget: return "Big Nugget"_sv;
		case Item::Binding_Band: return "Binding Band"_sv;
		case Item::BridgeMail_D: return "BridgeMail D"_sv;
		case Item::BridgeMail_M: return "BridgeMail M"_sv;
		case Item::BridgeMail_S: return "BridgeMail S"_sv;
		case Item::BridgeMail_T: return "BridgeMail T"_sv;
		case Item::BridgeMail_V: return "BridgeMail V"_sv;
		case Item::Bug_Gem: return "Bug Gem"_sv;
		case Item::Burn_Drive: return "Burn Drive"_sv;
		case Item::Casteliacone: return "Casteliacone"_sv;
		case Item::Cell_Battery: return "Cell Battery"_sv;
		case Item::Chill_Drive: return "Chill Drive"_sv;
		case Item::Clever_Wing: return "Clever Wing"_sv;
		case Item::Comet_Shard: return "Comet Shard"_sv;
		case Item::Cover_Fossil: return "Cover Fossil"_sv;
		case Item::Dark_Gem: return "Dark Gem"_sv;
		case Item::Douse_Drive: return "Douse Drive"_sv;
		case Item::Dragon_Gem: return "Dragon Gem"_sv;
		case Item::Dream_Ball: return "Dream Ball"_sv;
		case Item::Eject_Button: return "Eject Button"_sv;
		case Item::Electric_Gem: return "Electric Gem"_sv;
		case Item::Eviolite: return "Eviolite"_sv;
		case Item::Favored_Mail: return "Favored Mail"_sv;
		case Item::Fighting_Gem: return "Fighting Gem"_sv;
		case Item::Fire_Gem: return "Fire Gem"_sv;
		case Item::Float_Stone: return "Float Stone"_sv;
		case Item::Flying_Gem: return "Flying Gem"_sv;
		case Item::Genius_Wing: return "Genius Wing"_sv;
		case Item::Ghost_Gem: return "Ghost Gem"_sv;
		case Item::Grass_Gem: return "Grass Gem"_sv;
		case Item::Greet_Mail: return "Greet Mail"_sv;
		case Item::Ground_Gem: return "Ground Gem"_sv;
		case Item::Health_Wing: return "Health Wing"_sv;
		case Item::Ice_Gem: return "Ice Gem"_sv;
		case Item::Inquiry_Mail: return "Inquiry Mail"_sv;
		case Item::Like_Mail: return "Like Mail"_sv;
		case Item::Muscle_Wing: return "Muscle Wing"_sv;
		case Item::Normal_Gem: return "Normal Gem"_sv;
		case Item::Pass_Orb: return "Pass Orb"_sv;
		case Item::Pearl_String: return "Pearl String"_sv;
		case Item::Plume_Fossil: return "Plume Fossil"_sv;
		case Item::Poison_Gem: return "Poison Gem"_sv;
		case Item::Poke_Toy: return "Poke Toy"_sv;
		case Item::Pretty_Wing: return "Pretty Wing"_sv;
		case Item::Prism_Scale: return "Prism Scale"_sv;
		case Item::Psychic_Gem: return "Psychic Gem"_sv;
		case Item::Red_Card: return "Red Card"_sv;
		case Item::Relic_Band: return "Relic Band"_sv;
		case Item::Relic_Copper: return "Relic Copper"_sv;
		case Item::Relic_Crown: return "Relic Crown"_sv;
		case Item::Relic_Gold: return "Relic Gold"_sv;
		case Item::Relic_Silver: return "Relic Silver"_sv;
		case Item::Relic_Statue: return "Relic Statue"_sv;
		case Item::Relic_Vase: return "Relic Vase"_sv;
		case Item::Reply_Mail: return "Reply Mail"_sv;
		case Item::Resist_Wing: return "Resist Wing"_sv;
		case Item::Ring_Target: return "Ring Target"_sv;
		case Item::Rock_Gem: return "Rock Gem"_sv;
		case Item::Rocky_Helmet: return "Rocky Helmet"_sv;
		case Item::RSVP_Mail: return "RSVP Mail"_sv;
		case Item::Shock_Drive: return "Shock Drive"_sv;
		case Item::Steel_Gem: return "Steel Gem"_sv;
		case Item::Sweet_Heart: return "Sweet Heart"_sv;
		case Item::Swift_Wing: return "Swift Wing"_sv;
		case Item::Thanks_Mail: return "Thanks Mail"_sv;
		case Item::Water_Gem: return "Water Gem"_sv;

		// Generation 6
		case Item::Ability_Capsule: return "Ability Capsule"_sv;
		case Item::Abomasite: return "Abomasite"_sv;
		case Item::Absolite: return "Absolite"_sv;
		case Item::Aerodactylite: return "Aerodactylite"_sv;
		case Item::Aggronite: return "Aggronite"_sv;
		case Item::Alakazite: return "Alakazite"_sv;
		case Item::Altarianite: return "Altarianite"_sv;
		case Item::Amaze_Mulch: return "Amaze Mulch"_sv;
		case Item::Ampharosite: return "Ampharosite"_sv;
		case Item::Assault_Vest: return "Assault Vest"_sv;
		case Item::Audinite: return "Audinite"_sv;
		case Item::Banettite: return "Banettite"_sv;
		case Item::Beedrillite: return "Beedrillite"_sv;
		case Item::Blastoisinite: return "Blastoisinite"_sv;
		case Item::Blazikenite: return "Blazikenite"_sv;
		case Item::Boost_Mulch: return "Boost Mulch"_sv;
		case Item::Cameruptite: return "Cameruptite"_sv;
		case Item::Charizardite_X: return "Charizardite X"_sv;
		case Item::Charizardite_Y: return "Charizardite Y"_sv;
		case Item::Diancite: return "Diancite"_sv;
		case Item::Discount_Coupon: return "Discount Coupon"_sv;
		case Item::Fairy_Gem: return "Fairy Gem"_sv;
		case Item::Galladite: return "Galladite"_sv;
		case Item::Garchompite: return "Garchompite"_sv;
		case Item::Gardevoirite: return "Gardevoirite"_sv;
		case Item::Gengarite: return "Gengarite"_sv;
		case Item::Glalitite: return "Glalitite"_sv;
		case Item::Gyaradosite: return "Gyaradosite"_sv;
		case Item::Heracronite: return "Heracronite"_sv;
		case Item::Houndoominite: return "Houndoominite"_sv;
		case Item::Jaw_Fossil: return "Jaw Fossil"_sv;
		case Item::Kangaskhanite: return "Kangaskhanite"_sv;
		case Item::Kee_Berry: return "Kee Berry"_sv;
		case Item::Latiasite: return "Latiasite"_sv;
		case Item::Latiosite: return "Latiosite"_sv;
		case Item::Lopunnite: return "Lopunnite"_sv;
		case Item::Lucarionite: return "Lucarionite"_sv;
		case Item::Luminous_Moss: return "Luminous Moss"_sv;
		case Item::Lumiose_Galette: return "Lumiose Galette"_sv;
		case Item::Manectite: return "Manectite"_sv;
		case Item::Maranga_Berry: return "Maranga Berry"_sv;
		case Item::Mawilite: return "Mawilite"_sv;
		case Item::Medichamite: return "Medichamite"_sv;
		case Item::Metagrossite: return "Metagrossite"_sv;
		case Item::Mewtwonite_X: return "Mewtwonite X"_sv;
		case Item::Mewtwonite_Y: return "Mewtwonite Y"_sv;
		case Item::Pidgeotite: return "Pidgeotite"_sv;
		case Item::Pinsirite: return "Pinsirite"_sv;
		case Item::Pixie_Plate: return "Pixie Plate"_sv;
		case Item::Rich_Mulch: return "Rich Mulch"_sv;
		case Item::Roseli_Berry: return "Roseli Berry"_sv;
		case Item::Sablenite: return "Sablenite"_sv;
		case Item::Sachet: return "Sachet"_sv;
		case Item::Safety_Goggles: return "Safety Goggles"_sv;
		case Item::Sail_Fossil: return "Sail Fossil"_sv;
		case Item::Salamencite: return "Salamencite"_sv;
		case Item::Sceptilite: return "Sceptilite"_sv;
		case Item::Scizorite: return "Scizorite"_sv;
		case Item::Shalour_Sable: return "Shalour Sable"_sv;
		case Item::Sharpedonite: return "Sharpedonite"_sv;
		case Item::Slowbronite: return "Slowbronite"_sv;
		case Item::Snowball: return "Snowball"_sv;
		case Item::Steelixite: return "Steelixite"_sv;
		case Item::Strange_Souvenir: return "Strange Souvenir"_sv;
		case Item::Surprise_Mulch: return "Surprise Mulch"_sv;
		case Item::Swampertite: return "Swampertite"_sv;
		case Item::Tyranitarite: return "Tyranitarite"_sv;
		case Item::Venusaurite: return "Venusaurite"_sv;
		case Item::Weakness_Policy: return "Weakness Policy"_sv;
		case Item::Whipped_Dream: return "Whipped Dream"_sv;
		case Item::X_Defense: return "X Defense"_sv;
		case Item::X_Sp_Atk: return "X Sp. Atk"_sv;

		// Generation 7
		case Item::Adrenaline_Orb: return "Adrenaline Orb"_sv;
		case Item::Aloraichium_Z: return "Aloraichium Z"_sv;
		case Item::Beast_Ball: return "Beast Ball"_sv;
		case Item::Big_Malasada: return "Big Malasada"_sv;
		case Item::Bottle_Cap: return "Bottle Cap"_sv;
		case Item::Bug_Memory: return "Bug Memory"_sv;
		case Item::Buginium_Z: return "Buginium Z"_sv;
		case Item::Dark_Memory: return "Dark Memory"_sv;
		case Item::Darkinium_Z: return "Darkinium Z"_sv;
		case Item::Decidium_Z: return "Decidium Z"_sv;
		case Item::Dragon_Memory: return "Dragon Memory"_sv;
		case Item::Dragonium_Z: return "Dragonium Z"_sv;
		case Item::Eevium_Z: return "Eevium Z"_sv;
		case Item::Electric_Memory: return "Electric Memory"_sv;
		case Item::Electric_Seed: return "Electric Seed"_sv;
		case Item::Electrium_Z: return "Electrium Z"_sv;
		case Item::Fairium_Z: return "Fairium Z"_sv;
		case Item::Fairy_Memory: return "Fairy Memory"_sv;
		case Item::Festival_Ticket: return "Festival Ticket"_sv;
		case Item::Fighting_Memory: return "Fighting Memory"_sv;
		case Item::Fightinium_Z: return "Fightinium Z"_sv;
		case Item::Fire_Memory: return "Fire Memory"_sv;
		case Item::Firium_Z: return "Firium Z"_sv;
		case Item::Flying_Memory: return "Flying Memory"_sv;
		case Item::Flyinium_Z: return "Flyinium Z"_sv;
		case Item::Ghost_Memory: return "Ghost Memory"_sv;
		case Item::Ghostium_Z: return "Ghostium Z"_sv;
		case Item::Gold_Bottle_Cap: return "Gold Bottle Cap"_sv;
		case Item::Grass_Memory: return "Grass Memory"_sv;
		case Item::Grassium_Z: return "Grassium Z"_sv;
		case Item::Grassy_Seed: return "Grassy Seed"_sv;
		case Item::Ground_Memory: return "Ground Memory"_sv;
		case Item::Groundium_Z: return "Groundium Z"_sv;
		case Item::Ice_Memory: return "Ice Memory"_sv;
		case Item::Ice_Stone: return "Ice Stone"_sv;
		case Item::Icium_Z: return "Icium Z"_sv;
		case Item::Incinium_Z: return "Incinium Z"_sv;
		case Item::Kommonium_Z: return "Kommonium Z"_sv;
		case Item::Lunalium_Z: return "Lunalium Z"_sv;
		case Item::Lycanium_Z: return "Lycanium Z"_sv;
		case Item::Marshadium_Z: return "Marshadium Z"_sv;
		case Item::Mewnium_Z: return "Mewnium Z"_sv;
		case Item::Mimikium_Z: return "Mimikium Z"_sv;
		case Item::Misty_Seed: return "Misty Seed"_sv;
		case Item::Normalium_Z: return "Normalium Z"_sv;
		case Item::Pewter_Crunchies: return "Pewter Crunchies"_sv;
		case Item::Pikanium_Z: return "Pikanium Z"_sv;
		case Item::Pikashunium_Z: return "Pikashunium Z"_sv;
		case Item::Pink_Nectar: return "Pink Nectar"_sv;
		case Item::Poison_Memory: return "Poison Memory"_sv;
		case Item::Poisonium_Z: return "Poisonium Z"_sv;
		case Item::Primarium_Z: return "Primarium Z"_sv;
		case Item::Protective_Pads: return "Protective Pads"_sv;
		case Item::Psychic_Memory: return "Psychic Memory"_sv;
		case Item::Psychic_Seed: return "Psychic Seed"_sv;
		case Item::Psychium_Z: return "Psychium Z"_sv;
		case Item::Purple_Nectar: return "Purple Nectar"_sv;
		case Item::Rock_Memory: return "Rock Memory"_sv;
		case Item::Rockium_Z: return "Rockium Z"_sv;
		case Item::Snorlium_Z: return "Snorlium Z"_sv;
		case Item::Solganium_Z: return "Solganium Z"_sv;
		case Item::Steel_Memory: return "Steel Memory"_sv;
		case Item::Steelium_Z: return "Steelium Z"_sv;
		case Item::Tapunium_Z: return "Tapunium Z"_sv;
		case Item::Terrain_Extender: return "Terrain Extender"_sv;
		case Item::Ultranecrozium_Z: return "Ultranecrozium Z"_sv;
		case Item::Water_Memory: return "Water Memory"_sv;
		case Item::Waterium_Z: return "Waterium Z"_sv;
		case Item::Yellow_Nectar: return "Yellow Nectar"_sv;

		// Generation 8
		case Item::Adamant_Mint: return "Adamant Mint"_sv;
		case Item::Bachs_Food_Tin: return "Bach's Food Tin"_sv;
		case Item::Berry_Sweet: return "Berry Sweet"_sv;
		case Item::Blunder_Policy: return "Blunder Policy"_sv;
		case Item::Bobs_Food_Tin: return "Bob's Food Tin"_sv;
		case Item::Boiled_Egg: return "Boiled Egg"_sv;
		case Item::Bold_Mint: return "Bold Mint"_sv;
		case Item::Brave_Mint: return "Brave Mint"_sv;
		case Item::Bread: return "Bread"_sv;
		case Item::Brittle_Bones: return "Brittle Bones"_sv;
		case Item::Calm_Mint: return "Calm Mint"_sv;
		case Item::Careful_Mint: return "Careful Mint"_sv;
		case Item::Chipped_Pot: return "Chipped Pot"_sv;
		case Item::Clover_Sweet: return "Clover Sweet"_sv;
		case Item::Coconut_Milk: return "Coconut Milk"_sv;
		case Item::Cracked_Pot: return "Cracked Pot"_sv;
		case Item::Eject_Pack: return "Eject Pack"_sv;
		case Item::Fancy_Apple: return "Fancy Apple"_sv;
		case Item::Flower_Sweet: return "Flower Sweet"_sv;
		case Item::Fresh_Cream: return "Fresh Cream"_sv;
		case Item::Fossilized_Bird: return "Fossilized Bird"_sv;
		case Item::Fossilized_Dino: return "Fossilized Dino"_sv;
		case Item::Fossilized_Drake: return "Fossilized Drake"_sv;
		case Item::Fossilized_Fish: return "Fossilized Fish"_sv;
		case Item::Fried_Food: return "Fried Food"_sv;
		case Item::Fruit_Bunch: return "Fruit Bunch"_sv;
		case Item::Gentle_Mint: return "Gentle Mint"_sv;
		case Item::Gigantamix: return "Gigantamix"_sv;
		case Item::Hasty_Mint: return "Hasty Mint"_sv;
		case Item::Heavy_Duty_Boots: return "Heavy-Duty Boots"_sv;
		case Item::Impish_Mint: return "Impish Mint"_sv;
		case Item::Instant_Noodles: return "Instant Noodles"_sv;
		case Item::Jolly_Mint: return "Jolly Mint"_sv;
		case Item::Large_Leek: return "Large Leek"_sv;
		case Item::Lax_Mint: return "Lax Mint"_sv;
		case Item::Leek: return "Leek"_sv;
		case Item::Lonely_Mint: return "Lonely Mint"_sv;
		case Item::Love_Sweet: return "Love Sweet"_sv;
		case Item::Mild_Mint: return "Mild Mint"_sv;
		case Item::Mixed_Mushrooms: return "Mixed Mushrooms"_sv;
		case Item::Modest_Mint: return "Modest Mint"_sv;
		case Item::Moomoo_Cheese: return "Moomoo Cheese"_sv;
		case Item::Naive_Mint: return "Naive Mint"_sv;
		case Item::Naughty_Mint: return "Naughty Mint"_sv;
		case Item::Pack_of_Potatoes: return "Pack of Potatoes"_sv;
		case Item::Packaged_Curry: return "Packaged Curry"_sv;
		case Item::Pasta: return "Pasta"_sv;
		case Item::Precooked_Burger: return "Precooked Burger"_sv;
		case Item::Pungent_Root: return "Pungent Root"_sv;
		case Item::Quiet_Mint: return "Quiet Mint"_sv;
		case Item::Rash_Mint: return "Rash Mint"_sv;
		case Item::Relaxed_Mint: return "Relaxed Mint"_sv;
		case Item::Ribbon_Sweet: return "Ribbon Sweet"_sv;
		case Item::Room_Service: return "Room Service"_sv;
		case Item::Rusted_Shield: return "Rusted Shield"_sv;
		case Item::Rusted_Sword: return "Rusted Sword"_sv;
		case Item::Salad_Mix: return "Salad Mix"_sv;
		case Item::Sassy_Mint: return "Sassy Mint"_sv;
		case Item::Sausages: return "Sausages"_sv;
		case Item::Serious_Mint: return "Serious Mint"_sv;
		case Item::Smoke_Poke_Tail: return "Smoke-Poke Tail"_sv;
		case Item::Spice_Mix: return "Spice Mix"_sv;
		case Item::Star_Sweet: return "Star Sweet"_sv;
		case Item::Strawberry_Sweet: return "Strawberry Sweet"_sv;
		case Item::Sweet_Apple: return "Sweet Apple"_sv;
		case Item::Tart_Apple: return "Tart Apple"_sv;
		case Item::Throat_Spray: return "Throat Spray"_sv;
		case Item::Timid_Mint: return "Timid Mint"_sv;
		case Item::Tin_of_Beans: return "Tin of Beans"_sv;
		case Item::TR00: return "TR00"_sv;
		case Item::TR01: return "TR01"_sv;
		case Item::TR02: return "TR02"_sv;
		case Item::TR03: return "TR03"_sv;
		case Item::TR04: return "TR04"_sv;
		case Item::TR05: return "TR05"_sv;
		case Item::TR06: return "TR06"_sv;
		case Item::TR07: return "TR07"_sv;
		case Item::TR08: return "TR08"_sv;
		case Item::TR09: return "TR09"_sv;
		case Item::TR10: return "TR10"_sv;
		case Item::TR11: return "TR11"_sv;
		case Item::TR12: return "TR12"_sv;
		case Item::TR13: return "TR13"_sv;
		case Item::TR14: return "TR14"_sv;
		case Item::TR15: return "TR15"_sv;
		case Item::TR16: return "TR16"_sv;
		case Item::TR17: return "TR17"_sv;
		case Item::TR18: return "TR18"_sv;
		case Item::TR19: return "TR19"_sv;
		case Item::TR20: return "TR20"_sv;
		case Item::TR21: return "TR21"_sv;
		case Item::TR22: return "TR22"_sv;
		case Item::TR23: return "TR23"_sv;
		case Item::TR24: return "TR24"_sv;
		case Item::TR25: return "TR25"_sv;
		case Item::TR26: return "TR26"_sv;
		case Item::TR27: return "TR27"_sv;
		case Item::TR28: return "TR28"_sv;
		case Item::TR29: return "TR29"_sv;
		case Item::TR30: return "TR30"_sv;
		case Item::TR31: return "TR31"_sv;
		case Item::TR32: return "TR32"_sv;
		case Item::TR33: return "TR33"_sv;
		case Item::TR34: return "TR34"_sv;
		case Item::TR35: return "TR35"_sv;
		case Item::TR36: return "TR36"_sv;
		case Item::TR37: return "TR37"_sv;
		case Item::TR38: return "TR38"_sv;
		case Item::TR39: return "TR39"_sv;
		case Item::TR40: return "TR40"_sv;
		case Item::TR41: return "TR41"_sv;
		case Item::TR42: return "TR42"_sv;
		case Item::TR43: return "TR43"_sv;
		case Item::TR44: return "TR44"_sv;
		case Item::TR45: return "TR45"_sv;
		case Item::TR46: return "TR46"_sv;
		case Item::TR47: return "TR47"_sv;
		case Item::TR48: return "TR48"_sv;
		case Item::TR49: return "TR49"_sv;
		case Item::TR50: return "TR50"_sv;
		case Item::TR51: return "TR51"_sv;
		case Item::TR52: return "TR52"_sv;
		case Item::TR53: return "TR53"_sv;
		case Item::TR54: return "TR54"_sv;
		case Item::TR55: return "TR55"_sv;
		case Item::TR56: return "TR56"_sv;
		case Item::TR57: return "TR57"_sv;
		case Item::TR58: return "TR58"_sv;
		case Item::TR59: return "TR59"_sv;
		case Item::TR60: return "TR60"_sv;
		case Item::TR61: return "TR61"_sv;
		case Item::TR62: return "TR62"_sv;
		case Item::TR63: return "TR63"_sv;
		case Item::TR64: return "TR64"_sv;
		case Item::TR65: return "TR65"_sv;
		case Item::TR66: return "TR66"_sv;
		case Item::TR67: return "TR67"_sv;
		case Item::TR68: return "TR68"_sv;
		case Item::TR69: return "TR69"_sv;
		case Item::TR70: return "TR70"_sv;
		case Item::TR71: return "TR71"_sv;
		case Item::TR72: return "TR72"_sv;
		case Item::TR73: return "TR73"_sv;
		case Item::TR74: return "TR74"_sv;
		case Item::TR75: return "TR75"_sv;
		case Item::TR76: return "TR76"_sv;
		case Item::TR77: return "TR77"_sv;
		case Item::TR78: return "TR78"_sv;
		case Item::TR79: return "TR79"_sv;
		case Item::TR80: return "TR80"_sv;
		case Item::TR81: return "TR81"_sv;
		case Item::TR82: return "TR82"_sv;
		case Item::TR83: return "TR83"_sv;
		case Item::TR84: return "TR84"_sv;
		case Item::TR85: return "TR85"_sv;
		case Item::TR86: return "TR86"_sv;
		case Item::TR87: return "TR87"_sv;
		case Item::TR88: return "TR88"_sv;
		case Item::TR89: return "TR89"_sv;
		case Item::TR90: return "TR90"_sv;
		case Item::TR91: return "TR91"_sv;
		case Item::TR92: return "TR92"_sv;
		case Item::TR93: return "TR93"_sv;
		case Item::TR94: return "TR94"_sv;
		case Item::TR95: return "TR95"_sv;
		case Item::TR96: return "TR96"_sv;
		case Item::TR97: return "TR97"_sv;
		case Item::TR98: return "TR98"_sv;
		case Item::TR99: return "TR99"_sv;
		case Item::Utility_Umbrella: return "Utility Umbrella"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Item {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Item>>({
			{""_sv, Item::None},
			{"abilitycapsule"_sv, Item::Ability_Capsule},
			{"abomasite"_sv, Item::Abomasite},
			{"absolite"_sv, Item::Absolite},
			{"absorbbulb"_sv, Item::Absorb_Bulb},
			{"adamantmint"_sv, Item::Adamant_Mint},
			{"adamantorb"_sv, Item::Adamant_Orb},
			{"adrenalineorb"_sv, Item::Adrenaline_Orb},
			{"aerodactylite"_sv, Item::Aerodactylite},
			{"aggronite"_sv, Item::Aggronite},
			{"aguavberry"_sv, Item::Aguav_Berry},
			{"airballoon"_sv, Item::Air_Balloon},
			{"airmail"_sv, Item::Air_Mail},
			{"alakazite"_sv, Item::Alakazite},
			{"aloraichiumz"_sv, Item::Aloraichium_Z},
			{"altarianite"_sv, Item::Altarianite},
			{"amazemulch"_sv, Item::Amaze_Mulch},
			{"ampharosite"_sv, Item::Ampharosite},
			{"amuletcoin"_sv, Item::Amulet_Coin},
			{"antidote"_sv, Item::Antidote},
			{"apicotberry"_sv, Item::Apicot_Berry},
			{"armorfossil"_sv, Item::Armor_Fossil},
			{"aspearberry"_sv, Item::Aspear_Berry},
			{"assaultvest"_sv, Item::Assault_Vest},
			{"audinite"_sv, Item::Audinite},
			{"awakening"_sv, Item::Awakening},
			{"babiriberry"_sv, Item::Babiri_Berry},
			{"bachsfoodtin"_sv, Item::Bachs_Food_Tin},
			{"balmmushroom"_sv, Item::Balm_Mushroom},
			{"banettite"_sv, Item::Banettite},
			{"beadmail"_sv, Item::Bead_Mail},
			{"beastball"_sv, Item::Beast_Ball},
			{"beedrillite"_sv, Item::Beedrillite},
			{"belueberry"_sv, Item::Belue_Berry},
			{"berry"_sv, Item::Berry},
			{"berryjuice"_sv, Item::Berry_Juice},
			{"berrysweet"_sv, Item::Berry_Sweet},
			{"berserkgene"_sv, Item::Berserk_Gene},
			{"bigmalasada"_sv, Item::Big_Malasada},
			{"bigmushroom"_sv, Item::Big_Mushroom},
			{"bignugget"_sv, Item::Big_Nugget},
			{"bigpearl"_sv, Item::Big_Pearl},
			{"bigroot"_sv, Item::Big_Root},
			{"bindingband"_sv, Item::Binding_Band},
			{"bitterberry"_sv, Item::Bitter_Berry},
			{"blackapricorn"_sv, Item::Black_Apricorn},
			{"blackbelt"_sv, Item::Black_Belt},
			{"blackflute"_sv, Item::Black_Flute},
			{"blackglasses"_sv, Item::Black_Glasses},
			{"blacksludge"_sv, Item::Black_Sludge},
			{"blastoisinite"_sv, Item::Blastoisinite},
			{"blazikenite"_sv, Item::Blazikenite},
			{"bloommail"_sv, Item::Bloom_Mail},
			{"blueapricorn"_sv, Item::Blue_Apricorn},
			{"blueflute"_sv, Item::Blue_Flute},
			{"blueorb"_sv, Item::Blue_Orb},
			{"bluescarf"_sv, Item::Blue_Scarf},
			{"blueshard"_sv, Item::Blue_Shard},
			{"blueskymail"_sv, Item::Bluesky_Mail},
			{"blukberry"_sv, Item::Bluk_Berry},
			{"blunderpolicy"_sv, Item::Blunder_Policy},
			{"bobsfoodtin"_sv, Item::Bobs_Food_Tin},
			{"boiledegg"_sv, Item::Boiled_Egg},
			{"boldmint"_sv, Item::Bold_Mint},
			{"boostmulch"_sv, Item::Boost_Mulch},
			{"bottlecap"_sv, Item::Bottle_Cap},
			{"bravemint"_sv, Item::Brave_Mint},
			{"bread"_sv, Item::Bread},
			{"brickmail"_sv, Item::Brick_Mail},
			{"brickpiece"_sv, Item::Brick_Piece},
			{"bridgemaild"_sv, Item::BridgeMail_D},
			{"bridgemailm"_sv, Item::BridgeMail_M},
			{"bridgemails"_sv, Item::BridgeMail_S},
			{"bridgemailt"_sv, Item::BridgeMail_T},
			{"bridgemailv"_sv, Item::BridgeMail_V},
			{"brightpowder"_sv, Item::Bright_Powder},
			{"brittlebones"_sv, Item::Brittle_Bones},
			{"bubblemail"_sv, Item::Bubble_Mail},
			{"buggem"_sv, Item::Bug_Gem},
			{"buginiumz"_sv, Item::Buginium_Z},
			{"bugmemory"_sv, Item::Bug_Memory},
			{"burndrive"_sv, Item::Burn_Drive},
			{"burnheal"_sv, Item::Burn_Heal},
			{"burntberry"_sv, Item::Burnt_Berry},
			{"calcium"_sv, Item::Calcium},
			{"calmmint"_sv, Item::Calm_Mint},
			{"cameruptite"_sv, Item::Cameruptite},
			{"carbos"_sv, Item::Carbos},
			{"carefulmint"_sv, Item::Careful_Mint},
			{"casteliacone"_sv, Item::Casteliacone},
			{"cellbattery"_sv, Item::Cell_Battery},
			{"charcoal"_sv, Item::Charcoal},
			{"charizarditex"_sv, Item::Charizardite_X},
			{"charizarditey"_sv, Item::Charizardite_Y},
			{"chartiberry"_sv, Item::Charti_Berry},
			{"cheriberry"_sv, Item::Cheri_Berry},
			{"cherishball"_sv, Item::Cherish_Ball},
			{"chestoberry"_sv, Item::Chesto_Berry},
			{"chilanberry"_sv, Item::Chilan_Berry},
			{"chilldrive"_sv, Item::Chill_Drive},
			{"chippedpot"_sv, Item::Chipped_Pot},
			{"choiceband"_sv, Item::Choice_Band},
			{"choicescarf"_sv, Item::Choice_Scarf},
			{"choicespecs"_sv, Item::Choice_Specs},
			{"chopleberry"_sv, Item::Chople_Berry},
			{"clawfossil"_sv, Item::Claw_Fossil},
			{"cleansetag"_sv, Item::Cleanse_Tag},
			{"cleverwing"_sv, Item::Clever_Wing},
			{"cloversweet"_sv, Item::Clover_Sweet},
			{"cobaberry"_sv, Item::Coba_Berry},
			{"coconutmilk"_sv, Item::Coconut_Milk},
			{"colburberry"_sv, Item::Colbur_Berry},
			{"cometshard"_sv, Item::Comet_Shard},
			{"cornnberry"_sv, Item::Cornn_Berry},
			{"coverfossil"_sv, Item::Cover_Fossil},
			{"crackedpot"_sv, Item::Cracked_Pot},
			{"custapberry"_sv, Item::Custap_Berry},
			{"dampmulch"_sv, Item::Damp_Mulch},
			{"damprock"_sv, Item::Damp_Rock},
			{"darkgem"_sv, Item::Dark_Gem},
			{"darkiniumz"_sv, Item::Darkinium_Z},
			{"darkmemory"_sv, Item::Dark_Memory},
			{"dawnstone"_sv, Item::Dawn_Stone},
			{"decidiumz"_sv, Item::Decidium_Z},
			{"deepseascale"_sv, Item::Deep_Sea_Scale},
			{"deepseatooth"_sv, Item::Deep_Sea_Tooth},
			{"destinyknot"_sv, Item::Destiny_Knot},
			{"diancite"_sv, Item::Diancite},
			{"direhit"_sv, Item::Dire_Hit},
			{"discountcoupon"_sv, Item::Discount_Coupon},
			{"diveball"_sv, Item::Dive_Ball},
			{"domefossil"_sv, Item::Dome_Fossil},
			{"dousedrive"_sv, Item::Douse_Drive},
			{"dracoplate"_sv, Item::Draco_Plate},
			{"dragonfang"_sv, Item::Dragon_Fang},
			{"dragongem"_sv, Item::Dragon_Gem},
			{"dragoniumz"_sv, Item::Dragonium_Z},
			{"dragonmemory"_sv, Item::Dragon_Memory},
			{"dragonscale"_sv, Item::Dragon_Scale},
			{"drashberry"_sv, Item::Drash_Berry},
			{"dreadplate"_sv, Item::Dread_Plate},
			{"dreamball"_sv, Item::Dream_Ball},
			{"dreammail"_sv, Item::Dream_Mail},
			{"dubiousdisc"_sv, Item::Dubious_Disc},
			{"durinberry"_sv, Item::Durin_Berry},
			{"duskball"_sv, Item::Dusk_Ball},
			{"duskstone"_sv, Item::Dusk_Stone},
			{"earthplate"_sv, Item::Earth_Plate},
			{"eeviumz"_sv, Item::Eevium_Z},
			{"eggantberry"_sv, Item::Eggant_Berry},
			{"ejectbutton"_sv, Item::Eject_Button},
			{"ejectpack"_sv, Item::Eject_Pack},
			{"electirizer"_sv, Item::Electirizer},
			{"electricgem"_sv, Item::Electric_Gem},
			{"electricmemory"_sv, Item::Electric_Memory},
			{"electricseed"_sv, Item::Electric_Seed},
			{"electriumz"_sv, Item::Electrium_Z},
			{"elixir"_sv, Item::Elixir},
			{"energypowder"_sv, Item::Energy_Powder},
			{"energyroot"_sv, Item::Energy_Root},
			{"enigmaberry"_sv, Item::Enigma_Berry},
			{"eonmail"_sv, Item::Eon_Mail},
			{"escaperope"_sv, Item::Escape_Rope},
			{"ether"_sv, Item::Ether},
			{"everstone"_sv, Item::Everstone},
			{"eviolite"_sv, Item::Eviolite},
			{"expertbelt"_sv, Item::Expert_Belt},
			{"expshare"_sv, Item::Exp_Share},
			{"fabmail"_sv, Item::Fab_Mail},
			{"fairiumz"_sv, Item::Fairium_Z},
			{"fairygem"_sv, Item::Fairy_Gem},
			{"fairymemory"_sv, Item::Fairy_Memory},
			{"fancyapple"_sv, Item::Fancy_Apple},
			{"fastball"_sv, Item::Fast_Ball},
			{"favoredmail"_sv, Item::Favored_Mail},
			{"festivalticket"_sv, Item::Festival_Ticket},
			{"fightinggem"_sv, Item::Fighting_Gem},
			{"fightingmemory"_sv, Item::Fighting_Memory},
			{"fightiniumz"_sv, Item::Fightinium_Z},
			{"figyberry"_sv, Item::Figy_Berry},
			{"firegem"_sv, Item::Fire_Gem},
			{"firememory"_sv, Item::Fire_Memory},
			{"firestone"_sv, Item::Fire_Stone},
			{"firiumz"_sv, Item::Firium_Z},
			{"fistplate"_sv, Item::Fist_Plate},
			{"flamemail"_sv, Item::Flame_Mail},
			{"flameorb"_sv, Item::Flame_Orb},
			{"flameplate"_sv, Item::Flame_Plate},
			{"floatstone"_sv, Item::Float_Stone},
			{"flowermail"_sv, Item::Flower_Mail},
			{"flowersweet"_sv, Item::Flower_Sweet},
			{"fluffytail"_sv, Item::Fluffy_Tail},
			{"flyinggem"_sv, Item::Flying_Gem},
			{"flyingmemory"_sv, Item::Flying_Memory},
			{"flyiniumz"_sv, Item::Flyinium_Z},
			{"focusband"_sv, Item::Focus_Band},
			{"focussash"_sv, Item::Focus_Sash},
			{"fossilizedbird"_sv, Item::Fossilized_Bird},
			{"fossilizeddino"_sv, Item::Fossilized_Dino},
			{"fossilizeddrake"_sv, Item::Fossilized_Drake},
			{"fossilizedfish"_sv, Item::Fossilized_Fish},
			{"freshcream"_sv, Item::Fresh_Cream},
			{"freshwater"_sv, Item::Fresh_Water},
			{"friedfood"_sv, Item::Fried_Food},
			{"friendball"_sv, Item::Friend_Ball},
			{"fruitbunch"_sv, Item::Fruit_Bunch},
			{"fullheal"_sv, Item::Full_Heal},
			{"fullincense"_sv, Item::Full_Incense},
			{"fullrestore"_sv, Item::Full_Restore},
			{"galladite"_sv, Item::Galladite},
			{"ganlonberry"_sv, Item::Ganlon_Berry},
			{"garchompite"_sv, Item::Garchompite},
			{"gardevoirite"_sv, Item::Gardevoirite},
			{"gengarite"_sv, Item::Gengarite},
			{"geniuswing"_sv, Item::Genius_Wing},
			{"gentlemint"_sv, Item::Gentle_Mint},
			{"ghostgem"_sv, Item::Ghost_Gem},
			{"ghostiumz"_sv, Item::Ghostium_Z},
			{"ghostmemory"_sv, Item::Ghost_Memory},
			{"gigantamix"_sv, Item::Gigantamix},
			{"ginemaberry"_sv, Item::Ginema_Berry},
			{"glalitite"_sv, Item::Glalitite},
			{"glittermail"_sv, Item::Glitter_Mail},
			{"goldberry"_sv, Item::Gold_Berry},
			{"goldbottlecap"_sv, Item::Gold_Bottle_Cap},
			{"goldleaf"_sv, Item::Gold_Leaf},
			{"gooeymulch"_sv, Item::Gooey_Mulch},
			{"gorgeousbox"_sv, Item::Gorgeous_Box},
			{"grassgem"_sv, Item::Grass_Gem},
			{"grassiumz"_sv, Item::Grassium_Z},
			{"grassmail"_sv, Item::Grass_Mail},
			{"grassmemory"_sv, Item::Grass_Memory},
			{"grassyseed"_sv, Item::Grassy_Seed},
			{"greatball"_sv, Item::Great_Ball},
			{"greenapricorn"_sv, Item::Green_Apricorn},
			{"greenscarf"_sv, Item::Green_Scarf},
			{"greenshard"_sv, Item::Green_Shard},
			{"greetmail"_sv, Item::Greet_Mail},
			{"grepaberry"_sv, Item::Grepa_Berry},
			{"gripclaw"_sv, Item::Grip_Claw},
			{"griseousorb"_sv, Item::Griseous_Orb},
			{"groundgem"_sv, Item::Ground_Gem},
			{"groundiumz"_sv, Item::Groundium_Z},
			{"groundmemory"_sv, Item::Ground_Memory},
			{"growthmulch"_sv, Item::Growth_Mulch},
			{"guardspec"_sv, Item::Guard_Spec},
			{"gyaradosite"_sv, Item::Gyaradosite},
			{"habanberry"_sv, Item::Haban_Berry},
			{"harbormail"_sv, Item::Harbor_Mail},
			{"hardstone"_sv, Item::Hard_Stone},
			{"hastymint"_sv, Item::Hasty_Mint},
			{"healball"_sv, Item::Heal_Ball},
			{"healpowder"_sv, Item::Heal_Powder},
			{"healthwing"_sv, Item::Health_Wing},
			{"heartmail"_sv, Item::Heart_Mail},
			{"heartscale"_sv, Item::Heart_Scale},
			{"heatrock"_sv, Item::Heat_Rock},
			{"heavyball"_sv, Item::Heavy_Ball},
			{"heavydutyboots"_sv, Item::Heavy_Duty_Boots},
			{"helixfossil"_sv, Item::Helix_Fossil},
			{"heracronite"_sv, Item::Heracronite},
			{"hondewberry"_sv, Item::Hondew_Berry},
			{"honey"_sv, Item::Honey},
			{"houndoominite"_sv, Item::Houndoominite},
			{"hpup"_sv, Item::HP_Up},
			{"hyperpotion"_sv, Item::Hyper_Potion},
			{"iapapaberry"_sv, Item::Iapapa_Berry},
			{"iceberry"_sv, Item::Ice_Berry},
			{"icegem"_sv, Item::Ice_Gem},
			{"iceheal"_sv, Item::Ice_Heal},
			{"icememory"_sv, Item::Ice_Memory},
			{"icestone"_sv, Item::Ice_Stone},
			{"icicleplate"_sv, Item::Icicle_Plate},
			{"iciumz"_sv, Item::Icium_Z},
			{"icyrock"_sv, Item::Icy_Rock},
			{"impishmint"_sv, Item::Impish_Mint},
			{"inciniumz"_sv, Item::Incinium_Z},
			{"inquirymail"_sv, Item::Inquiry_Mail},
			{"insectplate"_sv, Item::Insect_Plate},
			{"instantnoodles"_sv, Item::Instant_Noodles},
			{"iron"_sv, Item::Iron},
			{"ironball"_sv, Item::Iron_Ball},
			{"ironplate"_sv, Item::Iron_Plate},
			{"jabocaberry"_sv, Item::Jaboca_Berry},
			{"jawfossil"_sv, Item::Jaw_Fossil},
			{"jollymint"_sv, Item::Jolly_Mint},
			{"kangaskhanite"_sv, Item::Kangaskhanite},
			{"kasibberry"_sv, Item::Kasib_Berry},
			{"kebiaberry"_sv, Item::Kebia_Berry},
			{"keeberry"_sv, Item::Kee_Berry},
			{"kelpsyberry"_sv, Item::Kelpsy_Berry},
			{"kingsrock"_sv, Item::Kings_Rock},
			{"kommoniumz"_sv, Item::Kommonium_Z},
			{"kuoberry"_sv, Item::Kuo_Berry},
			{"laggingtail"_sv, Item::Lagging_Tail},
			{"lansatberry"_sv, Item::Lansat_Berry},
			{"largeleek"_sv, Item::Large_Leek},
			{"latiasite"_sv, Item::Latiasite},
			{"latiosite"_sv, Item::Latiosite},
			{"lavacookie"_sv, Item::Lava_Cookie},
			{"laxincense"_sv, Item::Lax_Incense},
			{"laxmint"_sv, Item::Lax_Mint},
			{"leafstone"_sv, Item::Leaf_Stone},
			{"leek"_sv, Item::Leek},
			{"leftovers"_sv, Item::Leftovers},
			{"lemonade"_sv, Item::Lemonade},
			{"leppaberry"_sv, Item::Leppa_Berry},
			{"levelball"_sv, Item::Level_Ball},
			{"liechiberry"_sv, Item::Liechi_Berry},
			{"lifeorb"_sv, Item::Life_Orb},
			{"lightball"_sv, Item::Light_Ball},
			{"lightclay"_sv, Item::Light_Clay},
			{"likemail"_sv, Item::Like_Mail},
			{"litebluemail"_sv, Item::Litebluemail},
			{"lonelymint"_sv, Item::Lonely_Mint},
			{"lopunnite"_sv, Item::Lopunnite},
			{"loveball"_sv, Item::Love_Ball},
			{"lovelymail"_sv, Item::Lovely_Mail},
			{"lovesweet"_sv, Item::Love_Sweet},
			{"lucarionite"_sv, Item::Lucarionite},
			{"luckincense"_sv, Item::Luck_Incense},
			{"luckyegg"_sv, Item::Lucky_Egg},
			{"luckypunch"_sv, Item::Lucky_Punch},
			{"lumberry"_sv, Item::Lum_Berry},
			{"luminousmoss"_sv, Item::Luminous_Moss},
			{"lumiosegalette"_sv, Item::Lumiose_Galette},
			{"lunaliumz"_sv, Item::Lunalium_Z},
			{"lureball"_sv, Item::Lure_Ball},
			{"lustrousorb"_sv, Item::Lustrous_Orb},
			{"luxuryball"_sv, Item::Luxury_Ball},
			{"lycaniumz"_sv, Item::Lycanium_Z},
			{"machobrace"_sv, Item::Macho_Brace},
			{"magmarizer"_sv, Item::Magmarizer},
			{"magnet"_sv, Item::Magnet},
			{"magoberry"_sv, Item::Mago_Berry},
			{"magostberry"_sv, Item::Magost_Berry},
			{"mail"_sv, Item::Eon_Mail},
			{"manectite"_sv, Item::Manectite},
			{"marangaberry"_sv, Item::Maranga_Berry},
			{"marshadiumz"_sv, Item::Marshadium_Z},
			{"masterball"_sv, Item::Master_Ball},
			{"mawilite"_sv, Item::Mawilite},
			{"maxelixir"_sv, Item::Max_Elixir},
			{"maxether"_sv, Item::Max_Ether},
			{"maxpotion"_sv, Item::Max_Potion},
			{"maxrepel"_sv, Item::Max_Repel},
			{"maxrevive"_sv, Item::Max_Revive},
			{"meadowplate"_sv, Item::Meadow_Plate},
			{"mechmail"_sv, Item::Mech_Mail},
			{"medichamite"_sv, Item::Medichamite},
			{"mentalherb"_sv, Item::Mental_Herb},
			{"metagrossite"_sv, Item::Metagrossite},
			{"metalcoat"_sv, Item::Metal_Coat},
			{"metalpowder"_sv, Item::Metal_Powder},
			{"metronome"_sv, Item::Metronome},
			{"mewniumz"_sv, Item::Mewnium_Z},
			{"mewtwonitex"_sv, Item::Mewtwonite_X},
			{"mewtwonitey"_sv, Item::Mewtwonite_Y},
			{"micleberry"_sv, Item::Micle_Berry},
			{"mildmint"_sv, Item::Mild_Mint},
			{"mimikiumz"_sv, Item::Mimikium_Z},
			{"mindplate"_sv, Item::Mind_Plate},
			{"mintberry"_sv, Item::Mint_Berry},
			{"miracleberry"_sv, Item::MiracleBerry},
			{"miracleseed"_sv, Item::Miracle_Seed},
			{"miragemail"_sv, Item::Mirage_Mail},
			{"mistyseed"_sv, Item::Misty_Seed},
			{"mixedmushrooms"_sv, Item::Mixed_Mushrooms},
			{"modestmint"_sv, Item::Modest_Mint},
			{"moomoocheese"_sv, Item::Moomoo_Cheese},
			{"moomoomilk"_sv, Item::Moomoo_Milk},
			{"moonball"_sv, Item::Moon_Ball},
			{"moonstone"_sv, Item::Moon_Stone},
			{"morphmail"_sv, Item::Morph_Mail},
			{"mosaicmail"_sv, Item::Mosaic_Mail},
			{"muscleband"_sv, Item::Muscle_Band},
			{"musclewing"_sv, Item::Muscle_Wing},
			{"musicmail"_sv, Item::Music_Mail},
			{"mysteryberry"_sv, Item::MysteryBerry},
			{"mysticwater"_sv, Item::Mystic_Water},
			{"naivemint"_sv, Item::Naive_Mint},
			{"nanabberry"_sv, Item::Nanab_Berry},
			{"naughtymint"_sv, Item::Naughty_Mint},
			{"nestball"_sv, Item::Nest_Ball},
			{"netball"_sv, Item::Net_Ball},
			{"nevermeltice"_sv, Item::Never_Melt_Ice},
			{"ninikuberry"_sv, Item::Niniku_Berry},
			{"nomelberry"_sv, Item::Nomel_Berry},
			{"none"_sv, Item::None},
			{"normalbox"_sv, Item::Normal_Box},
			{"normalgem"_sv, Item::Normal_Gem},
			{"normaliumz"_sv, Item::Normalium_Z},
			{"nugget"_sv, Item::Nugget},
			{"nutpeaberry"_sv, Item::Nutpea_Berry},
			{"occaberry"_sv, Item::Occa_Berry},
			{"oddincense"_sv, Item::Odd_Incense},
			{"oddkeystone"_sv, Item::Odd_Keystone},
			{"oldamber"_sv, Item::Old_Amber},
			{"oldgateau"_sv, Item::Old_Gateau},
			{"oranberry"_sv, Item::Oran_Berry},
			{"orangemail"_sv, Item::Orange_Mail},
			{"ovalstone"_sv, Item::Oval_Stone},
			{"packagedcurry"_sv, Item::Packaged_Curry},
			{"packofpotatoes"_sv, Item::Pack_of_Potatoes},
			{"pamtreberry"_sv, Item::Pamtre_Berry},
			{"paralyzeheal"_sv, Item::Paralyze_Heal},
			{"parkball"_sv, Item::Park_Ball},
			{"parlyzheal"_sv, Item::Paralyze_Heal},
			{"passhoberry"_sv, Item::Passho_Berry},
			{"passorb"_sv, Item::Pass_Orb},
			{"pasta"_sv, Item::Pasta},
			{"payapaberry"_sv, Item::Payapa_Berry},
			{"pearl"_sv, Item::Pearl},
			{"pearlstring"_sv, Item::Pearl_String},
			{"pechaberry"_sv, Item::Pecha_Berry},
			{"persimberry"_sv, Item::Persim_Berry},
			{"petayaberry"_sv, Item::Petaya_Berry},
			{"pewtercrunchies"_sv, Item::Pewter_Crunchies},
			{"pidgeotite"_sv, Item::Pidgeotite},
			{"pikaniumz"_sv, Item::Pikanium_Z},
			{"pikashuniumz"_sv, Item::Pikashunium_Z},
			{"pinapberry"_sv, Item::Pinap_Berry},
			{"pinkapricorn"_sv, Item::Pink_Apricorn},
			{"pinkbow"_sv, Item::Pink_Bow},
			{"pinknectar"_sv, Item::Pink_Nectar},
			{"pinkscarf"_sv, Item::Pink_Scarf},
			{"pinsirite"_sv, Item::Pinsirite},
			{"pixieplate"_sv, Item::Pixie_Plate},
			{"plumefossil"_sv, Item::Plume_Fossil},
			{"poisonbarb"_sv, Item::Poison_Barb},
			{"poisongem"_sv, Item::Poison_Gem},
			{"poisoniumz"_sv, Item::Poisonium_Z},
			{"poisonmemory"_sv, Item::Poison_Memory},
			{"pokeball"_sv, Item::Poke_Ball},
			{"pokedoll"_sv, Item::Poke_Doll},
			{"poketoy"_sv, Item::Poke_Toy},
			{"polkadotbow"_sv, Item::Polkadot_Bow},
			{"pomegberry"_sv, Item::Pomeg_Berry},
			{"portraitmail"_sv, Item::Portraitmail},
			{"potion"_sv, Item::Potion},
			{"poweranklet"_sv, Item::Power_Anklet},
			{"powerband"_sv, Item::Power_Band},
			{"powerbelt"_sv, Item::Power_Belt},
			{"powerbracer"_sv, Item::Power_Bracer},
			{"powerherb"_sv, Item::Power_Herb},
			{"powerlens"_sv, Item::Power_Lens},
			{"powerweight"_sv, Item::Power_Weight},
			{"ppmax"_sv, Item::PP_Max},
			{"ppup"_sv, Item::PP_Up},
			{"precookedburger"_sv, Item::Precooked_Burger},
			{"premierball"_sv, Item::Premier_Ball},
			{"prettywing"_sv, Item::Pretty_Wing},
			{"primariumz"_sv, Item::Primarium_Z},
			{"prismscale"_sv, Item::Prism_Scale},
			{"protectivepads"_sv, Item::Protective_Pads},
			{"protector"_sv, Item::Protector},
			{"protein"_sv, Item::Protein},
			{"przcureberry"_sv, Item::PRZCureBerry},
			{"psncureberry"_sv, Item::PSNCureBerry},
			{"psychicgem"_sv, Item::Psychic_Gem},
			{"psychicmemory"_sv, Item::Psychic_Memory},
			{"psychicseed"_sv, Item::Psychic_Seed},
			{"psychiumz"_sv, Item::Psychium_Z},
			{"pumkinberry"_sv, Item::Pumkin_Berry},
			{"pungentroot"_sv, Item::Pungent_Root},
			{"pureincense"_sv, Item::Pure_Incense},
			{"purplenectar"_sv, Item::Purple_Nectar},
			{"qualotberry"_sv, Item::Qualot_Berry},
			{"quickball"_sv, Item::Quick_Ball},
			{"quickclaw"_sv, Item::Quick_Claw},
			{"quickpowder"_sv, Item::Quick_Powder},
			{"quietmint"_sv, Item::Quiet_Mint},
			{"rabutaberry"_sv, Item::Rabuta_Berry},
			{"ragecandybar"_sv, Item::Rage_Candy_Bar},
			{"rarebone"_sv, Item::Rare_Bone},
			{"rarecandy"_sv, Item::Rare_Candy},
			{"rashmint"_sv, Item::Rash_Mint},
			{"rawstberry"_sv, Item::Rawst_Berry},
			{"razorclaw"_sv, Item::Razor_Claw},
			{"razorfang"_sv, Item::Razor_Fang},
			{"razzberry"_sv, Item::Razz_Berry},
			{"reapercloth"_sv, Item::Reaper_Cloth},
			{"redapricorn"_sv, Item::Red_Apricorn},
			{"redcard"_sv, Item::Red_Card},
			{"redflute"_sv, Item::Red_Flute},
			{"redorb"_sv, Item::Red_Orb},
			{"redscarf"_sv, Item::Red_Scarf},
			{"redshard"_sv, Item::Red_Shard},
			{"relaxedmint"_sv, Item::Relaxed_Mint},
			{"relicband"_sv, Item::Relic_Band},
			{"reliccopper"_sv, Item::Relic_Copper},
			{"reliccrown"_sv, Item::Relic_Crown},
			{"relicgold"_sv, Item::Relic_Gold},
			{"relicsilver"_sv, Item::Relic_Silver},
			{"relicstatue"_sv, Item::Relic_Statue},
			{"relicvase"_sv, Item::Relic_Vase},
			{"repeatball"_sv, Item::Repeat_Ball},
			{"repel"_sv, Item::Repel},
			{"replymail"_sv, Item::Reply_Mail},
			{"resistwing"_sv, Item::Resist_Wing},
			{"retromail"_sv, Item::Retro_Mail},
			{"revivalherb"_sv, Item::Revival_Herb},
			{"revive"_sv, Item::Revive},
			{"ribbonsweet"_sv, Item::Ribbon_Sweet},
			{"richmulch"_sv, Item::Rich_Mulch},
			{"rindoberry"_sv, Item::Rindo_Berry},
			{"ringtarget"_sv, Item::Ring_Target},
			{"rockgem"_sv, Item::Rock_Gem},
			{"rockincense"_sv, Item::Rock_Incense},
			{"rockiumz"_sv, Item::Rockium_Z},
			{"rockmemory"_sv, Item::Rock_Memory},
			{"rockyhelmet"_sv, Item::Rocky_Helmet},
			{"roomservice"_sv, Item::Room_Service},
			{"rootfossil"_sv, Item::Root_Fossil},
			{"roseincense"_sv, Item::Rose_Incense},
			{"roseliberry"_sv, Item::Roseli_Berry},
			{"rowapberry"_sv, Item::Rowap_Berry},
			{"rsvpmail"_sv, Item::RSVP_Mail},
			{"rustedshield"_sv, Item::Rusted_Shield},
			{"rustedsword"_sv, Item::Rusted_Sword},
			{"sablenite"_sv, Item::Sablenite},
			{"sachet"_sv, Item::Sachet},
			{"sacredash"_sv, Item::Sacred_Ash},
			{"safariball"_sv, Item::Safari_Ball},
			{"safetygoggles"_sv, Item::Safety_Goggles},
			{"sailfossil"_sv, Item::Sail_Fossil},
			{"salacberry"_sv, Item::Salac_Berry},
			{"saladmix"_sv, Item::Salad_Mix},
			{"salamencite"_sv, Item::Salamencite},
			{"sassymint"_sv, Item::Sassy_Mint},
			{"sausages"_sv, Item::Sausages},
			{"sceptilite"_sv, Item::Sceptilite},
			{"scizorite"_sv, Item::Scizorite},
			{"scopelens"_sv, Item::Scope_Lens},
			{"seaincense"_sv, Item::Sea_Incense},
			{"seriousmint"_sv, Item::Serious_Mint},
			{"shadowmail"_sv, Item::Shadow_Mail},
			{"shaloursable"_sv, Item::Shalour_Sable},
			{"sharpbeak"_sv, Item::Sharp_Beak},
			{"sharpedonite"_sv, Item::Sharpedonite},
			{"shedshell"_sv, Item::Shed_Shell},
			{"shellbell"_sv, Item::Shell_Bell},
			{"shinystone"_sv, Item::Shiny_Stone},
			{"shoalsalt"_sv, Item::Shoal_Salt},
			{"shoalshell"_sv, Item::Shoal_Shell},
			{"shockdrive"_sv, Item::Shock_Drive},
			{"shucaberry"_sv, Item::Shuca_Berry},
			{"silkscarf"_sv, Item::Silk_Scarf},
			{"silverleaf"_sv, Item::Silver_Leaf},
			{"silverpowder"_sv, Item::Silver_Powder},
			{"sitrusberry"_sv, Item::Sitrus_Berry},
			{"skullfossil"_sv, Item::Skull_Fossil},
			{"skyplate"_sv, Item::Sky_Plate},
			{"slowbronite"_sv, Item::Slowbronite},
			{"slowpoketail"_sv, Item::Slowpoke_Tail},
			{"smokeball"_sv, Item::Smoke_Ball},
			{"smokepoketail"_sv, Item::Smoke_Poke_Tail},
			{"smoothrock"_sv, Item::Smooth_Rock},
			{"snorliumz"_sv, Item::Snorlium_Z},
			{"snowball"_sv, Item::Snowball},
			{"snowmail"_sv, Item::Snow_Mail},
			{"sodapop"_sv, Item::Soda_Pop},
			{"softsand"_sv, Item::Soft_Sand},
			{"solganiumz"_sv, Item::Solganium_Z},
			{"soothebell"_sv, Item::Soothe_Bell},
			{"souldew"_sv, Item::Soul_Dew},
			{"spacemail"_sv, Item::Space_Mail},
			{"spelltag"_sv, Item::Spell_Tag},
			{"spelonberry"_sv, Item::Spelon_Berry},
			{"spicemix"_sv, Item::Spice_Mix},
			{"splashplate"_sv, Item::Splash_Plate},
			{"spookyplate"_sv, Item::Spooky_Plate},
			{"sportball"_sv, Item::Sport_Ball},
			{"stablemulch"_sv, Item::Stable_Mulch},
			{"stardust"_sv, Item::Stardust},
			{"starfberry"_sv, Item::Starf_Berry},
			{"starpiece"_sv, Item::Star_Piece},
			{"starsweet"_sv, Item::Star_Sweet},
			{"steelgem"_sv, Item::Steel_Gem},
			{"steeliumz"_sv, Item::Steelium_Z},
			{"steelixite"_sv, Item::Steelixite},
			{"steelmail"_sv, Item::Steel_Mail},
			{"steelmemory"_sv, Item::Steel_Memory},
			{"stick"_sv, Item::Stick},
			{"stickybarb"_sv, Item::Sticky_Barb},
			{"stoneplate"_sv, Item::Stone_Plate},
			{"strangesouvenir"_sv, Item::Strange_Souvenir},
			{"strawberrysweet"_sv, Item::Strawberry_Sweet},
			{"stribberry"_sv, Item::Strib_Berry},
			{"sunstone"_sv, Item::Sun_Stone},
			{"superpotion"_sv, Item::Super_Potion},
			{"superrepel"_sv, Item::Super_Repel},
			{"surfmail"_sv, Item::Surf_Mail},
			{"surprisemulch"_sv, Item::Surprise_Mulch},
			{"swampertite"_sv, Item::Swampertite},
			{"sweetapple"_sv, Item::Sweet_Apple},
			{"sweetheart"_sv, Item::Sweet_Heart},
			{"swiftwing"_sv, Item::Swift_Wing},
			{"tamatoberry"_sv, Item::Tamato_Berry},
			{"tangaberry"_sv, Item::Tanga_Berry},
			{"tapuniumz"_sv, Item::Tapunium_Z},
			{"tartapple"_sv, Item::Tart_Apple},
			{"terrainextender"_sv, Item::Terrain_Extender},
			{"thanksmail"_sv, Item::Thanks_Mail},
			{"thickclub"_sv, Item::Thick_Club},
			{"throatspray"_sv, Item::Throat_Spray},
			{"thunderstone"_sv, Item::Thunder_Stone},
			{"timerball"_sv, Item::Timer_Ball},
			{"timidmint"_sv, Item::Timid_Mint},
			{"tinofbeans"_sv, Item::Tin_of_Beans},
			{"tinymushroom"_sv, Item::Tiny_Mushroom},
			{"tm01"_sv, Item::TM01},
			{"tm02"_sv, Item::TM02},
			{"tm03"_sv, Item::TM03},
			{"tm04"_sv, Item::TM04},
			{"tm05"_sv, Item::TM05},
			{"tm06"_sv, Item::TM06},
			{"tm07"_sv, Item::TM07},
			{"tm08"_sv, Item::TM08},
			{"tm09"_sv, Item::TM09},
			{"tm10"_sv, Item::TM10},
			{"tm11"_sv, Item::TM11},
			{"tm12"_sv, Item::TM12},
			{"tm13"_sv, Item::TM13},
			{"tm14"_sv, Item::TM14},
			{"tm15"_sv, Item::TM15},
			{"tm16"_sv, Item::TM16},
			{"tm17"_sv, Item::TM17},
			{"tm18"_sv, Item::TM18},
			{"tm19"_sv, Item::TM19},
			{"tm20"_sv, Item::TM20},
			{"tm21"_sv, Item::TM21},
			{"tm22"_sv, Item::TM22},
			{"tm23"_sv, Item::TM23},
			{"tm24"_sv, Item::TM24},
			{"tm25"_sv, Item::TM25},
			{"tm26"_sv, Item::TM26},
			{"tm27"_sv, Item::TM27},
			{"tm28"_sv, Item::TM28},
			{"tm29"_sv, Item::TM29},
			{"tm30"_sv, Item::TM30},
			{"tm31"_sv, Item::TM31},
			{"tm32"_sv, Item::TM32},
			{"tm33"_sv, Item::TM33},
			{"tm34"_sv, Item::TM34},
			{"tm35"_sv, Item::TM35},
			{"tm36"_sv, Item::TM36},
			{"tm37"_sv, Item::TM37},
			{"tm38"_sv, Item::TM38},
			{"tm39"_sv, Item::TM39},
			{"tm40"_sv, Item::TM40},
			{"tm41"_sv, Item::TM41},
			{"tm42"_sv, Item::TM42},
			{"tm43"_sv, Item::TM43},
			{"tm44"_sv, Item::TM44},
			{"tm45"_sv, Item::TM45},
			{"tm46"_sv, Item::TM46},
			{"tm47"_sv, Item::TM47},
			{"tm48"_sv, Item::TM48},
			{"tm49"_sv, Item::TM49},
			{"tm50"_sv, Item::TM50},
			{"tm51"_sv, Item::TM51},
			{"tm52"_sv, Item::TM52},
			{"tm53"_sv, Item::TM53},
			{"tm54"_sv, Item::TM54},
			{"tm55"_sv, Item::TM55},
			{"tm56"_sv, Item::TM56},
			{"tm57"_sv, Item::TM57},
			{"tm58"_sv, Item::TM58},
			{"tm59"_sv, Item::TM59},
			{"tm60"_sv, Item::TM60},
			{"tm61"_sv, Item::TM61},
			{"tm62"_sv, Item::TM62},
			{"tm63"_sv, Item::TM63},
			{"tm64"_sv, Item::TM64},
			{"tm65"_sv, Item::TM65},
			{"tm66"_sv, Item::TM66},
			{"tm67"_sv, Item::TM67},
			{"tm68"_sv, Item::TM68},
			{"tm69"_sv, Item::TM69},
			{"tm70"_sv, Item::TM70},
			{"tm71"_sv, Item::TM71},
			{"tm72"_sv, Item::TM72},
			{"tm73"_sv, Item::TM73},
			{"tm74"_sv, Item::TM74},
			{"tm75"_sv, Item::TM75},
			{"tm76"_sv, Item::TM76},
			{"tm77"_sv, Item::TM77},
			{"tm78"_sv, Item::TM78},
			{"tm79"_sv, Item::TM79},
			{"tm80"_sv, Item::TM80},
			{"tm81"_sv, Item::TM81},
			{"tm82"_sv, Item::TM82},
			{"tm83"_sv, Item::TM83},
			{"tm84"_sv, Item::TM84},
			{"tm85"_sv, Item::TM85},
			{"tm86"_sv, Item::TM86},
			{"tm87"_sv, Item::TM87},
			{"tm88"_sv, Item::TM88},
			{"tm89"_sv, Item::TM89},
			{"tm90"_sv, Item::TM90},
			{"tm91"_sv, Item::TM91},
			{"tm92"_sv, Item::TM92},
			{"topoberry"_sv, Item::Topo_Berry},
			{"tougaberry"_sv, Item::Touga_Berry},
			{"toxicorb"_sv, Item::Toxic_Orb},
			{"toxicplate"_sv, Item::Toxic_Plate},
			{"tr00"_sv, Item::TR00},
			{"tr01"_sv, Item::TR01},
			{"tr02"_sv, Item::TR02},
			{"tr03"_sv, Item::TR03},
			{"tr04"_sv, Item::TR04},
			{"tr05"_sv, Item::TR05},
			{"tr06"_sv, Item::TR06},
			{"tr07"_sv, Item::TR07},
			{"tr08"_sv, Item::TR08},
			{"tr09"_sv, Item::TR09},
			{"tr10"_sv, Item::TR10},
			{"tr11"_sv, Item::TR11},
			{"tr12"_sv, Item::TR12},
			{"tr13"_sv, Item::TR13},
			{"tr14"_sv, Item::TR14},
			{"tr15"_sv, Item::TR15},
			{"tr16"_sv, Item::TR16},
			{"tr17"_sv, Item::TR17},
			{"tr18"_sv, Item::TR18},
			{"tr19"_sv, Item::TR19},
			{"tr20"_sv, Item::TR20},
			{"tr21"_sv, Item::TR21},
			{"tr22"_sv, Item::TR22},
			{"tr23"_sv, Item::TR23},
			{"tr24"_sv, Item::TR24},
			{"tr25"_sv, Item::TR25},
			{"tr26"_sv, Item::TR26},
			{"tr27"_sv, Item::TR27},
			{"tr28"_sv, Item::TR28},
			{"tr29"_sv, Item::TR29},
			{"tr30"_sv, Item::TR30},
			{"tr31"_sv, Item::TR31},
			{"tr32"_sv, Item::TR32},
			{"tr33"_sv, Item::TR33},
			{"tr34"_sv, Item::TR34},
			{"tr35"_sv, Item::TR35},
			{"tr36"_sv, Item::TR36},
			{"tr37"_sv, Item::TR37},
			{"tr38"_sv, Item::TR38},
			{"tr39"_sv, Item::TR39},
			{"tr40"_sv, Item::TR40},
			{"tr41"_sv, Item::TR41},
			{"tr42"_sv, Item::TR42},
			{"tr43"_sv, Item::TR43},
			{"tr44"_sv, Item::TR44},
			{"tr45"_sv, Item::TR45},
			{"tr46"_sv, Item::TR46},
			{"tr47"_sv, Item::TR47},
			{"tr48"_sv, Item::TR48},
			{"tr49"_sv, Item::TR49},
			{"tr50"_sv, Item::TR50},
			{"tr51"_sv, Item::TR51},
			{"tr52"_sv, Item::TR52},
			{"tr53"_sv, Item::TR53},
			{"tr54"_sv, Item::TR54},
			{"tr55"_sv, Item::TR55},
			{"tr56"_sv, Item::TR56},
			{"tr57"_sv, Item::TR57},
			{"tr58"_sv, Item::TR58},
			{"tr59"_sv, Item::TR59},
			{"tr60"_sv, Item::TR60},
			{"tr61"_sv, Item::TR61},
			{"tr62"_sv, Item::TR62},
			{"tr63"_sv, Item::TR63},
			{"tr64"_sv, Item::TR64},
			{"tr65"_sv, Item::TR65},
			{"tr66"_sv, Item::TR66},
			{"tr67"_sv, Item::TR67},
			{"tr68"_sv, Item::TR68},
			{"tr69"_sv, Item::TR69},
			{"tr70"_sv, Item::TR70},
			{"tr71"_sv, Item::TR71},
			{"tr72"_sv, Item::TR72},
			{"tr73"_sv, Item::TR73},
			{"tr74"_sv, Item::TR74},
			{"tr75"_sv, Item::TR75},
			{"tr76"_sv, Item::TR76},
			{"tr77"_sv, Item::TR77},
			{"tr78"_sv, Item::TR78},
			{"tr79"_sv, Item::TR79},
			{"tr80"_sv, Item::TR80},
			{"tr81"_sv, Item::TR81},
			{"tr82"_sv, Item::TR82},
			{"tr83"_sv, Item::TR83},
			{"tr84"_sv, Item::TR84},
			{"tr85"_sv, Item::TR85},
			{"tr86"_sv, Item::TR86},
			{"tr87"_sv, Item::TR87},
			{"tr88"_sv, Item::TR88},
			{"tr89"_sv, Item::TR89},
			{"tr90"_sv, Item::TR90},
			{"tr91"_sv, Item::TR91},
			{"tr92"_sv, Item::TR92},
			{"tr93"_sv, Item::TR93},
			{"tr94"_sv, Item::TR94},
			{"tr95"_sv, Item::TR95},
			{"tr96"_sv, Item::TR96},
			{"tr97"_sv, Item::TR97},
			{"tr98"_sv, Item::TR98},
			{"tr99"_sv, Item::TR99},
			{"tropicmail"_sv, Item::Tropic_Mail},
			{"tunnelmail"_sv, Item::Tunnel_Mail},
			{"twistedspoon"_sv, Item::Twisted_Spoon},
			{"tyranitarite"_sv, Item::Tyranitarite},
			{"ultraball"_sv, Item::Ultra_Ball},
			{"ultranecroziumz"_sv, Item::Ultranecrozium_Z},
			{"upgrade"_sv, Item::Up_Grade},
			{"utilityumbrella"_sv, Item::Utility_Umbrella},
			{"venusaurite"_sv, Item::Venusaurite},
			{"wacanberry"_sv, Item::Wacan_Berry},
			{"watergem"_sv, Item::Water_Gem},
			{"wateriumz"_sv, Item::Waterium_Z},
			{"watermemory"_sv, Item::Water_Memory},
			{"waterstone"_sv, Item::Water_Stone},
			{"watmelberry"_sv, Item::Watmel_Berry},
			{"waveincense"_sv, Item::Wave_Incense},
			{"wavemail"_sv, Item::Wave_Mail},
			{"weaknesspolicy"_sv, Item::Weakness_Policy},
			{"wepearberry"_sv, Item::Wepear_Berry},
			{"whippeddream"_sv, Item::Whipped_Dream},
			{"whiteapricorn"_sv, Item::White_Apricorn},
			{"whiteflute"_sv, Item::White_Flute},
			{"whiteherb"_sv, Item::White_Herb},
			{"widelens"_sv, Item::Wide_Lens},
			{"wikiberry"_sv, Item::Wiki_Berry},
			{"wiseglasses"_sv, Item::Wise_Glasses},
			{"woodmail"_sv, Item::Wood_Mail},
			{"xaccuracy"_sv, Item::X_Accuracy},
			{"xattack"_sv, Item::X_Attack},
			{"xdefend"_sv, Item::X_Defend},
			{"xdefense"_sv, Item::X_Defense},
			{"xspatk"_sv, Item::X_Sp_Atk},
			{"xspdef"_sv, Item::X_Sp_Def},
			{"xspecial"_sv, Item::X_Special},
			{"xspeed"_sv, Item::X_Speed},
			{"yacheberry"_sv, Item::Yache_Berry},
			{"yagoberry"_sv, Item::Yago_Berry},
			{"yellowapricorn"_sv, Item::Yellow_Apricorn},
			{"yellowflute"_sv, Item::Yellow_Flute},
			{"yellownectar"_sv, Item::Yellow_Nectar},
			{"yellowscarf"_sv, Item::Yellow_Scarf},
			{"yellowshard"_sv, Item::Yellow_Shard},
			{"zapplate"_sv, Item::Zap_Plate},
			{"zinc"_sv, Item::Zinc},
			{"zoomlens"_sv, Item::Zoom_Lens},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<15_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Item"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
