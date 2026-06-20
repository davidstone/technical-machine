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
		case Item::None: return "None"_s;

		case Item::Antidote: return "Antidote"_s;
		case Item::Awakening: return "Awakening"_s;
		case Item::Burn_Heal: return "Burn Heal"_s;
		case Item::Calcium: return "Calcium"_s;
		case Item::Carbos: return "Carbos"_s;
		case Item::Dire_Hit: return "Dire Hit"_s;
		case Item::Dome_Fossil: return "Dome Fossil"_s;
		case Item::Elixir: return "Elixir"_s;
		case Item::Escape_Rope: return "Escape Rope"_s;
		case Item::Ether: return "Ether"_s;
		case Item::Exp_Share: return "Exp. Share"_s;
		case Item::Fire_Stone: return "Fire Stone"_s;
		case Item::Fresh_Water: return "Fresh Water"_s;
		case Item::Full_Heal: return "Full Heal"_s;
		case Item::Full_Restore: return "Full Restore"_s;
		case Item::Great_Ball: return "Great Ball"_s;
		case Item::Guard_Spec: return "Guard Spec."_s;
		case Item::Helix_Fossil: return "Helix Fossil"_s;
		case Item::HP_Up: return "HP Up"_s;
		case Item::Hyper_Potion: return "Hyper Potion"_s;
		case Item::Ice_Heal: return "Ice Heal"_s;
		case Item::Iron: return "Iron"_s;
		case Item::Leaf_Stone: return "Leaf Stone"_s;
		case Item::Lemonade: return "Lemonade"_s;
		case Item::Master_Ball: return "Master Ball"_s;
		case Item::Max_Elixir: return "Max Elixir"_s;
		case Item::Max_Ether: return "Max Ether"_s;
		case Item::Max_Potion: return "Max Potion"_s;
		case Item::Max_Repel: return "Max Repel"_s;
		case Item::Max_Revive: return "Max Revive"_s;
		case Item::Moon_Stone: return "Moon Stone"_s;
		case Item::Nugget: return "Nugget"_s;
		case Item::Old_Amber: return "Old Amber"_s;
		case Item::Paralyze_Heal: return "Paralyze Heal"_s;
		case Item::Poke_Ball: return "Poke Ball"_s;
		case Item::Poke_Doll: return "Poke Doll"_s;
		case Item::Potion: return "Potion"_s;
		case Item::PP_Up: return "PP Up"_s;
		case Item::Protein: return "Protein"_s;
		case Item::Rare_Candy: return "Rare Candy"_s;
		case Item::Repel: return "Repel"_s;
		case Item::Revive: return "Revive"_s;
		case Item::Safari_Ball: return "Safari Ball"_s;
		case Item::Soda_Pop: return "Soda Pop"_s;
		case Item::Super_Potion: return "Super Potion"_s;
		case Item::Super_Repel: return "Super Repel"_s;
		case Item::Thunder_Stone: return "Thunder Stone"_s;
		case Item::TM01: return "TM01"_s;
		case Item::TM02: return "TM02"_s;
		case Item::TM03: return "TM03"_s;
		case Item::TM04: return "TM04"_s;
		case Item::TM05: return "TM05"_s;
		case Item::TM06: return "TM06"_s;
		case Item::TM07: return "TM07"_s;
		case Item::TM08: return "TM08"_s;
		case Item::TM09: return "TM09"_s;
		case Item::TM10: return "TM10"_s;
		case Item::TM11: return "TM11"_s;
		case Item::TM12: return "TM12"_s;
		case Item::TM13: return "TM13"_s;
		case Item::TM14: return "TM14"_s;
		case Item::TM15: return "TM15"_s;
		case Item::TM16: return "TM16"_s;
		case Item::TM17: return "TM17"_s;
		case Item::TM18: return "TM18"_s;
		case Item::TM19: return "TM19"_s;
		case Item::TM20: return "TM20"_s;
		case Item::TM21: return "TM21"_s;
		case Item::TM22: return "TM22"_s;
		case Item::TM23: return "TM23"_s;
		case Item::TM24: return "TM24"_s;
		case Item::TM25: return "TM25"_s;
		case Item::TM26: return "TM26"_s;
		case Item::TM27: return "TM27"_s;
		case Item::TM28: return "TM28"_s;
		case Item::TM29: return "TM29"_s;
		case Item::TM30: return "TM30"_s;
		case Item::TM31: return "TM31"_s;
		case Item::TM32: return "TM32"_s;
		case Item::TM33: return "TM33"_s;
		case Item::TM34: return "TM34"_s;
		case Item::TM35: return "TM35"_s;
		case Item::TM36: return "TM36"_s;
		case Item::TM37: return "TM37"_s;
		case Item::TM38: return "TM38"_s;
		case Item::TM39: return "TM39"_s;
		case Item::TM40: return "TM40"_s;
		case Item::TM41: return "TM41"_s;
		case Item::TM42: return "TM42"_s;
		case Item::TM43: return "TM43"_s;
		case Item::TM44: return "TM44"_s;
		case Item::TM45: return "TM45"_s;
		case Item::TM46: return "TM46"_s;
		case Item::TM47: return "TM47"_s;
		case Item::TM48: return "TM48"_s;
		case Item::TM49: return "TM49"_s;
		case Item::TM50: return "TM50"_s;
		case Item::Ultra_Ball: return "Ultra Ball"_s;
		case Item::Water_Stone: return "Water Stone"_s;
		case Item::X_Accuracy: return "X Accuracy"_s;
		case Item::X_Attack: return "X Attack"_s;
		case Item::X_Defend: return "X Defend"_s;
		case Item::X_Special: return "X Special"_s;
		case Item::X_Speed: return "X Speed"_s;

		// Generation 2
		case Item::Amulet_Coin: return "Amulet Coin"_s;
		case Item::Berry_Juice: return "Berry Juice"_s;
		case Item::Berry: return "Berry"_s;
		case Item::Berserk_Gene: return "Berserk Gene"_s;
		case Item::Big_Mushroom: return "Big Mushroom"_s;
		case Item::Big_Pearl: return "Big Pearl"_s;
		case Item::Bitter_Berry: return "Bitter Berry"_s;
		case Item::Black_Apricorn: return "Black Apricorn"_s;
		case Item::Black_Belt: return "Black Belt"_s;
		case Item::Black_Glasses: return "Black Glasses"_s;
		case Item::Blue_Apricorn: return "Blue Apricorn"_s;
		case Item::Bluesky_Mail: return "Bluesky Mail"_s;
		case Item::Brick_Piece: return "Brick Piece"_s;
		case Item::Bright_Powder: return "Bright Powder"_s;
		case Item::Burnt_Berry: return "Burnt Berry"_s;
		case Item::Charcoal: return "Charcoal"_s;
		case Item::Cleanse_Tag: return "Cleanse Tag"_s;
		case Item::Dragon_Fang: return "Dragon Fang"_s;
		case Item::Dragon_Scale: return "Dragon Scale"_s;
		case Item::Energy_Powder: return "Energy Powder"_s;
		case Item::Energy_Root: return "Energy Root"_s;
		case Item::Eon_Mail: return "Eon Mail"_s;
		case Item::Everstone: return "Everstone"_s;
		case Item::Fast_Ball: return "Fast Ball"_s;
		case Item::Flower_Mail: return "Flower Mail"_s;
		case Item::Focus_Band: return "Focus Band"_s;
		case Item::Friend_Ball: return "Friend Ball"_s;
		case Item::Gold_Berry: return "Gold Berry"_s;
		case Item::Gold_Leaf: return "Gold Leaf"_s;
		case Item::Gorgeous_Box: return "Gorgeous Box"_s;
		case Item::Green_Apricorn: return "Green Apricorn"_s;
		case Item::Hard_Stone: return "Hard Stone"_s;
		case Item::Heal_Powder: return "Heal Powder"_s;
		case Item::Heavy_Ball: return "Heavy Ball"_s;
		case Item::Ice_Berry: return "Ice Berry"_s;
		case Item::Kings_Rock: return "King's Rock"_s;
		case Item::Stick: return "Stick"_s;
		case Item::Leftovers: return "Leftovers"_s;
		case Item::Level_Ball: return "Level Ball"_s;
		case Item::Light_Ball: return "Light Ball"_s;
		case Item::Litebluemail: return "Litebluemail"_s;
		case Item::Love_Ball: return "Love Ball"_s;
		case Item::Lovely_Mail: return "Lovely Mail"_s;
		case Item::Lucky_Egg: return "Lucky Egg"_s;
		case Item::Lucky_Punch: return "Lucky Punch"_s;
		case Item::Lure_Ball: return "Lure Ball"_s;
		case Item::Magnet: return "Magnet"_s;
		case Item::Metal_Coat: return "Metal Coat"_s;
		case Item::Metal_Powder: return "Metal Powder"_s;
		case Item::Mint_Berry: return "Mint Berry"_s;
		case Item::Miracle_Seed: return "Miracle Seed"_s;
		case Item::MiracleBerry: return "MiracleBerry"_s;
		case Item::Mirage_Mail: return "Mirage Mail"_s;
		case Item::Moomoo_Milk: return "Moomoo Milk"_s;
		case Item::Moon_Ball: return "Moon Ball"_s;
		case Item::Morph_Mail: return "Morph Mail"_s;
		case Item::Music_Mail: return "Music Mail"_s;
		case Item::MysteryBerry: return "MysteryBerry"_s;
		case Item::Mystic_Water: return "Mystic Water"_s;
		case Item::Never_Melt_Ice: return "Never-Melt Ice"_s;
		case Item::Normal_Box: return "Normal Box"_s;
		case Item::Pearl: return "Pearl"_s;
		case Item::Pink_Apricorn: return "Pink Apricorn"_s;
		case Item::Pink_Bow: return "Pink Bow"_s;
		case Item::Poison_Barb: return "Poison Barb"_s;
		case Item::Polkadot_Bow: return "Polkadot Bow"_s;
		case Item::Portraitmail: return "Portraitmail"_s;
		case Item::PRZCureBerry: return "PRZCureBerry"_s;
		case Item::PSNCureBerry: return "PSNCureBerry"_s;
		case Item::Quick_Claw: return "Quick Claw"_s;
		case Item::Rage_Candy_Bar: return "Rage Candy Bar"_s;
		case Item::Red_Apricorn: return "Red Apricorn"_s;
		case Item::Revival_Herb: return "Revival Herb"_s;
		case Item::Sacred_Ash: return "Sacred Ash"_s;
		case Item::Scope_Lens: return "Scope Lens"_s;
		case Item::Sharp_Beak: return "Sharp Beak"_s;
		case Item::Silver_Leaf: return "Silver Leaf"_s;
		case Item::Silver_Powder: return "Silver Powder"_s;
		case Item::Slowpoke_Tail: return "Slowpoke Tail"_s;
		case Item::Smoke_Ball: return "Smoke Ball"_s;
		case Item::Soft_Sand: return "Soft Sand"_s;
		case Item::Spell_Tag: return "Spell Tag"_s;
		case Item::Sport_Ball: return "Sport Ball"_s;
		case Item::Star_Piece: return "Star Piece"_s;
		case Item::Stardust: return "Stardust"_s;
		case Item::Sun_Stone: return "Sun Stone"_s;
		case Item::Surf_Mail: return "Surf Mail"_s;
		case Item::Thick_Club: return "Thick Club"_s;
		case Item::Tiny_Mushroom: return "Tiny Mushroom"_s;
		case Item::Twisted_Spoon: return "Twisted Spoon"_s;
		case Item::Up_Grade: return "Up-Grade"_s;
		case Item::White_Apricorn: return "White Apricorn"_s;
		case Item::Yellow_Apricorn: return "Yellow Apricorn"_s;

		// Generation 3
		case Item::Aguav_Berry: return "Aguav Berry"_s;
		case Item::Apicot_Berry: return "Apicot Berry"_s;
		case Item::Aspear_Berry: return "Aspear Berry"_s;
		case Item::Bead_Mail: return "Bead Mail"_s;
		case Item::Belue_Berry: return "Belue Berry"_s;
		case Item::Black_Flute: return "Black Flute"_s;
		case Item::Blue_Flute: return "Blue Flute"_s;
		case Item::Blue_Orb: return "Blue Orb"_s;
		case Item::Blue_Scarf: return "Blue Scarf"_s;
		case Item::Blue_Shard: return "Blue Shard"_s;
		case Item::Bluk_Berry: return "Bluk Berry"_s;
		case Item::Cheri_Berry: return "Cheri Berry"_s;
		case Item::Chesto_Berry: return "Chesto Berry"_s;
		case Item::Choice_Band: return "Choice Band"_s;
		case Item::Claw_Fossil: return "Claw Fossil"_s;
		case Item::Cornn_Berry: return "Cornn Berry"_s;
		case Item::Deep_Sea_Scale: return "Deep Sea Scale"_s;
		case Item::Deep_Sea_Tooth: return "Deep Sea Tooth"_s;
		case Item::Dive_Ball: return "Dive Ball"_s;
		case Item::Drash_Berry: return "Drash Berry"_s;
		case Item::Dream_Mail: return "Dream Mail"_s;
		case Item::Durin_Berry: return "Durin Berry"_s;
		case Item::Eggant_Berry: return "Eggant Berry"_s;
		case Item::Enigma_Berry: return "Enigma Berry"_s;
		case Item::Fab_Mail: return "Fab Mail"_s;
		case Item::Figy_Berry: return "Figy Berry"_s;
		case Item::Fluffy_Tail: return "Fluffy Tail"_s;
		case Item::Ganlon_Berry: return "Ganlon Berry"_s;
		case Item::Ginema_Berry: return "Ginema Berry"_s;
		case Item::Glitter_Mail: return "Glitter Mail"_s;
		case Item::Green_Scarf: return "Green Scarf"_s;
		case Item::Green_Shard: return "Green Shard"_s;
		case Item::Grepa_Berry: return "Grepa Berry"_s;
		case Item::Harbor_Mail: return "Harbor Mail"_s;
		case Item::Heart_Scale: return "Heart Scale"_s;
		case Item::Hondew_Berry: return "Hondew Berry"_s;
		case Item::Iapapa_Berry: return "Iapapa Berry"_s;
		case Item::Kelpsy_Berry: return "Kelpsy Berry"_s;
		case Item::Kuo_Berry: return "Kuo Berry"_s;
		case Item::Lansat_Berry: return "Lansat Berry"_s;
		case Item::Lava_Cookie: return "Lava Cookie"_s;
		case Item::Lax_Incense: return "Lax Incense"_s;
		case Item::Leppa_Berry: return "Leppa Berry"_s;
		case Item::Liechi_Berry: return "Liechi Berry"_s;
		case Item::Lum_Berry: return "Lum Berry"_s;
		case Item::Luxury_Ball: return "Luxury Ball"_s;
		case Item::Macho_Brace: return "Macho Brace"_s;
		case Item::Mago_Berry: return "Mago Berry"_s;
		case Item::Magost_Berry: return "Magost Berry"_s;
		case Item::Mech_Mail: return "Mech Mail"_s;
		case Item::Mental_Herb: return "Mental Herb"_s;
		case Item::Nanab_Berry: return "Nanab Berry"_s;
		case Item::Nest_Ball: return "Nest Ball"_s;
		case Item::Net_Ball: return "Net Ball"_s;
		case Item::Niniku_Berry: return "Niniku Berry"_s;
		case Item::Nomel_Berry: return "Nomel Berry"_s;
		case Item::Nutpea_Berry: return "Nutpea Berry"_s;
		case Item::Oran_Berry: return "Oran Berry"_s;
		case Item::Orange_Mail: return "Orange Mail"_s;
		case Item::Pamtre_Berry: return "Pamtre Berry"_s;
		case Item::Pecha_Berry: return "Pecha Berry"_s;
		case Item::Persim_Berry: return "Persim Berry"_s;
		case Item::Petaya_Berry: return "Petaya Berry"_s;
		case Item::Pinap_Berry: return "Pinap Berry"_s;
		case Item::Pink_Scarf: return "Pink Scarf"_s;
		case Item::Pomeg_Berry: return "Pomeg Berry"_s;
		case Item::PP_Max: return "PP Max"_s;
		case Item::Premier_Ball: return "Premier Ball"_s;
		case Item::Pumkin_Berry: return "Pumkin Berry"_s;
		case Item::Qualot_Berry: return "Qualot Berry"_s;
		case Item::Rabuta_Berry: return "Rabuta Berry"_s;
		case Item::Rawst_Berry: return "Rawst Berry"_s;
		case Item::Razz_Berry: return "Razz Berry"_s;
		case Item::Red_Flute: return "Red Flute"_s;
		case Item::Red_Orb: return "Red Orb"_s;
		case Item::Red_Scarf: return "Red Scarf"_s;
		case Item::Red_Shard: return "Red Shard"_s;
		case Item::Repeat_Ball: return "Repeat Ball"_s;
		case Item::Retro_Mail: return "Retro Mail"_s;
		case Item::Root_Fossil: return "Root Fossil"_s;
		case Item::Salac_Berry: return "Salac Berry"_s;
		case Item::Sea_Incense: return "Sea Incense"_s;
		case Item::Shadow_Mail: return "Shadow Mail"_s;
		case Item::Shell_Bell: return "Shell Bell"_s;
		case Item::Shoal_Salt: return "Shoal Salt"_s;
		case Item::Shoal_Shell: return "Shoal Shell"_s;
		case Item::Silk_Scarf: return "Silk Scarf"_s;
		case Item::Sitrus_Berry: return "Sitrus Berry"_s;
		case Item::Soothe_Bell: return "Soothe Bell"_s;
		case Item::Soul_Dew: return "Soul Dew"_s;
		case Item::Spelon_Berry: return "Spelon Berry"_s;
		case Item::Starf_Berry: return "Starf Berry"_s;
		case Item::Strib_Berry: return "Strib Berry"_s;
		case Item::Tamato_Berry: return "Tamato Berry"_s;
		case Item::Timer_Ball: return "Timer Ball"_s;
		case Item::Topo_Berry: return "Topo Berry"_s;
		case Item::Touga_Berry: return "Touga Berry"_s;
		case Item::Tropic_Mail: return "Tropic Mail"_s;
		case Item::Watmel_Berry: return "Watmel Berry"_s;
		case Item::Wave_Mail: return "Wave Mail"_s;
		case Item::Wepear_Berry: return "Wepear Berry"_s;
		case Item::White_Flute: return "White Flute"_s;
		case Item::White_Herb: return "White Herb"_s;
		case Item::Wiki_Berry: return "Wiki Berry"_s;
		case Item::Wood_Mail: return "Wood Mail"_s;
		case Item::Yago_Berry: return "Yago Berry"_s;
		case Item::Yellow_Flute: return "Yellow Flute"_s;
		case Item::Yellow_Scarf: return "Yellow Scarf"_s;
		case Item::Yellow_Shard: return "Yellow Shard"_s;
		case Item::Zinc: return "Zinc"_s;

		// Generation 4
		case Item::Adamant_Orb: return "Adamant Orb"_s;
		case Item::Air_Mail: return "Air Mail"_s;
		case Item::Armor_Fossil: return "Armor Fossil"_s;
		case Item::Babiri_Berry: return "Babiri Berry"_s;
		case Item::Big_Root: return "Big Root"_s;
		case Item::Black_Sludge: return "Black Sludge"_s;
		case Item::Bloom_Mail: return "Bloom Mail"_s;
		case Item::Brick_Mail: return "Brick Mail"_s;
		case Item::Bubble_Mail: return "Bubble Mail"_s;
		case Item::Charti_Berry: return "Charti Berry"_s;
		case Item::Cherish_Ball: return "Cherish Ball"_s;
		case Item::Chilan_Berry: return "Chilan Berry"_s;
		case Item::Choice_Scarf: return "Choice Scarf"_s;
		case Item::Choice_Specs: return "Choice Specs"_s;
		case Item::Chople_Berry: return "Chople Berry"_s;
		case Item::Coba_Berry: return "Coba Berry"_s;
		case Item::Colbur_Berry: return "Colbur Berry"_s;
		case Item::Custap_Berry: return "Custap Berry"_s;
		case Item::Damp_Mulch: return "Damp Mulch"_s;
		case Item::Damp_Rock: return "Damp Rock"_s;
		case Item::Dawn_Stone: return "Dawn Stone"_s;
		case Item::Destiny_Knot: return "Destiny Knot"_s;
		case Item::Draco_Plate: return "Draco Plate"_s;
		case Item::Dread_Plate: return "Dread Plate"_s;
		case Item::Dubious_Disc: return "Dubious Disc"_s;
		case Item::Dusk_Ball: return "Dusk Ball"_s;
		case Item::Dusk_Stone: return "Dusk Stone"_s;
		case Item::Earth_Plate: return "Earth Plate"_s;
		case Item::Electirizer: return "Electirizer"_s;
		case Item::Expert_Belt: return "Expert Belt"_s;
		case Item::Fist_Plate: return "Fist Plate"_s;
		case Item::Flame_Mail: return "Flame Mail"_s;
		case Item::Flame_Orb: return "Flame Orb"_s;
		case Item::Flame_Plate: return "Flame Plate"_s;
		case Item::Focus_Sash: return "Focus Sash"_s;
		case Item::Full_Incense: return "Full Incense"_s;
		case Item::Gooey_Mulch: return "Gooey Mulch"_s;
		case Item::Grass_Mail: return "Grass Mail"_s;
		case Item::Grip_Claw: return "Grip Claw"_s;
		case Item::Griseous_Orb: return "Griseous Orb"_s;
		case Item::Growth_Mulch: return "Growth Mulch"_s;
		case Item::Haban_Berry: return "Haban Berry"_s;
		case Item::Heal_Ball: return "Heal Ball"_s;
		case Item::Heart_Mail: return "Heart Mail"_s;
		case Item::Heat_Rock: return "Heat Rock"_s;
		case Item::Honey: return "Honey"_s;
		case Item::Icicle_Plate: return "Icicle Plate"_s;
		case Item::Icy_Rock: return "Icy Rock"_s;
		case Item::Insect_Plate: return "Insect Plate"_s;
		case Item::Iron_Ball: return "Iron Ball"_s;
		case Item::Iron_Plate: return "Iron Plate"_s;
		case Item::Jaboca_Berry: return "Jaboca Berry"_s;
		case Item::Kasib_Berry: return "Kasib Berry"_s;
		case Item::Kebia_Berry: return "Kebia Berry"_s;
		case Item::Lagging_Tail: return "Lagging Tail"_s;
		case Item::Life_Orb: return "Life Orb"_s;
		case Item::Light_Clay: return "Light Clay"_s;
		case Item::Luck_Incense: return "Luck Incense"_s;
		case Item::Lustrous_Orb: return "Lustrous Orb"_s;
		case Item::Magmarizer: return "Magmarizer"_s;
		case Item::Meadow_Plate: return "Meadow Plate"_s;
		case Item::Metronome: return "Metronome"_s;
		case Item::Micle_Berry: return "Micle Berry"_s;
		case Item::Mind_Plate: return "Mind Plate"_s;
		case Item::Mosaic_Mail: return "Mosaic Mail"_s;
		case Item::Muscle_Band: return "Muscle Band"_s;
		case Item::Occa_Berry: return "Occa Berry"_s;
		case Item::Odd_Incense: return "Odd Incense"_s;
		case Item::Odd_Keystone: return "Odd Keystone"_s;
		case Item::Old_Gateau: return "Old Gateau"_s;
		case Item::Oval_Stone: return "Oval Stone"_s;
		case Item::Park_Ball: return "Park Ball"_s;
		case Item::Passho_Berry: return "Passho Berry"_s;
		case Item::Payapa_Berry: return "Payapa Berry"_s;
		case Item::Power_Anklet: return "Power Anklet"_s;
		case Item::Power_Band: return "Power Band"_s;
		case Item::Power_Belt: return "Power Belt"_s;
		case Item::Power_Bracer: return "Power Bracer"_s;
		case Item::Power_Herb: return "Power Herb"_s;
		case Item::Power_Lens: return "Power Lens"_s;
		case Item::Power_Weight: return "Power Weight"_s;
		case Item::Protector: return "Protector"_s;
		case Item::Pure_Incense: return "Pure Incense"_s;
		case Item::Quick_Ball: return "Quick Ball"_s;
		case Item::Quick_Powder: return "Quick Powder"_s;
		case Item::Rare_Bone: return "Rare Bone"_s;
		case Item::Razor_Claw: return "Razor Claw"_s;
		case Item::Razor_Fang: return "Razor Fang"_s;
		case Item::Reaper_Cloth: return "Reaper Cloth"_s;
		case Item::Rindo_Berry: return "Rindo Berry"_s;
		case Item::Rock_Incense: return "Rock Incense"_s;
		case Item::Rose_Incense: return "Rose Incense"_s;
		case Item::Rowap_Berry: return "Rowap Berry"_s;
		case Item::Shed_Shell: return "Shed Shell"_s;
		case Item::Shiny_Stone: return "Shiny Stone"_s;
		case Item::Shuca_Berry: return "Shuca Berry"_s;
		case Item::Skull_Fossil: return "Skull Fossil"_s;
		case Item::Sky_Plate: return "Sky Plate"_s;
		case Item::Smooth_Rock: return "Smooth Rock"_s;
		case Item::Snow_Mail: return "Snow Mail"_s;
		case Item::Space_Mail: return "Space Mail"_s;
		case Item::Splash_Plate: return "Splash Plate"_s;
		case Item::Spooky_Plate: return "Spooky Plate"_s;
		case Item::Stable_Mulch: return "Stable Mulch"_s;
		case Item::Steel_Mail: return "Steel Mail"_s;
		case Item::Sticky_Barb: return "Sticky Barb"_s;
		case Item::Stone_Plate: return "Stone Plate"_s;
		case Item::Tanga_Berry: return "Tanga Berry"_s;
		case Item::TM51: return "TM51"_s;
		case Item::TM52: return "TM52"_s;
		case Item::TM53: return "TM53"_s;
		case Item::TM54: return "TM54"_s;
		case Item::TM55: return "TM55"_s;
		case Item::TM56: return "TM56"_s;
		case Item::TM57: return "TM57"_s;
		case Item::TM58: return "TM58"_s;
		case Item::TM59: return "TM59"_s;
		case Item::TM60: return "TM60"_s;
		case Item::TM61: return "TM61"_s;
		case Item::TM62: return "TM62"_s;
		case Item::TM63: return "TM63"_s;
		case Item::TM64: return "TM64"_s;
		case Item::TM65: return "TM65"_s;
		case Item::TM66: return "TM66"_s;
		case Item::TM67: return "TM67"_s;
		case Item::TM68: return "TM68"_s;
		case Item::TM69: return "TM69"_s;
		case Item::TM70: return "TM70"_s;
		case Item::TM71: return "TM71"_s;
		case Item::TM72: return "TM72"_s;
		case Item::TM73: return "TM73"_s;
		case Item::TM74: return "TM74"_s;
		case Item::TM75: return "TM75"_s;
		case Item::TM76: return "TM76"_s;
		case Item::TM77: return "TM77"_s;
		case Item::TM78: return "TM78"_s;
		case Item::TM79: return "TM79"_s;
		case Item::TM80: return "TM80"_s;
		case Item::TM81: return "TM81"_s;
		case Item::TM82: return "TM82"_s;
		case Item::TM83: return "TM83"_s;
		case Item::TM84: return "TM84"_s;
		case Item::TM85: return "TM85"_s;
		case Item::TM86: return "TM86"_s;
		case Item::TM87: return "TM87"_s;
		case Item::TM88: return "TM88"_s;
		case Item::TM89: return "TM89"_s;
		case Item::TM90: return "TM90"_s;
		case Item::TM91: return "TM91"_s;
		case Item::TM92: return "TM92"_s;
		case Item::Toxic_Orb: return "Toxic Orb"_s;
		case Item::Toxic_Plate: return "Toxic Plate"_s;
		case Item::Tunnel_Mail: return "Tunnel Mail"_s;
		case Item::Wacan_Berry: return "Wacan Berry"_s;
		case Item::Wave_Incense: return "Wave Incense"_s;
		case Item::Wide_Lens: return "Wide Lens"_s;
		case Item::Wise_Glasses: return "Wise Glasses"_s;
		case Item::X_Sp_Def: return "X Sp. Def"_s;
		case Item::Yache_Berry: return "Yache Berry"_s;
		case Item::Zap_Plate: return "Zap Plate"_s;
		case Item::Zoom_Lens: return "Zoom Lens"_s;

		// Generation 5
		case Item::Absorb_Bulb: return "Absorb Bulb"_s;
		case Item::Air_Balloon: return "Air Balloon"_s;
		case Item::Balm_Mushroom: return "Balm Mushroom"_s;
		case Item::Big_Nugget: return "Big Nugget"_s;
		case Item::Binding_Band: return "Binding Band"_s;
		case Item::BridgeMail_D: return "BridgeMail D"_s;
		case Item::BridgeMail_M: return "BridgeMail M"_s;
		case Item::BridgeMail_S: return "BridgeMail S"_s;
		case Item::BridgeMail_T: return "BridgeMail T"_s;
		case Item::BridgeMail_V: return "BridgeMail V"_s;
		case Item::Bug_Gem: return "Bug Gem"_s;
		case Item::Burn_Drive: return "Burn Drive"_s;
		case Item::Casteliacone: return "Casteliacone"_s;
		case Item::Cell_Battery: return "Cell Battery"_s;
		case Item::Chill_Drive: return "Chill Drive"_s;
		case Item::Clever_Wing: return "Clever Wing"_s;
		case Item::Comet_Shard: return "Comet Shard"_s;
		case Item::Cover_Fossil: return "Cover Fossil"_s;
		case Item::Dark_Gem: return "Dark Gem"_s;
		case Item::Douse_Drive: return "Douse Drive"_s;
		case Item::Dragon_Gem: return "Dragon Gem"_s;
		case Item::Dream_Ball: return "Dream Ball"_s;
		case Item::Eject_Button: return "Eject Button"_s;
		case Item::Electric_Gem: return "Electric Gem"_s;
		case Item::Eviolite: return "Eviolite"_s;
		case Item::Favored_Mail: return "Favored Mail"_s;
		case Item::Fighting_Gem: return "Fighting Gem"_s;
		case Item::Fire_Gem: return "Fire Gem"_s;
		case Item::Float_Stone: return "Float Stone"_s;
		case Item::Flying_Gem: return "Flying Gem"_s;
		case Item::Genius_Wing: return "Genius Wing"_s;
		case Item::Ghost_Gem: return "Ghost Gem"_s;
		case Item::Grass_Gem: return "Grass Gem"_s;
		case Item::Greet_Mail: return "Greet Mail"_s;
		case Item::Ground_Gem: return "Ground Gem"_s;
		case Item::Health_Wing: return "Health Wing"_s;
		case Item::Ice_Gem: return "Ice Gem"_s;
		case Item::Inquiry_Mail: return "Inquiry Mail"_s;
		case Item::Like_Mail: return "Like Mail"_s;
		case Item::Muscle_Wing: return "Muscle Wing"_s;
		case Item::Normal_Gem: return "Normal Gem"_s;
		case Item::Pass_Orb: return "Pass Orb"_s;
		case Item::Pearl_String: return "Pearl String"_s;
		case Item::Plume_Fossil: return "Plume Fossil"_s;
		case Item::Poison_Gem: return "Poison Gem"_s;
		case Item::Poke_Toy: return "Poke Toy"_s;
		case Item::Pretty_Wing: return "Pretty Wing"_s;
		case Item::Prism_Scale: return "Prism Scale"_s;
		case Item::Psychic_Gem: return "Psychic Gem"_s;
		case Item::Red_Card: return "Red Card"_s;
		case Item::Relic_Band: return "Relic Band"_s;
		case Item::Relic_Copper: return "Relic Copper"_s;
		case Item::Relic_Crown: return "Relic Crown"_s;
		case Item::Relic_Gold: return "Relic Gold"_s;
		case Item::Relic_Silver: return "Relic Silver"_s;
		case Item::Relic_Statue: return "Relic Statue"_s;
		case Item::Relic_Vase: return "Relic Vase"_s;
		case Item::Reply_Mail: return "Reply Mail"_s;
		case Item::Resist_Wing: return "Resist Wing"_s;
		case Item::Ring_Target: return "Ring Target"_s;
		case Item::Rock_Gem: return "Rock Gem"_s;
		case Item::Rocky_Helmet: return "Rocky Helmet"_s;
		case Item::RSVP_Mail: return "RSVP Mail"_s;
		case Item::Shock_Drive: return "Shock Drive"_s;
		case Item::Steel_Gem: return "Steel Gem"_s;
		case Item::Sweet_Heart: return "Sweet Heart"_s;
		case Item::Swift_Wing: return "Swift Wing"_s;
		case Item::Thanks_Mail: return "Thanks Mail"_s;
		case Item::Water_Gem: return "Water Gem"_s;

		// Generation 6
		case Item::Ability_Capsule: return "Ability Capsule"_s;
		case Item::Abomasite: return "Abomasite"_s;
		case Item::Absolite: return "Absolite"_s;
		case Item::Aerodactylite: return "Aerodactylite"_s;
		case Item::Aggronite: return "Aggronite"_s;
		case Item::Alakazite: return "Alakazite"_s;
		case Item::Altarianite: return "Altarianite"_s;
		case Item::Amaze_Mulch: return "Amaze Mulch"_s;
		case Item::Ampharosite: return "Ampharosite"_s;
		case Item::Assault_Vest: return "Assault Vest"_s;
		case Item::Audinite: return "Audinite"_s;
		case Item::Banettite: return "Banettite"_s;
		case Item::Beedrillite: return "Beedrillite"_s;
		case Item::Blastoisinite: return "Blastoisinite"_s;
		case Item::Blazikenite: return "Blazikenite"_s;
		case Item::Boost_Mulch: return "Boost Mulch"_s;
		case Item::Cameruptite: return "Cameruptite"_s;
		case Item::Charizardite_X: return "Charizardite X"_s;
		case Item::Charizardite_Y: return "Charizardite Y"_s;
		case Item::Diancite: return "Diancite"_s;
		case Item::Discount_Coupon: return "Discount Coupon"_s;
		case Item::Fairy_Gem: return "Fairy Gem"_s;
		case Item::Galladite: return "Galladite"_s;
		case Item::Garchompite: return "Garchompite"_s;
		case Item::Gardevoirite: return "Gardevoirite"_s;
		case Item::Gengarite: return "Gengarite"_s;
		case Item::Glalitite: return "Glalitite"_s;
		case Item::Gyaradosite: return "Gyaradosite"_s;
		case Item::Heracronite: return "Heracronite"_s;
		case Item::Houndoominite: return "Houndoominite"_s;
		case Item::Jaw_Fossil: return "Jaw Fossil"_s;
		case Item::Kangaskhanite: return "Kangaskhanite"_s;
		case Item::Kee_Berry: return "Kee Berry"_s;
		case Item::Latiasite: return "Latiasite"_s;
		case Item::Latiosite: return "Latiosite"_s;
		case Item::Lopunnite: return "Lopunnite"_s;
		case Item::Lucarionite: return "Lucarionite"_s;
		case Item::Luminous_Moss: return "Luminous Moss"_s;
		case Item::Lumiose_Galette: return "Lumiose Galette"_s;
		case Item::Manectite: return "Manectite"_s;
		case Item::Maranga_Berry: return "Maranga Berry"_s;
		case Item::Mawilite: return "Mawilite"_s;
		case Item::Medichamite: return "Medichamite"_s;
		case Item::Metagrossite: return "Metagrossite"_s;
		case Item::Mewtwonite_X: return "Mewtwonite X"_s;
		case Item::Mewtwonite_Y: return "Mewtwonite Y"_s;
		case Item::Pidgeotite: return "Pidgeotite"_s;
		case Item::Pinsirite: return "Pinsirite"_s;
		case Item::Pixie_Plate: return "Pixie Plate"_s;
		case Item::Rich_Mulch: return "Rich Mulch"_s;
		case Item::Roseli_Berry: return "Roseli Berry"_s;
		case Item::Sablenite: return "Sablenite"_s;
		case Item::Sachet: return "Sachet"_s;
		case Item::Safety_Goggles: return "Safety Goggles"_s;
		case Item::Sail_Fossil: return "Sail Fossil"_s;
		case Item::Salamencite: return "Salamencite"_s;
		case Item::Sceptilite: return "Sceptilite"_s;
		case Item::Scizorite: return "Scizorite"_s;
		case Item::Shalour_Sable: return "Shalour Sable"_s;
		case Item::Sharpedonite: return "Sharpedonite"_s;
		case Item::Slowbronite: return "Slowbronite"_s;
		case Item::Snowball: return "Snowball"_s;
		case Item::Steelixite: return "Steelixite"_s;
		case Item::Strange_Souvenir: return "Strange Souvenir"_s;
		case Item::Surprise_Mulch: return "Surprise Mulch"_s;
		case Item::Swampertite: return "Swampertite"_s;
		case Item::Tyranitarite: return "Tyranitarite"_s;
		case Item::Venusaurite: return "Venusaurite"_s;
		case Item::Weakness_Policy: return "Weakness Policy"_s;
		case Item::Whipped_Dream: return "Whipped Dream"_s;
		case Item::X_Defense: return "X Defense"_s;
		case Item::X_Sp_Atk: return "X Sp. Atk"_s;

		// Generation 7
		case Item::Adrenaline_Orb: return "Adrenaline Orb"_s;
		case Item::Aloraichium_Z: return "Aloraichium Z"_s;
		case Item::Beast_Ball: return "Beast Ball"_s;
		case Item::Big_Malasada: return "Big Malasada"_s;
		case Item::Bottle_Cap: return "Bottle Cap"_s;
		case Item::Bug_Memory: return "Bug Memory"_s;
		case Item::Buginium_Z: return "Buginium Z"_s;
		case Item::Dark_Memory: return "Dark Memory"_s;
		case Item::Darkinium_Z: return "Darkinium Z"_s;
		case Item::Decidium_Z: return "Decidium Z"_s;
		case Item::Dragon_Memory: return "Dragon Memory"_s;
		case Item::Dragonium_Z: return "Dragonium Z"_s;
		case Item::Eevium_Z: return "Eevium Z"_s;
		case Item::Electric_Memory: return "Electric Memory"_s;
		case Item::Electric_Seed: return "Electric Seed"_s;
		case Item::Electrium_Z: return "Electrium Z"_s;
		case Item::Fairium_Z: return "Fairium Z"_s;
		case Item::Fairy_Memory: return "Fairy Memory"_s;
		case Item::Festival_Ticket: return "Festival Ticket"_s;
		case Item::Fighting_Memory: return "Fighting Memory"_s;
		case Item::Fightinium_Z: return "Fightinium Z"_s;
		case Item::Fire_Memory: return "Fire Memory"_s;
		case Item::Firium_Z: return "Firium Z"_s;
		case Item::Flying_Memory: return "Flying Memory"_s;
		case Item::Flyinium_Z: return "Flyinium Z"_s;
		case Item::Ghost_Memory: return "Ghost Memory"_s;
		case Item::Ghostium_Z: return "Ghostium Z"_s;
		case Item::Gold_Bottle_Cap: return "Gold Bottle Cap"_s;
		case Item::Grass_Memory: return "Grass Memory"_s;
		case Item::Grassium_Z: return "Grassium Z"_s;
		case Item::Grassy_Seed: return "Grassy Seed"_s;
		case Item::Ground_Memory: return "Ground Memory"_s;
		case Item::Groundium_Z: return "Groundium Z"_s;
		case Item::Ice_Memory: return "Ice Memory"_s;
		case Item::Ice_Stone: return "Ice Stone"_s;
		case Item::Icium_Z: return "Icium Z"_s;
		case Item::Incinium_Z: return "Incinium Z"_s;
		case Item::Kommonium_Z: return "Kommonium Z"_s;
		case Item::Lunalium_Z: return "Lunalium Z"_s;
		case Item::Lycanium_Z: return "Lycanium Z"_s;
		case Item::Marshadium_Z: return "Marshadium Z"_s;
		case Item::Mewnium_Z: return "Mewnium Z"_s;
		case Item::Mimikium_Z: return "Mimikium Z"_s;
		case Item::Misty_Seed: return "Misty Seed"_s;
		case Item::Normalium_Z: return "Normalium Z"_s;
		case Item::Pewter_Crunchies: return "Pewter Crunchies"_s;
		case Item::Pikanium_Z: return "Pikanium Z"_s;
		case Item::Pikashunium_Z: return "Pikashunium Z"_s;
		case Item::Pink_Nectar: return "Pink Nectar"_s;
		case Item::Poison_Memory: return "Poison Memory"_s;
		case Item::Poisonium_Z: return "Poisonium Z"_s;
		case Item::Primarium_Z: return "Primarium Z"_s;
		case Item::Protective_Pads: return "Protective Pads"_s;
		case Item::Psychic_Memory: return "Psychic Memory"_s;
		case Item::Psychic_Seed: return "Psychic Seed"_s;
		case Item::Psychium_Z: return "Psychium Z"_s;
		case Item::Purple_Nectar: return "Purple Nectar"_s;
		case Item::Rock_Memory: return "Rock Memory"_s;
		case Item::Rockium_Z: return "Rockium Z"_s;
		case Item::Snorlium_Z: return "Snorlium Z"_s;
		case Item::Solganium_Z: return "Solganium Z"_s;
		case Item::Steel_Memory: return "Steel Memory"_s;
		case Item::Steelium_Z: return "Steelium Z"_s;
		case Item::Tapunium_Z: return "Tapunium Z"_s;
		case Item::Terrain_Extender: return "Terrain Extender"_s;
		case Item::Ultranecrozium_Z: return "Ultranecrozium Z"_s;
		case Item::Water_Memory: return "Water Memory"_s;
		case Item::Waterium_Z: return "Waterium Z"_s;
		case Item::Yellow_Nectar: return "Yellow Nectar"_s;

		// Generation 8
		case Item::Adamant_Mint: return "Adamant Mint"_s;
		case Item::Bachs_Food_Tin: return "Bach's Food Tin"_s;
		case Item::Berry_Sweet: return "Berry Sweet"_s;
		case Item::Blunder_Policy: return "Blunder Policy"_s;
		case Item::Bobs_Food_Tin: return "Bob's Food Tin"_s;
		case Item::Boiled_Egg: return "Boiled Egg"_s;
		case Item::Bold_Mint: return "Bold Mint"_s;
		case Item::Brave_Mint: return "Brave Mint"_s;
		case Item::Bread: return "Bread"_s;
		case Item::Brittle_Bones: return "Brittle Bones"_s;
		case Item::Calm_Mint: return "Calm Mint"_s;
		case Item::Careful_Mint: return "Careful Mint"_s;
		case Item::Chipped_Pot: return "Chipped Pot"_s;
		case Item::Clover_Sweet: return "Clover Sweet"_s;
		case Item::Coconut_Milk: return "Coconut Milk"_s;
		case Item::Cracked_Pot: return "Cracked Pot"_s;
		case Item::Eject_Pack: return "Eject Pack"_s;
		case Item::Fancy_Apple: return "Fancy Apple"_s;
		case Item::Flower_Sweet: return "Flower Sweet"_s;
		case Item::Fresh_Cream: return "Fresh Cream"_s;
		case Item::Fossilized_Bird: return "Fossilized Bird"_s;
		case Item::Fossilized_Dino: return "Fossilized Dino"_s;
		case Item::Fossilized_Drake: return "Fossilized Drake"_s;
		case Item::Fossilized_Fish: return "Fossilized Fish"_s;
		case Item::Fried_Food: return "Fried Food"_s;
		case Item::Fruit_Bunch: return "Fruit Bunch"_s;
		case Item::Gentle_Mint: return "Gentle Mint"_s;
		case Item::Gigantamix: return "Gigantamix"_s;
		case Item::Hasty_Mint: return "Hasty Mint"_s;
		case Item::Heavy_Duty_Boots: return "Heavy-Duty Boots"_s;
		case Item::Impish_Mint: return "Impish Mint"_s;
		case Item::Instant_Noodles: return "Instant Noodles"_s;
		case Item::Jolly_Mint: return "Jolly Mint"_s;
		case Item::Large_Leek: return "Large Leek"_s;
		case Item::Lax_Mint: return "Lax Mint"_s;
		case Item::Leek: return "Leek"_s;
		case Item::Lonely_Mint: return "Lonely Mint"_s;
		case Item::Love_Sweet: return "Love Sweet"_s;
		case Item::Mild_Mint: return "Mild Mint"_s;
		case Item::Mixed_Mushrooms: return "Mixed Mushrooms"_s;
		case Item::Modest_Mint: return "Modest Mint"_s;
		case Item::Moomoo_Cheese: return "Moomoo Cheese"_s;
		case Item::Naive_Mint: return "Naive Mint"_s;
		case Item::Naughty_Mint: return "Naughty Mint"_s;
		case Item::Pack_of_Potatoes: return "Pack of Potatoes"_s;
		case Item::Packaged_Curry: return "Packaged Curry"_s;
		case Item::Pasta: return "Pasta"_s;
		case Item::Precooked_Burger: return "Precooked Burger"_s;
		case Item::Pungent_Root: return "Pungent Root"_s;
		case Item::Quiet_Mint: return "Quiet Mint"_s;
		case Item::Rash_Mint: return "Rash Mint"_s;
		case Item::Relaxed_Mint: return "Relaxed Mint"_s;
		case Item::Ribbon_Sweet: return "Ribbon Sweet"_s;
		case Item::Room_Service: return "Room Service"_s;
		case Item::Rusted_Shield: return "Rusted Shield"_s;
		case Item::Rusted_Sword: return "Rusted Sword"_s;
		case Item::Salad_Mix: return "Salad Mix"_s;
		case Item::Sassy_Mint: return "Sassy Mint"_s;
		case Item::Sausages: return "Sausages"_s;
		case Item::Serious_Mint: return "Serious Mint"_s;
		case Item::Smoke_Poke_Tail: return "Smoke-Poke Tail"_s;
		case Item::Spice_Mix: return "Spice Mix"_s;
		case Item::Star_Sweet: return "Star Sweet"_s;
		case Item::Strawberry_Sweet: return "Strawberry Sweet"_s;
		case Item::Sweet_Apple: return "Sweet Apple"_s;
		case Item::Tart_Apple: return "Tart Apple"_s;
		case Item::Throat_Spray: return "Throat Spray"_s;
		case Item::Timid_Mint: return "Timid Mint"_s;
		case Item::Tin_of_Beans: return "Tin of Beans"_s;
		case Item::TR00: return "TR00"_s;
		case Item::TR01: return "TR01"_s;
		case Item::TR02: return "TR02"_s;
		case Item::TR03: return "TR03"_s;
		case Item::TR04: return "TR04"_s;
		case Item::TR05: return "TR05"_s;
		case Item::TR06: return "TR06"_s;
		case Item::TR07: return "TR07"_s;
		case Item::TR08: return "TR08"_s;
		case Item::TR09: return "TR09"_s;
		case Item::TR10: return "TR10"_s;
		case Item::TR11: return "TR11"_s;
		case Item::TR12: return "TR12"_s;
		case Item::TR13: return "TR13"_s;
		case Item::TR14: return "TR14"_s;
		case Item::TR15: return "TR15"_s;
		case Item::TR16: return "TR16"_s;
		case Item::TR17: return "TR17"_s;
		case Item::TR18: return "TR18"_s;
		case Item::TR19: return "TR19"_s;
		case Item::TR20: return "TR20"_s;
		case Item::TR21: return "TR21"_s;
		case Item::TR22: return "TR22"_s;
		case Item::TR23: return "TR23"_s;
		case Item::TR24: return "TR24"_s;
		case Item::TR25: return "TR25"_s;
		case Item::TR26: return "TR26"_s;
		case Item::TR27: return "TR27"_s;
		case Item::TR28: return "TR28"_s;
		case Item::TR29: return "TR29"_s;
		case Item::TR30: return "TR30"_s;
		case Item::TR31: return "TR31"_s;
		case Item::TR32: return "TR32"_s;
		case Item::TR33: return "TR33"_s;
		case Item::TR34: return "TR34"_s;
		case Item::TR35: return "TR35"_s;
		case Item::TR36: return "TR36"_s;
		case Item::TR37: return "TR37"_s;
		case Item::TR38: return "TR38"_s;
		case Item::TR39: return "TR39"_s;
		case Item::TR40: return "TR40"_s;
		case Item::TR41: return "TR41"_s;
		case Item::TR42: return "TR42"_s;
		case Item::TR43: return "TR43"_s;
		case Item::TR44: return "TR44"_s;
		case Item::TR45: return "TR45"_s;
		case Item::TR46: return "TR46"_s;
		case Item::TR47: return "TR47"_s;
		case Item::TR48: return "TR48"_s;
		case Item::TR49: return "TR49"_s;
		case Item::TR50: return "TR50"_s;
		case Item::TR51: return "TR51"_s;
		case Item::TR52: return "TR52"_s;
		case Item::TR53: return "TR53"_s;
		case Item::TR54: return "TR54"_s;
		case Item::TR55: return "TR55"_s;
		case Item::TR56: return "TR56"_s;
		case Item::TR57: return "TR57"_s;
		case Item::TR58: return "TR58"_s;
		case Item::TR59: return "TR59"_s;
		case Item::TR60: return "TR60"_s;
		case Item::TR61: return "TR61"_s;
		case Item::TR62: return "TR62"_s;
		case Item::TR63: return "TR63"_s;
		case Item::TR64: return "TR64"_s;
		case Item::TR65: return "TR65"_s;
		case Item::TR66: return "TR66"_s;
		case Item::TR67: return "TR67"_s;
		case Item::TR68: return "TR68"_s;
		case Item::TR69: return "TR69"_s;
		case Item::TR70: return "TR70"_s;
		case Item::TR71: return "TR71"_s;
		case Item::TR72: return "TR72"_s;
		case Item::TR73: return "TR73"_s;
		case Item::TR74: return "TR74"_s;
		case Item::TR75: return "TR75"_s;
		case Item::TR76: return "TR76"_s;
		case Item::TR77: return "TR77"_s;
		case Item::TR78: return "TR78"_s;
		case Item::TR79: return "TR79"_s;
		case Item::TR80: return "TR80"_s;
		case Item::TR81: return "TR81"_s;
		case Item::TR82: return "TR82"_s;
		case Item::TR83: return "TR83"_s;
		case Item::TR84: return "TR84"_s;
		case Item::TR85: return "TR85"_s;
		case Item::TR86: return "TR86"_s;
		case Item::TR87: return "TR87"_s;
		case Item::TR88: return "TR88"_s;
		case Item::TR89: return "TR89"_s;
		case Item::TR90: return "TR90"_s;
		case Item::TR91: return "TR91"_s;
		case Item::TR92: return "TR92"_s;
		case Item::TR93: return "TR93"_s;
		case Item::TR94: return "TR94"_s;
		case Item::TR95: return "TR95"_s;
		case Item::TR96: return "TR96"_s;
		case Item::TR97: return "TR97"_s;
		case Item::TR98: return "TR98"_s;
		case Item::TR99: return "TR99"_s;
		case Item::Utility_Umbrella: return "Utility Umbrella"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Item {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Item>>({
			{""_s, Item::None},
			{"abilitycapsule"_s, Item::Ability_Capsule},
			{"abomasite"_s, Item::Abomasite},
			{"absolite"_s, Item::Absolite},
			{"absorbbulb"_s, Item::Absorb_Bulb},
			{"adamantmint"_s, Item::Adamant_Mint},
			{"adamantorb"_s, Item::Adamant_Orb},
			{"adrenalineorb"_s, Item::Adrenaline_Orb},
			{"aerodactylite"_s, Item::Aerodactylite},
			{"aggronite"_s, Item::Aggronite},
			{"aguavberry"_s, Item::Aguav_Berry},
			{"airballoon"_s, Item::Air_Balloon},
			{"airmail"_s, Item::Air_Mail},
			{"alakazite"_s, Item::Alakazite},
			{"aloraichiumz"_s, Item::Aloraichium_Z},
			{"altarianite"_s, Item::Altarianite},
			{"amazemulch"_s, Item::Amaze_Mulch},
			{"ampharosite"_s, Item::Ampharosite},
			{"amuletcoin"_s, Item::Amulet_Coin},
			{"antidote"_s, Item::Antidote},
			{"apicotberry"_s, Item::Apicot_Berry},
			{"armorfossil"_s, Item::Armor_Fossil},
			{"aspearberry"_s, Item::Aspear_Berry},
			{"assaultvest"_s, Item::Assault_Vest},
			{"audinite"_s, Item::Audinite},
			{"awakening"_s, Item::Awakening},
			{"babiriberry"_s, Item::Babiri_Berry},
			{"bachsfoodtin"_s, Item::Bachs_Food_Tin},
			{"balmmushroom"_s, Item::Balm_Mushroom},
			{"banettite"_s, Item::Banettite},
			{"beadmail"_s, Item::Bead_Mail},
			{"beastball"_s, Item::Beast_Ball},
			{"beedrillite"_s, Item::Beedrillite},
			{"belueberry"_s, Item::Belue_Berry},
			{"berry"_s, Item::Berry},
			{"berryjuice"_s, Item::Berry_Juice},
			{"berrysweet"_s, Item::Berry_Sweet},
			{"berserkgene"_s, Item::Berserk_Gene},
			{"bigmalasada"_s, Item::Big_Malasada},
			{"bigmushroom"_s, Item::Big_Mushroom},
			{"bignugget"_s, Item::Big_Nugget},
			{"bigpearl"_s, Item::Big_Pearl},
			{"bigroot"_s, Item::Big_Root},
			{"bindingband"_s, Item::Binding_Band},
			{"bitterberry"_s, Item::Bitter_Berry},
			{"blackapricorn"_s, Item::Black_Apricorn},
			{"blackbelt"_s, Item::Black_Belt},
			{"blackflute"_s, Item::Black_Flute},
			{"blackglasses"_s, Item::Black_Glasses},
			{"blacksludge"_s, Item::Black_Sludge},
			{"blastoisinite"_s, Item::Blastoisinite},
			{"blazikenite"_s, Item::Blazikenite},
			{"bloommail"_s, Item::Bloom_Mail},
			{"blueapricorn"_s, Item::Blue_Apricorn},
			{"blueflute"_s, Item::Blue_Flute},
			{"blueorb"_s, Item::Blue_Orb},
			{"bluescarf"_s, Item::Blue_Scarf},
			{"blueshard"_s, Item::Blue_Shard},
			{"blueskymail"_s, Item::Bluesky_Mail},
			{"blukberry"_s, Item::Bluk_Berry},
			{"blunderpolicy"_s, Item::Blunder_Policy},
			{"bobsfoodtin"_s, Item::Bobs_Food_Tin},
			{"boiledegg"_s, Item::Boiled_Egg},
			{"boldmint"_s, Item::Bold_Mint},
			{"boostmulch"_s, Item::Boost_Mulch},
			{"bottlecap"_s, Item::Bottle_Cap},
			{"bravemint"_s, Item::Brave_Mint},
			{"bread"_s, Item::Bread},
			{"brickmail"_s, Item::Brick_Mail},
			{"brickpiece"_s, Item::Brick_Piece},
			{"bridgemaild"_s, Item::BridgeMail_D},
			{"bridgemailm"_s, Item::BridgeMail_M},
			{"bridgemails"_s, Item::BridgeMail_S},
			{"bridgemailt"_s, Item::BridgeMail_T},
			{"bridgemailv"_s, Item::BridgeMail_V},
			{"brightpowder"_s, Item::Bright_Powder},
			{"brittlebones"_s, Item::Brittle_Bones},
			{"bubblemail"_s, Item::Bubble_Mail},
			{"buggem"_s, Item::Bug_Gem},
			{"buginiumz"_s, Item::Buginium_Z},
			{"bugmemory"_s, Item::Bug_Memory},
			{"burndrive"_s, Item::Burn_Drive},
			{"burnheal"_s, Item::Burn_Heal},
			{"burntberry"_s, Item::Burnt_Berry},
			{"calcium"_s, Item::Calcium},
			{"calmmint"_s, Item::Calm_Mint},
			{"cameruptite"_s, Item::Cameruptite},
			{"carbos"_s, Item::Carbos},
			{"carefulmint"_s, Item::Careful_Mint},
			{"casteliacone"_s, Item::Casteliacone},
			{"cellbattery"_s, Item::Cell_Battery},
			{"charcoal"_s, Item::Charcoal},
			{"charizarditex"_s, Item::Charizardite_X},
			{"charizarditey"_s, Item::Charizardite_Y},
			{"chartiberry"_s, Item::Charti_Berry},
			{"cheriberry"_s, Item::Cheri_Berry},
			{"cherishball"_s, Item::Cherish_Ball},
			{"chestoberry"_s, Item::Chesto_Berry},
			{"chilanberry"_s, Item::Chilan_Berry},
			{"chilldrive"_s, Item::Chill_Drive},
			{"chippedpot"_s, Item::Chipped_Pot},
			{"choiceband"_s, Item::Choice_Band},
			{"choicescarf"_s, Item::Choice_Scarf},
			{"choicespecs"_s, Item::Choice_Specs},
			{"chopleberry"_s, Item::Chople_Berry},
			{"clawfossil"_s, Item::Claw_Fossil},
			{"cleansetag"_s, Item::Cleanse_Tag},
			{"cleverwing"_s, Item::Clever_Wing},
			{"cloversweet"_s, Item::Clover_Sweet},
			{"cobaberry"_s, Item::Coba_Berry},
			{"coconutmilk"_s, Item::Coconut_Milk},
			{"colburberry"_s, Item::Colbur_Berry},
			{"cometshard"_s, Item::Comet_Shard},
			{"cornnberry"_s, Item::Cornn_Berry},
			{"coverfossil"_s, Item::Cover_Fossil},
			{"crackedpot"_s, Item::Cracked_Pot},
			{"custapberry"_s, Item::Custap_Berry},
			{"dampmulch"_s, Item::Damp_Mulch},
			{"damprock"_s, Item::Damp_Rock},
			{"darkgem"_s, Item::Dark_Gem},
			{"darkiniumz"_s, Item::Darkinium_Z},
			{"darkmemory"_s, Item::Dark_Memory},
			{"dawnstone"_s, Item::Dawn_Stone},
			{"decidiumz"_s, Item::Decidium_Z},
			{"deepseascale"_s, Item::Deep_Sea_Scale},
			{"deepseatooth"_s, Item::Deep_Sea_Tooth},
			{"destinyknot"_s, Item::Destiny_Knot},
			{"diancite"_s, Item::Diancite},
			{"direhit"_s, Item::Dire_Hit},
			{"discountcoupon"_s, Item::Discount_Coupon},
			{"diveball"_s, Item::Dive_Ball},
			{"domefossil"_s, Item::Dome_Fossil},
			{"dousedrive"_s, Item::Douse_Drive},
			{"dracoplate"_s, Item::Draco_Plate},
			{"dragonfang"_s, Item::Dragon_Fang},
			{"dragongem"_s, Item::Dragon_Gem},
			{"dragoniumz"_s, Item::Dragonium_Z},
			{"dragonmemory"_s, Item::Dragon_Memory},
			{"dragonscale"_s, Item::Dragon_Scale},
			{"drashberry"_s, Item::Drash_Berry},
			{"dreadplate"_s, Item::Dread_Plate},
			{"dreamball"_s, Item::Dream_Ball},
			{"dreammail"_s, Item::Dream_Mail},
			{"dubiousdisc"_s, Item::Dubious_Disc},
			{"durinberry"_s, Item::Durin_Berry},
			{"duskball"_s, Item::Dusk_Ball},
			{"duskstone"_s, Item::Dusk_Stone},
			{"earthplate"_s, Item::Earth_Plate},
			{"eeviumz"_s, Item::Eevium_Z},
			{"eggantberry"_s, Item::Eggant_Berry},
			{"ejectbutton"_s, Item::Eject_Button},
			{"ejectpack"_s, Item::Eject_Pack},
			{"electirizer"_s, Item::Electirizer},
			{"electricgem"_s, Item::Electric_Gem},
			{"electricmemory"_s, Item::Electric_Memory},
			{"electricseed"_s, Item::Electric_Seed},
			{"electriumz"_s, Item::Electrium_Z},
			{"elixir"_s, Item::Elixir},
			{"energypowder"_s, Item::Energy_Powder},
			{"energyroot"_s, Item::Energy_Root},
			{"enigmaberry"_s, Item::Enigma_Berry},
			{"eonmail"_s, Item::Eon_Mail},
			{"escaperope"_s, Item::Escape_Rope},
			{"ether"_s, Item::Ether},
			{"everstone"_s, Item::Everstone},
			{"eviolite"_s, Item::Eviolite},
			{"expertbelt"_s, Item::Expert_Belt},
			{"expshare"_s, Item::Exp_Share},
			{"fabmail"_s, Item::Fab_Mail},
			{"fairiumz"_s, Item::Fairium_Z},
			{"fairygem"_s, Item::Fairy_Gem},
			{"fairymemory"_s, Item::Fairy_Memory},
			{"fancyapple"_s, Item::Fancy_Apple},
			{"fastball"_s, Item::Fast_Ball},
			{"favoredmail"_s, Item::Favored_Mail},
			{"festivalticket"_s, Item::Festival_Ticket},
			{"fightinggem"_s, Item::Fighting_Gem},
			{"fightingmemory"_s, Item::Fighting_Memory},
			{"fightiniumz"_s, Item::Fightinium_Z},
			{"figyberry"_s, Item::Figy_Berry},
			{"firegem"_s, Item::Fire_Gem},
			{"firememory"_s, Item::Fire_Memory},
			{"firestone"_s, Item::Fire_Stone},
			{"firiumz"_s, Item::Firium_Z},
			{"fistplate"_s, Item::Fist_Plate},
			{"flamemail"_s, Item::Flame_Mail},
			{"flameorb"_s, Item::Flame_Orb},
			{"flameplate"_s, Item::Flame_Plate},
			{"floatstone"_s, Item::Float_Stone},
			{"flowermail"_s, Item::Flower_Mail},
			{"flowersweet"_s, Item::Flower_Sweet},
			{"fluffytail"_s, Item::Fluffy_Tail},
			{"flyinggem"_s, Item::Flying_Gem},
			{"flyingmemory"_s, Item::Flying_Memory},
			{"flyiniumz"_s, Item::Flyinium_Z},
			{"focusband"_s, Item::Focus_Band},
			{"focussash"_s, Item::Focus_Sash},
			{"fossilizedbird"_s, Item::Fossilized_Bird},
			{"fossilizeddino"_s, Item::Fossilized_Dino},
			{"fossilizeddrake"_s, Item::Fossilized_Drake},
			{"fossilizedfish"_s, Item::Fossilized_Fish},
			{"freshcream"_s, Item::Fresh_Cream},
			{"freshwater"_s, Item::Fresh_Water},
			{"friedfood"_s, Item::Fried_Food},
			{"friendball"_s, Item::Friend_Ball},
			{"fruitbunch"_s, Item::Fruit_Bunch},
			{"fullheal"_s, Item::Full_Heal},
			{"fullincense"_s, Item::Full_Incense},
			{"fullrestore"_s, Item::Full_Restore},
			{"galladite"_s, Item::Galladite},
			{"ganlonberry"_s, Item::Ganlon_Berry},
			{"garchompite"_s, Item::Garchompite},
			{"gardevoirite"_s, Item::Gardevoirite},
			{"gengarite"_s, Item::Gengarite},
			{"geniuswing"_s, Item::Genius_Wing},
			{"gentlemint"_s, Item::Gentle_Mint},
			{"ghostgem"_s, Item::Ghost_Gem},
			{"ghostiumz"_s, Item::Ghostium_Z},
			{"ghostmemory"_s, Item::Ghost_Memory},
			{"gigantamix"_s, Item::Gigantamix},
			{"ginemaberry"_s, Item::Ginema_Berry},
			{"glalitite"_s, Item::Glalitite},
			{"glittermail"_s, Item::Glitter_Mail},
			{"goldberry"_s, Item::Gold_Berry},
			{"goldbottlecap"_s, Item::Gold_Bottle_Cap},
			{"goldleaf"_s, Item::Gold_Leaf},
			{"gooeymulch"_s, Item::Gooey_Mulch},
			{"gorgeousbox"_s, Item::Gorgeous_Box},
			{"grassgem"_s, Item::Grass_Gem},
			{"grassiumz"_s, Item::Grassium_Z},
			{"grassmail"_s, Item::Grass_Mail},
			{"grassmemory"_s, Item::Grass_Memory},
			{"grassyseed"_s, Item::Grassy_Seed},
			{"greatball"_s, Item::Great_Ball},
			{"greenapricorn"_s, Item::Green_Apricorn},
			{"greenscarf"_s, Item::Green_Scarf},
			{"greenshard"_s, Item::Green_Shard},
			{"greetmail"_s, Item::Greet_Mail},
			{"grepaberry"_s, Item::Grepa_Berry},
			{"gripclaw"_s, Item::Grip_Claw},
			{"griseousorb"_s, Item::Griseous_Orb},
			{"groundgem"_s, Item::Ground_Gem},
			{"groundiumz"_s, Item::Groundium_Z},
			{"groundmemory"_s, Item::Ground_Memory},
			{"growthmulch"_s, Item::Growth_Mulch},
			{"guardspec"_s, Item::Guard_Spec},
			{"gyaradosite"_s, Item::Gyaradosite},
			{"habanberry"_s, Item::Haban_Berry},
			{"harbormail"_s, Item::Harbor_Mail},
			{"hardstone"_s, Item::Hard_Stone},
			{"hastymint"_s, Item::Hasty_Mint},
			{"healball"_s, Item::Heal_Ball},
			{"healpowder"_s, Item::Heal_Powder},
			{"healthwing"_s, Item::Health_Wing},
			{"heartmail"_s, Item::Heart_Mail},
			{"heartscale"_s, Item::Heart_Scale},
			{"heatrock"_s, Item::Heat_Rock},
			{"heavyball"_s, Item::Heavy_Ball},
			{"heavydutyboots"_s, Item::Heavy_Duty_Boots},
			{"helixfossil"_s, Item::Helix_Fossil},
			{"heracronite"_s, Item::Heracronite},
			{"hondewberry"_s, Item::Hondew_Berry},
			{"honey"_s, Item::Honey},
			{"houndoominite"_s, Item::Houndoominite},
			{"hpup"_s, Item::HP_Up},
			{"hyperpotion"_s, Item::Hyper_Potion},
			{"iapapaberry"_s, Item::Iapapa_Berry},
			{"iceberry"_s, Item::Ice_Berry},
			{"icegem"_s, Item::Ice_Gem},
			{"iceheal"_s, Item::Ice_Heal},
			{"icememory"_s, Item::Ice_Memory},
			{"icestone"_s, Item::Ice_Stone},
			{"icicleplate"_s, Item::Icicle_Plate},
			{"iciumz"_s, Item::Icium_Z},
			{"icyrock"_s, Item::Icy_Rock},
			{"impishmint"_s, Item::Impish_Mint},
			{"inciniumz"_s, Item::Incinium_Z},
			{"inquirymail"_s, Item::Inquiry_Mail},
			{"insectplate"_s, Item::Insect_Plate},
			{"instantnoodles"_s, Item::Instant_Noodles},
			{"iron"_s, Item::Iron},
			{"ironball"_s, Item::Iron_Ball},
			{"ironplate"_s, Item::Iron_Plate},
			{"jabocaberry"_s, Item::Jaboca_Berry},
			{"jawfossil"_s, Item::Jaw_Fossil},
			{"jollymint"_s, Item::Jolly_Mint},
			{"kangaskhanite"_s, Item::Kangaskhanite},
			{"kasibberry"_s, Item::Kasib_Berry},
			{"kebiaberry"_s, Item::Kebia_Berry},
			{"keeberry"_s, Item::Kee_Berry},
			{"kelpsyberry"_s, Item::Kelpsy_Berry},
			{"kingsrock"_s, Item::Kings_Rock},
			{"kommoniumz"_s, Item::Kommonium_Z},
			{"kuoberry"_s, Item::Kuo_Berry},
			{"laggingtail"_s, Item::Lagging_Tail},
			{"lansatberry"_s, Item::Lansat_Berry},
			{"largeleek"_s, Item::Large_Leek},
			{"latiasite"_s, Item::Latiasite},
			{"latiosite"_s, Item::Latiosite},
			{"lavacookie"_s, Item::Lava_Cookie},
			{"laxincense"_s, Item::Lax_Incense},
			{"laxmint"_s, Item::Lax_Mint},
			{"leafstone"_s, Item::Leaf_Stone},
			{"leek"_s, Item::Leek},
			{"leftovers"_s, Item::Leftovers},
			{"lemonade"_s, Item::Lemonade},
			{"leppaberry"_s, Item::Leppa_Berry},
			{"levelball"_s, Item::Level_Ball},
			{"liechiberry"_s, Item::Liechi_Berry},
			{"lifeorb"_s, Item::Life_Orb},
			{"lightball"_s, Item::Light_Ball},
			{"lightclay"_s, Item::Light_Clay},
			{"likemail"_s, Item::Like_Mail},
			{"litebluemail"_s, Item::Litebluemail},
			{"lonelymint"_s, Item::Lonely_Mint},
			{"lopunnite"_s, Item::Lopunnite},
			{"loveball"_s, Item::Love_Ball},
			{"lovelymail"_s, Item::Lovely_Mail},
			{"lovesweet"_s, Item::Love_Sweet},
			{"lucarionite"_s, Item::Lucarionite},
			{"luckincense"_s, Item::Luck_Incense},
			{"luckyegg"_s, Item::Lucky_Egg},
			{"luckypunch"_s, Item::Lucky_Punch},
			{"lumberry"_s, Item::Lum_Berry},
			{"luminousmoss"_s, Item::Luminous_Moss},
			{"lumiosegalette"_s, Item::Lumiose_Galette},
			{"lunaliumz"_s, Item::Lunalium_Z},
			{"lureball"_s, Item::Lure_Ball},
			{"lustrousorb"_s, Item::Lustrous_Orb},
			{"luxuryball"_s, Item::Luxury_Ball},
			{"lycaniumz"_s, Item::Lycanium_Z},
			{"machobrace"_s, Item::Macho_Brace},
			{"magmarizer"_s, Item::Magmarizer},
			{"magnet"_s, Item::Magnet},
			{"magoberry"_s, Item::Mago_Berry},
			{"magostberry"_s, Item::Magost_Berry},
			{"mail"_s, Item::Eon_Mail},
			{"manectite"_s, Item::Manectite},
			{"marangaberry"_s, Item::Maranga_Berry},
			{"marshadiumz"_s, Item::Marshadium_Z},
			{"masterball"_s, Item::Master_Ball},
			{"mawilite"_s, Item::Mawilite},
			{"maxelixir"_s, Item::Max_Elixir},
			{"maxether"_s, Item::Max_Ether},
			{"maxpotion"_s, Item::Max_Potion},
			{"maxrepel"_s, Item::Max_Repel},
			{"maxrevive"_s, Item::Max_Revive},
			{"meadowplate"_s, Item::Meadow_Plate},
			{"mechmail"_s, Item::Mech_Mail},
			{"medichamite"_s, Item::Medichamite},
			{"mentalherb"_s, Item::Mental_Herb},
			{"metagrossite"_s, Item::Metagrossite},
			{"metalcoat"_s, Item::Metal_Coat},
			{"metalpowder"_s, Item::Metal_Powder},
			{"metronome"_s, Item::Metronome},
			{"mewniumz"_s, Item::Mewnium_Z},
			{"mewtwonitex"_s, Item::Mewtwonite_X},
			{"mewtwonitey"_s, Item::Mewtwonite_Y},
			{"micleberry"_s, Item::Micle_Berry},
			{"mildmint"_s, Item::Mild_Mint},
			{"mimikiumz"_s, Item::Mimikium_Z},
			{"mindplate"_s, Item::Mind_Plate},
			{"mintberry"_s, Item::Mint_Berry},
			{"miracleberry"_s, Item::MiracleBerry},
			{"miracleseed"_s, Item::Miracle_Seed},
			{"miragemail"_s, Item::Mirage_Mail},
			{"mistyseed"_s, Item::Misty_Seed},
			{"mixedmushrooms"_s, Item::Mixed_Mushrooms},
			{"modestmint"_s, Item::Modest_Mint},
			{"moomoocheese"_s, Item::Moomoo_Cheese},
			{"moomoomilk"_s, Item::Moomoo_Milk},
			{"moonball"_s, Item::Moon_Ball},
			{"moonstone"_s, Item::Moon_Stone},
			{"morphmail"_s, Item::Morph_Mail},
			{"mosaicmail"_s, Item::Mosaic_Mail},
			{"muscleband"_s, Item::Muscle_Band},
			{"musclewing"_s, Item::Muscle_Wing},
			{"musicmail"_s, Item::Music_Mail},
			{"mysteryberry"_s, Item::MysteryBerry},
			{"mysticwater"_s, Item::Mystic_Water},
			{"naivemint"_s, Item::Naive_Mint},
			{"nanabberry"_s, Item::Nanab_Berry},
			{"naughtymint"_s, Item::Naughty_Mint},
			{"nestball"_s, Item::Nest_Ball},
			{"netball"_s, Item::Net_Ball},
			{"nevermeltice"_s, Item::Never_Melt_Ice},
			{"ninikuberry"_s, Item::Niniku_Berry},
			{"nomelberry"_s, Item::Nomel_Berry},
			{"none"_s, Item::None},
			{"normalbox"_s, Item::Normal_Box},
			{"normalgem"_s, Item::Normal_Gem},
			{"normaliumz"_s, Item::Normalium_Z},
			{"nugget"_s, Item::Nugget},
			{"nutpeaberry"_s, Item::Nutpea_Berry},
			{"occaberry"_s, Item::Occa_Berry},
			{"oddincense"_s, Item::Odd_Incense},
			{"oddkeystone"_s, Item::Odd_Keystone},
			{"oldamber"_s, Item::Old_Amber},
			{"oldgateau"_s, Item::Old_Gateau},
			{"oranberry"_s, Item::Oran_Berry},
			{"orangemail"_s, Item::Orange_Mail},
			{"ovalstone"_s, Item::Oval_Stone},
			{"packagedcurry"_s, Item::Packaged_Curry},
			{"packofpotatoes"_s, Item::Pack_of_Potatoes},
			{"pamtreberry"_s, Item::Pamtre_Berry},
			{"paralyzeheal"_s, Item::Paralyze_Heal},
			{"parkball"_s, Item::Park_Ball},
			{"parlyzheal"_s, Item::Paralyze_Heal},
			{"passhoberry"_s, Item::Passho_Berry},
			{"passorb"_s, Item::Pass_Orb},
			{"pasta"_s, Item::Pasta},
			{"payapaberry"_s, Item::Payapa_Berry},
			{"pearl"_s, Item::Pearl},
			{"pearlstring"_s, Item::Pearl_String},
			{"pechaberry"_s, Item::Pecha_Berry},
			{"persimberry"_s, Item::Persim_Berry},
			{"petayaberry"_s, Item::Petaya_Berry},
			{"pewtercrunchies"_s, Item::Pewter_Crunchies},
			{"pidgeotite"_s, Item::Pidgeotite},
			{"pikaniumz"_s, Item::Pikanium_Z},
			{"pikashuniumz"_s, Item::Pikashunium_Z},
			{"pinapberry"_s, Item::Pinap_Berry},
			{"pinkapricorn"_s, Item::Pink_Apricorn},
			{"pinkbow"_s, Item::Pink_Bow},
			{"pinknectar"_s, Item::Pink_Nectar},
			{"pinkscarf"_s, Item::Pink_Scarf},
			{"pinsirite"_s, Item::Pinsirite},
			{"pixieplate"_s, Item::Pixie_Plate},
			{"plumefossil"_s, Item::Plume_Fossil},
			{"poisonbarb"_s, Item::Poison_Barb},
			{"poisongem"_s, Item::Poison_Gem},
			{"poisoniumz"_s, Item::Poisonium_Z},
			{"poisonmemory"_s, Item::Poison_Memory},
			{"pokeball"_s, Item::Poke_Ball},
			{"pokedoll"_s, Item::Poke_Doll},
			{"poketoy"_s, Item::Poke_Toy},
			{"polkadotbow"_s, Item::Polkadot_Bow},
			{"pomegberry"_s, Item::Pomeg_Berry},
			{"portraitmail"_s, Item::Portraitmail},
			{"potion"_s, Item::Potion},
			{"poweranklet"_s, Item::Power_Anklet},
			{"powerband"_s, Item::Power_Band},
			{"powerbelt"_s, Item::Power_Belt},
			{"powerbracer"_s, Item::Power_Bracer},
			{"powerherb"_s, Item::Power_Herb},
			{"powerlens"_s, Item::Power_Lens},
			{"powerweight"_s, Item::Power_Weight},
			{"ppmax"_s, Item::PP_Max},
			{"ppup"_s, Item::PP_Up},
			{"precookedburger"_s, Item::Precooked_Burger},
			{"premierball"_s, Item::Premier_Ball},
			{"prettywing"_s, Item::Pretty_Wing},
			{"primariumz"_s, Item::Primarium_Z},
			{"prismscale"_s, Item::Prism_Scale},
			{"protectivepads"_s, Item::Protective_Pads},
			{"protector"_s, Item::Protector},
			{"protein"_s, Item::Protein},
			{"przcureberry"_s, Item::PRZCureBerry},
			{"psncureberry"_s, Item::PSNCureBerry},
			{"psychicgem"_s, Item::Psychic_Gem},
			{"psychicmemory"_s, Item::Psychic_Memory},
			{"psychicseed"_s, Item::Psychic_Seed},
			{"psychiumz"_s, Item::Psychium_Z},
			{"pumkinberry"_s, Item::Pumkin_Berry},
			{"pungentroot"_s, Item::Pungent_Root},
			{"pureincense"_s, Item::Pure_Incense},
			{"purplenectar"_s, Item::Purple_Nectar},
			{"qualotberry"_s, Item::Qualot_Berry},
			{"quickball"_s, Item::Quick_Ball},
			{"quickclaw"_s, Item::Quick_Claw},
			{"quickpowder"_s, Item::Quick_Powder},
			{"quietmint"_s, Item::Quiet_Mint},
			{"rabutaberry"_s, Item::Rabuta_Berry},
			{"ragecandybar"_s, Item::Rage_Candy_Bar},
			{"rarebone"_s, Item::Rare_Bone},
			{"rarecandy"_s, Item::Rare_Candy},
			{"rashmint"_s, Item::Rash_Mint},
			{"rawstberry"_s, Item::Rawst_Berry},
			{"razorclaw"_s, Item::Razor_Claw},
			{"razorfang"_s, Item::Razor_Fang},
			{"razzberry"_s, Item::Razz_Berry},
			{"reapercloth"_s, Item::Reaper_Cloth},
			{"redapricorn"_s, Item::Red_Apricorn},
			{"redcard"_s, Item::Red_Card},
			{"redflute"_s, Item::Red_Flute},
			{"redorb"_s, Item::Red_Orb},
			{"redscarf"_s, Item::Red_Scarf},
			{"redshard"_s, Item::Red_Shard},
			{"relaxedmint"_s, Item::Relaxed_Mint},
			{"relicband"_s, Item::Relic_Band},
			{"reliccopper"_s, Item::Relic_Copper},
			{"reliccrown"_s, Item::Relic_Crown},
			{"relicgold"_s, Item::Relic_Gold},
			{"relicsilver"_s, Item::Relic_Silver},
			{"relicstatue"_s, Item::Relic_Statue},
			{"relicvase"_s, Item::Relic_Vase},
			{"repeatball"_s, Item::Repeat_Ball},
			{"repel"_s, Item::Repel},
			{"replymail"_s, Item::Reply_Mail},
			{"resistwing"_s, Item::Resist_Wing},
			{"retromail"_s, Item::Retro_Mail},
			{"revivalherb"_s, Item::Revival_Herb},
			{"revive"_s, Item::Revive},
			{"ribbonsweet"_s, Item::Ribbon_Sweet},
			{"richmulch"_s, Item::Rich_Mulch},
			{"rindoberry"_s, Item::Rindo_Berry},
			{"ringtarget"_s, Item::Ring_Target},
			{"rockgem"_s, Item::Rock_Gem},
			{"rockincense"_s, Item::Rock_Incense},
			{"rockiumz"_s, Item::Rockium_Z},
			{"rockmemory"_s, Item::Rock_Memory},
			{"rockyhelmet"_s, Item::Rocky_Helmet},
			{"roomservice"_s, Item::Room_Service},
			{"rootfossil"_s, Item::Root_Fossil},
			{"roseincense"_s, Item::Rose_Incense},
			{"roseliberry"_s, Item::Roseli_Berry},
			{"rowapberry"_s, Item::Rowap_Berry},
			{"rsvpmail"_s, Item::RSVP_Mail},
			{"rustedshield"_s, Item::Rusted_Shield},
			{"rustedsword"_s, Item::Rusted_Sword},
			{"sablenite"_s, Item::Sablenite},
			{"sachet"_s, Item::Sachet},
			{"sacredash"_s, Item::Sacred_Ash},
			{"safariball"_s, Item::Safari_Ball},
			{"safetygoggles"_s, Item::Safety_Goggles},
			{"sailfossil"_s, Item::Sail_Fossil},
			{"salacberry"_s, Item::Salac_Berry},
			{"saladmix"_s, Item::Salad_Mix},
			{"salamencite"_s, Item::Salamencite},
			{"sassymint"_s, Item::Sassy_Mint},
			{"sausages"_s, Item::Sausages},
			{"sceptilite"_s, Item::Sceptilite},
			{"scizorite"_s, Item::Scizorite},
			{"scopelens"_s, Item::Scope_Lens},
			{"seaincense"_s, Item::Sea_Incense},
			{"seriousmint"_s, Item::Serious_Mint},
			{"shadowmail"_s, Item::Shadow_Mail},
			{"shaloursable"_s, Item::Shalour_Sable},
			{"sharpbeak"_s, Item::Sharp_Beak},
			{"sharpedonite"_s, Item::Sharpedonite},
			{"shedshell"_s, Item::Shed_Shell},
			{"shellbell"_s, Item::Shell_Bell},
			{"shinystone"_s, Item::Shiny_Stone},
			{"shoalsalt"_s, Item::Shoal_Salt},
			{"shoalshell"_s, Item::Shoal_Shell},
			{"shockdrive"_s, Item::Shock_Drive},
			{"shucaberry"_s, Item::Shuca_Berry},
			{"silkscarf"_s, Item::Silk_Scarf},
			{"silverleaf"_s, Item::Silver_Leaf},
			{"silverpowder"_s, Item::Silver_Powder},
			{"sitrusberry"_s, Item::Sitrus_Berry},
			{"skullfossil"_s, Item::Skull_Fossil},
			{"skyplate"_s, Item::Sky_Plate},
			{"slowbronite"_s, Item::Slowbronite},
			{"slowpoketail"_s, Item::Slowpoke_Tail},
			{"smokeball"_s, Item::Smoke_Ball},
			{"smokepoketail"_s, Item::Smoke_Poke_Tail},
			{"smoothrock"_s, Item::Smooth_Rock},
			{"snorliumz"_s, Item::Snorlium_Z},
			{"snowball"_s, Item::Snowball},
			{"snowmail"_s, Item::Snow_Mail},
			{"sodapop"_s, Item::Soda_Pop},
			{"softsand"_s, Item::Soft_Sand},
			{"solganiumz"_s, Item::Solganium_Z},
			{"soothebell"_s, Item::Soothe_Bell},
			{"souldew"_s, Item::Soul_Dew},
			{"spacemail"_s, Item::Space_Mail},
			{"spelltag"_s, Item::Spell_Tag},
			{"spelonberry"_s, Item::Spelon_Berry},
			{"spicemix"_s, Item::Spice_Mix},
			{"splashplate"_s, Item::Splash_Plate},
			{"spookyplate"_s, Item::Spooky_Plate},
			{"sportball"_s, Item::Sport_Ball},
			{"stablemulch"_s, Item::Stable_Mulch},
			{"stardust"_s, Item::Stardust},
			{"starfberry"_s, Item::Starf_Berry},
			{"starpiece"_s, Item::Star_Piece},
			{"starsweet"_s, Item::Star_Sweet},
			{"steelgem"_s, Item::Steel_Gem},
			{"steeliumz"_s, Item::Steelium_Z},
			{"steelixite"_s, Item::Steelixite},
			{"steelmail"_s, Item::Steel_Mail},
			{"steelmemory"_s, Item::Steel_Memory},
			{"stick"_s, Item::Stick},
			{"stickybarb"_s, Item::Sticky_Barb},
			{"stoneplate"_s, Item::Stone_Plate},
			{"strangesouvenir"_s, Item::Strange_Souvenir},
			{"strawberrysweet"_s, Item::Strawberry_Sweet},
			{"stribberry"_s, Item::Strib_Berry},
			{"sunstone"_s, Item::Sun_Stone},
			{"superpotion"_s, Item::Super_Potion},
			{"superrepel"_s, Item::Super_Repel},
			{"surfmail"_s, Item::Surf_Mail},
			{"surprisemulch"_s, Item::Surprise_Mulch},
			{"swampertite"_s, Item::Swampertite},
			{"sweetapple"_s, Item::Sweet_Apple},
			{"sweetheart"_s, Item::Sweet_Heart},
			{"swiftwing"_s, Item::Swift_Wing},
			{"tamatoberry"_s, Item::Tamato_Berry},
			{"tangaberry"_s, Item::Tanga_Berry},
			{"tapuniumz"_s, Item::Tapunium_Z},
			{"tartapple"_s, Item::Tart_Apple},
			{"terrainextender"_s, Item::Terrain_Extender},
			{"thanksmail"_s, Item::Thanks_Mail},
			{"thickclub"_s, Item::Thick_Club},
			{"throatspray"_s, Item::Throat_Spray},
			{"thunderstone"_s, Item::Thunder_Stone},
			{"timerball"_s, Item::Timer_Ball},
			{"timidmint"_s, Item::Timid_Mint},
			{"tinofbeans"_s, Item::Tin_of_Beans},
			{"tinymushroom"_s, Item::Tiny_Mushroom},
			{"tm01"_s, Item::TM01},
			{"tm02"_s, Item::TM02},
			{"tm03"_s, Item::TM03},
			{"tm04"_s, Item::TM04},
			{"tm05"_s, Item::TM05},
			{"tm06"_s, Item::TM06},
			{"tm07"_s, Item::TM07},
			{"tm08"_s, Item::TM08},
			{"tm09"_s, Item::TM09},
			{"tm10"_s, Item::TM10},
			{"tm11"_s, Item::TM11},
			{"tm12"_s, Item::TM12},
			{"tm13"_s, Item::TM13},
			{"tm14"_s, Item::TM14},
			{"tm15"_s, Item::TM15},
			{"tm16"_s, Item::TM16},
			{"tm17"_s, Item::TM17},
			{"tm18"_s, Item::TM18},
			{"tm19"_s, Item::TM19},
			{"tm20"_s, Item::TM20},
			{"tm21"_s, Item::TM21},
			{"tm22"_s, Item::TM22},
			{"tm23"_s, Item::TM23},
			{"tm24"_s, Item::TM24},
			{"tm25"_s, Item::TM25},
			{"tm26"_s, Item::TM26},
			{"tm27"_s, Item::TM27},
			{"tm28"_s, Item::TM28},
			{"tm29"_s, Item::TM29},
			{"tm30"_s, Item::TM30},
			{"tm31"_s, Item::TM31},
			{"tm32"_s, Item::TM32},
			{"tm33"_s, Item::TM33},
			{"tm34"_s, Item::TM34},
			{"tm35"_s, Item::TM35},
			{"tm36"_s, Item::TM36},
			{"tm37"_s, Item::TM37},
			{"tm38"_s, Item::TM38},
			{"tm39"_s, Item::TM39},
			{"tm40"_s, Item::TM40},
			{"tm41"_s, Item::TM41},
			{"tm42"_s, Item::TM42},
			{"tm43"_s, Item::TM43},
			{"tm44"_s, Item::TM44},
			{"tm45"_s, Item::TM45},
			{"tm46"_s, Item::TM46},
			{"tm47"_s, Item::TM47},
			{"tm48"_s, Item::TM48},
			{"tm49"_s, Item::TM49},
			{"tm50"_s, Item::TM50},
			{"tm51"_s, Item::TM51},
			{"tm52"_s, Item::TM52},
			{"tm53"_s, Item::TM53},
			{"tm54"_s, Item::TM54},
			{"tm55"_s, Item::TM55},
			{"tm56"_s, Item::TM56},
			{"tm57"_s, Item::TM57},
			{"tm58"_s, Item::TM58},
			{"tm59"_s, Item::TM59},
			{"tm60"_s, Item::TM60},
			{"tm61"_s, Item::TM61},
			{"tm62"_s, Item::TM62},
			{"tm63"_s, Item::TM63},
			{"tm64"_s, Item::TM64},
			{"tm65"_s, Item::TM65},
			{"tm66"_s, Item::TM66},
			{"tm67"_s, Item::TM67},
			{"tm68"_s, Item::TM68},
			{"tm69"_s, Item::TM69},
			{"tm70"_s, Item::TM70},
			{"tm71"_s, Item::TM71},
			{"tm72"_s, Item::TM72},
			{"tm73"_s, Item::TM73},
			{"tm74"_s, Item::TM74},
			{"tm75"_s, Item::TM75},
			{"tm76"_s, Item::TM76},
			{"tm77"_s, Item::TM77},
			{"tm78"_s, Item::TM78},
			{"tm79"_s, Item::TM79},
			{"tm80"_s, Item::TM80},
			{"tm81"_s, Item::TM81},
			{"tm82"_s, Item::TM82},
			{"tm83"_s, Item::TM83},
			{"tm84"_s, Item::TM84},
			{"tm85"_s, Item::TM85},
			{"tm86"_s, Item::TM86},
			{"tm87"_s, Item::TM87},
			{"tm88"_s, Item::TM88},
			{"tm89"_s, Item::TM89},
			{"tm90"_s, Item::TM90},
			{"tm91"_s, Item::TM91},
			{"tm92"_s, Item::TM92},
			{"topoberry"_s, Item::Topo_Berry},
			{"tougaberry"_s, Item::Touga_Berry},
			{"toxicorb"_s, Item::Toxic_Orb},
			{"toxicplate"_s, Item::Toxic_Plate},
			{"tr00"_s, Item::TR00},
			{"tr01"_s, Item::TR01},
			{"tr02"_s, Item::TR02},
			{"tr03"_s, Item::TR03},
			{"tr04"_s, Item::TR04},
			{"tr05"_s, Item::TR05},
			{"tr06"_s, Item::TR06},
			{"tr07"_s, Item::TR07},
			{"tr08"_s, Item::TR08},
			{"tr09"_s, Item::TR09},
			{"tr10"_s, Item::TR10},
			{"tr11"_s, Item::TR11},
			{"tr12"_s, Item::TR12},
			{"tr13"_s, Item::TR13},
			{"tr14"_s, Item::TR14},
			{"tr15"_s, Item::TR15},
			{"tr16"_s, Item::TR16},
			{"tr17"_s, Item::TR17},
			{"tr18"_s, Item::TR18},
			{"tr19"_s, Item::TR19},
			{"tr20"_s, Item::TR20},
			{"tr21"_s, Item::TR21},
			{"tr22"_s, Item::TR22},
			{"tr23"_s, Item::TR23},
			{"tr24"_s, Item::TR24},
			{"tr25"_s, Item::TR25},
			{"tr26"_s, Item::TR26},
			{"tr27"_s, Item::TR27},
			{"tr28"_s, Item::TR28},
			{"tr29"_s, Item::TR29},
			{"tr30"_s, Item::TR30},
			{"tr31"_s, Item::TR31},
			{"tr32"_s, Item::TR32},
			{"tr33"_s, Item::TR33},
			{"tr34"_s, Item::TR34},
			{"tr35"_s, Item::TR35},
			{"tr36"_s, Item::TR36},
			{"tr37"_s, Item::TR37},
			{"tr38"_s, Item::TR38},
			{"tr39"_s, Item::TR39},
			{"tr40"_s, Item::TR40},
			{"tr41"_s, Item::TR41},
			{"tr42"_s, Item::TR42},
			{"tr43"_s, Item::TR43},
			{"tr44"_s, Item::TR44},
			{"tr45"_s, Item::TR45},
			{"tr46"_s, Item::TR46},
			{"tr47"_s, Item::TR47},
			{"tr48"_s, Item::TR48},
			{"tr49"_s, Item::TR49},
			{"tr50"_s, Item::TR50},
			{"tr51"_s, Item::TR51},
			{"tr52"_s, Item::TR52},
			{"tr53"_s, Item::TR53},
			{"tr54"_s, Item::TR54},
			{"tr55"_s, Item::TR55},
			{"tr56"_s, Item::TR56},
			{"tr57"_s, Item::TR57},
			{"tr58"_s, Item::TR58},
			{"tr59"_s, Item::TR59},
			{"tr60"_s, Item::TR60},
			{"tr61"_s, Item::TR61},
			{"tr62"_s, Item::TR62},
			{"tr63"_s, Item::TR63},
			{"tr64"_s, Item::TR64},
			{"tr65"_s, Item::TR65},
			{"tr66"_s, Item::TR66},
			{"tr67"_s, Item::TR67},
			{"tr68"_s, Item::TR68},
			{"tr69"_s, Item::TR69},
			{"tr70"_s, Item::TR70},
			{"tr71"_s, Item::TR71},
			{"tr72"_s, Item::TR72},
			{"tr73"_s, Item::TR73},
			{"tr74"_s, Item::TR74},
			{"tr75"_s, Item::TR75},
			{"tr76"_s, Item::TR76},
			{"tr77"_s, Item::TR77},
			{"tr78"_s, Item::TR78},
			{"tr79"_s, Item::TR79},
			{"tr80"_s, Item::TR80},
			{"tr81"_s, Item::TR81},
			{"tr82"_s, Item::TR82},
			{"tr83"_s, Item::TR83},
			{"tr84"_s, Item::TR84},
			{"tr85"_s, Item::TR85},
			{"tr86"_s, Item::TR86},
			{"tr87"_s, Item::TR87},
			{"tr88"_s, Item::TR88},
			{"tr89"_s, Item::TR89},
			{"tr90"_s, Item::TR90},
			{"tr91"_s, Item::TR91},
			{"tr92"_s, Item::TR92},
			{"tr93"_s, Item::TR93},
			{"tr94"_s, Item::TR94},
			{"tr95"_s, Item::TR95},
			{"tr96"_s, Item::TR96},
			{"tr97"_s, Item::TR97},
			{"tr98"_s, Item::TR98},
			{"tr99"_s, Item::TR99},
			{"tropicmail"_s, Item::Tropic_Mail},
			{"tunnelmail"_s, Item::Tunnel_Mail},
			{"twistedspoon"_s, Item::Twisted_Spoon},
			{"tyranitarite"_s, Item::Tyranitarite},
			{"ultraball"_s, Item::Ultra_Ball},
			{"ultranecroziumz"_s, Item::Ultranecrozium_Z},
			{"upgrade"_s, Item::Up_Grade},
			{"utilityumbrella"_s, Item::Utility_Umbrella},
			{"venusaurite"_s, Item::Venusaurite},
			{"wacanberry"_s, Item::Wacan_Berry},
			{"watergem"_s, Item::Water_Gem},
			{"wateriumz"_s, Item::Waterium_Z},
			{"watermemory"_s, Item::Water_Memory},
			{"waterstone"_s, Item::Water_Stone},
			{"watmelberry"_s, Item::Watmel_Berry},
			{"waveincense"_s, Item::Wave_Incense},
			{"wavemail"_s, Item::Wave_Mail},
			{"weaknesspolicy"_s, Item::Weakness_Policy},
			{"wepearberry"_s, Item::Wepear_Berry},
			{"whippeddream"_s, Item::Whipped_Dream},
			{"whiteapricorn"_s, Item::White_Apricorn},
			{"whiteflute"_s, Item::White_Flute},
			{"whiteherb"_s, Item::White_Herb},
			{"widelens"_s, Item::Wide_Lens},
			{"wikiberry"_s, Item::Wiki_Berry},
			{"wiseglasses"_s, Item::Wise_Glasses},
			{"woodmail"_s, Item::Wood_Mail},
			{"xaccuracy"_s, Item::X_Accuracy},
			{"xattack"_s, Item::X_Attack},
			{"xdefend"_s, Item::X_Defend},
			{"xdefense"_s, Item::X_Defense},
			{"xspatk"_s, Item::X_Sp_Atk},
			{"xspdef"_s, Item::X_Sp_Def},
			{"xspecial"_s, Item::X_Special},
			{"xspeed"_s, Item::X_Speed},
			{"yacheberry"_s, Item::Yache_Berry},
			{"yagoberry"_s, Item::Yago_Berry},
			{"yellowapricorn"_s, Item::Yellow_Apricorn},
			{"yellowflute"_s, Item::Yellow_Flute},
			{"yellownectar"_s, Item::Yellow_Nectar},
			{"yellowscarf"_s, Item::Yellow_Scarf},
			{"yellowshard"_s, Item::Yellow_Shard},
			{"zapplate"_s, Item::Zap_Plate},
			{"zinc"_s, Item::Zinc},
			{"zoomlens"_s, Item::Zoom_Lens},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<15_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Item"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
