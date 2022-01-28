// Item string conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/item.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/item.hpp>

#include <containers/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {

using namespace bounded::literal;

auto to_string(Item const item) -> std::string_view {
	switch (item) {
		// Generation 1
		case Item::None: return "None";

		case Item::Antidote: return "Antidote";
		case Item::Awakening: return "Awakening";
		case Item::Burn_Heal: return "Burn Heal";
		case Item::Calcium: return "Calcium";
		case Item::Carbos: return "Carbos";
		case Item::Dire_Hit: return "Dire Hit";
		case Item::Dome_Fossil: return "Dome Fossil";
		case Item::Elixir: return "Elixir";
		case Item::Escape_Rope: return "Escape Rope";
		case Item::Ether: return "Ether";
		case Item::Exp_Share: return "Exp. Share";
		case Item::Fire_Stone: return "Fire Stone";
		case Item::Fresh_Water: return "Fresh Water";
		case Item::Full_Heal: return "Full Heal";
		case Item::Full_Restore: return "Full Restore";
		case Item::Great_Ball: return "Great Ball";
		case Item::Guard_Spec: return "Guard Spec.";
		case Item::Helix_Fossil: return "Helix Fossil";
		case Item::HP_Up: return "HP Up";
		case Item::Hyper_Potion: return "Hyper Potion";
		case Item::Ice_Heal: return "Ice Heal";
		case Item::Iron: return "Iron";
		case Item::Leaf_Stone: return "Leaf Stone";
		case Item::Lemonade: return "Lemonade";
		case Item::Master_Ball: return "Master Ball";
		case Item::Max_Elixir: return "Max Elixir";
		case Item::Max_Ether: return "Max Ether";
		case Item::Max_Potion: return "Max Potion";
		case Item::Max_Repel: return "Max Repel";
		case Item::Max_Revive: return "Max Revive";
		case Item::Moon_Stone: return "Moon Stone";
		case Item::Nugget: return "Nugget";
		case Item::Old_Amber: return "Old Amber";
		case Item::Paralyze_Heal: return "Paralyze Heal";
		case Item::Poke_Ball: return "Poke Ball";
		case Item::Poke_Doll: return "Poke Doll";
		case Item::Potion: return "Potion";
		case Item::PP_Up: return "PP Up";
		case Item::Protein: return "Protein";
		case Item::Rare_Candy: return "Rare Candy";
		case Item::Repel: return "Repel";
		case Item::Revive: return "Revive";
		case Item::Safari_Ball: return "Safari Ball";
		case Item::Soda_Pop: return "Soda Pop";
		case Item::Super_Potion: return "Super Potion";
		case Item::Super_Repel: return "Super Repel";
		case Item::Thunder_Stone: return "Thunder Stone";
		case Item::TM01: return "TM01";
		case Item::TM02: return "TM02";
		case Item::TM03: return "TM03";
		case Item::TM04: return "TM04";
		case Item::TM05: return "TM05";
		case Item::TM06: return "TM06";
		case Item::TM07: return "TM07";
		case Item::TM08: return "TM08";
		case Item::TM09: return "TM09";
		case Item::TM10: return "TM10";
		case Item::TM11: return "TM11";
		case Item::TM12: return "TM12";
		case Item::TM13: return "TM13";
		case Item::TM14: return "TM14";
		case Item::TM15: return "TM15";
		case Item::TM16: return "TM16";
		case Item::TM17: return "TM17";
		case Item::TM18: return "TM18";
		case Item::TM19: return "TM19";
		case Item::TM20: return "TM20";
		case Item::TM21: return "TM21";
		case Item::TM22: return "TM22";
		case Item::TM23: return "TM23";
		case Item::TM24: return "TM24";
		case Item::TM25: return "TM25";
		case Item::TM26: return "TM26";
		case Item::TM27: return "TM27";
		case Item::TM28: return "TM28";
		case Item::TM29: return "TM29";
		case Item::TM30: return "TM30";
		case Item::TM31: return "TM31";
		case Item::TM32: return "TM32";
		case Item::TM33: return "TM33";
		case Item::TM34: return "TM34";
		case Item::TM35: return "TM35";
		case Item::TM36: return "TM36";
		case Item::TM37: return "TM37";
		case Item::TM38: return "TM38";
		case Item::TM39: return "TM39";
		case Item::TM40: return "TM40";
		case Item::TM41: return "TM41";
		case Item::TM42: return "TM42";
		case Item::TM43: return "TM43";
		case Item::TM44: return "TM44";
		case Item::TM45: return "TM45";
		case Item::TM46: return "TM46";
		case Item::TM47: return "TM47";
		case Item::TM48: return "TM48";
		case Item::TM49: return "TM49";
		case Item::TM50: return "TM50";
		case Item::Ultra_Ball: return "Ultra Ball";
		case Item::Water_Stone: return "Water Stone";
		case Item::X_Accuracy: return "X Accuracy";
		case Item::X_Attack: return "X Attack";
		case Item::X_Defend: return "X Defend";
		case Item::X_Special: return "X Special";
		case Item::X_Speed: return "X Speed";
		
		// Generation 2
		case Item::Amulet_Coin: return "Amulet Coin";
		case Item::Berry_Juice: return "Berry Juice";
		case Item::Berry: return "Berry";
		case Item::Berserk_Gene: return "Berserk Gene";
		case Item::Big_Mushroom: return "Big Mushroom";
		case Item::Big_Pearl: return "Big Pearl";
		case Item::Bitter_Berry: return "Bitter Berry";
		case Item::Black_Apricorn: return "Black Apricorn";
		case Item::Black_Belt: return "Black Belt";
		case Item::Black_Glasses: return "Black Glasses";
		case Item::Blue_Apricorn: return "Blue Apricorn";
		case Item::Bluesky_Mail: return "Bluesky Mail";
		case Item::Brick_Piece: return "Brick Piece";
		case Item::Bright_Powder: return "Bright Powder";
		case Item::Burnt_Berry: return "Burnt Berry";
		case Item::Charcoal: return "Charcoal";
		case Item::Cleanse_Tag: return "Cleanse Tag";
		case Item::Dragon_Fang: return "Dragon Fang";
		case Item::Dragon_Scale: return "Dragon Scale";
		case Item::Energy_Powder: return "Energy Powder";
		case Item::Energy_Root: return "Energy Root";
		case Item::Eon_Mail: return "Eon Mail";
		case Item::Everstone: return "Everstone";
		case Item::Fast_Ball: return "Fast Ball";
		case Item::Flower_Mail: return "Flower Mail";
		case Item::Focus_Band: return "Focus Band";
		case Item::Friend_Ball: return "Friend Ball";
		case Item::Gold_Berry: return "Gold Berry";
		case Item::Gold_Leaf: return "Gold Leaf";
		case Item::Gorgeous_Box: return "Gorgeous Box";
		case Item::Green_Apricorn: return "Green Apricorn";
		case Item::Hard_Stone: return "Hard Stone";
		case Item::Heal_Powder: return "Heal Powder";
		case Item::Heavy_Ball: return "Heavy Ball";
		case Item::Ice_Berry: return "Ice Berry";
		case Item::Kings_Rock: return "King's Rock";
		case Item::Stick: return "Stick";
		case Item::Leftovers: return "Leftovers";
		case Item::Level_Ball: return "Level Ball";
		case Item::Light_Ball: return "Light Ball";
		case Item::Litebluemail: return "Litebluemail";
		case Item::Love_Ball: return "Love Ball";
		case Item::Lovely_Mail: return "Lovely Mail";
		case Item::Lucky_Egg: return "Lucky Egg";
		case Item::Lucky_Punch: return "Lucky Punch";
		case Item::Lure_Ball: return "Lure Ball";
		case Item::Magnet: return "Magnet";
		case Item::Metal_Coat: return "Metal Coat";
		case Item::Metal_Powder: return "Metal Powder";
		case Item::Mint_Berry: return "Mint Berry";
		case Item::Miracle_Seed: return "Miracle Seed";
		case Item::MiracleBerry: return "MiracleBerry";
		case Item::Mirage_Mail: return "Mirage Mail";
		case Item::Moomoo_Milk: return "Moomoo Milk";
		case Item::Moon_Ball: return "Moon Ball";
		case Item::Morph_Mail: return "Morph Mail";
		case Item::Music_Mail: return "Music Mail";
		case Item::MysteryBerry: return "MysteryBerry";
		case Item::Mystic_Water: return "Mystic Water";
		case Item::Never_Melt_Ice: return "Never-Melt Ice";
		case Item::Normal_Box: return "Normal Box";
		case Item::Pearl: return "Pearl";
		case Item::Pink_Apricorn: return "Pink Apricorn";
		case Item::Pink_Bow: return "Pink Bow";
		case Item::Poison_Barb: return "Poison Barb";
		case Item::Polkadot_Bow: return "Polkadot Bow";
		case Item::Portraitmail: return "Portraitmail";
		case Item::PRZCureBerry: return "PRZCureBerry";
		case Item::PSNCureBerry: return "PSNCureBerry";
		case Item::Quick_Claw: return "Quick Claw";
		case Item::Rage_Candy_Bar: return "Rage Candy Bar";
		case Item::Red_Apricorn: return "Red Apricorn";
		case Item::Revival_Herb: return "Revival Herb";
		case Item::Sacred_Ash: return "Sacred Ash";
		case Item::Scope_Lens: return "Scope Lens";
		case Item::Sharp_Beak: return "Sharp Beak";
		case Item::Silver_Leaf: return "Silver Leaf";
		case Item::Silver_Powder: return "Silver Powder";
		case Item::Slowpoke_Tail: return "Slowpoke Tail";
		case Item::Smoke_Ball: return "Smoke Ball";
		case Item::Soft_Sand: return "Soft Sand";
		case Item::Spell_Tag: return "Spell Tag";
		case Item::Sport_Ball: return "Sport Ball";
		case Item::Star_Piece: return "Star Piece";
		case Item::Stardust: return "Stardust";
		case Item::Sun_Stone: return "Sun Stone";
		case Item::Surf_Mail: return "Surf Mail";
		case Item::Thick_Club: return "Thick Club";
		case Item::Tiny_Mushroom: return "Tiny Mushroom";
		case Item::Twisted_Spoon: return "Twisted Spoon";
		case Item::Up_Grade: return "Up-Grade";
		case Item::White_Apricorn: return "White Apricorn";
		case Item::Yellow_Apricorn: return "Yellow Apricorn";

		// Generation 3
		case Item::Aguav_Berry: return "Aguav Berry";
		case Item::Apicot_Berry: return "Apicot Berry";
		case Item::Aspear_Berry: return "Aspear Berry";
		case Item::Bead_Mail: return "Bead Mail";
		case Item::Belue_Berry: return "Belue Berry";
		case Item::Black_Flute: return "Black Flute";
		case Item::Blue_Flute: return "Blue Flute";
		case Item::Blue_Orb: return "Blue Orb";
		case Item::Blue_Scarf: return "Blue Scarf";
		case Item::Blue_Shard: return "Blue Shard";
		case Item::Bluk_Berry: return "Bluk Berry";
		case Item::Cheri_Berry: return "Cheri Berry";
		case Item::Chesto_Berry: return "Chesto Berry";
		case Item::Choice_Band: return "Choice Band";
		case Item::Claw_Fossil: return "Claw Fossil";
		case Item::Cornn_Berry: return "Cornn Berry";
		case Item::Deep_Sea_Scale: return "Deep Sea Scale";
		case Item::Deep_Sea_Tooth: return "Deep Sea Tooth";
		case Item::Dive_Ball: return "Dive Ball";
		case Item::Drash_Berry: return "Drash Berry";
		case Item::Dream_Mail: return "Dream Mail";
		case Item::Durin_Berry: return "Durin Berry";
		case Item::Eggant_Berry: return "Eggant Berry";
		case Item::Enigma_Berry: return "Enigma Berry";
		case Item::Fab_Mail: return "Fab Mail";
		case Item::Figy_Berry: return "Figy Berry";
		case Item::Fluffy_Tail: return "Fluffy Tail";
		case Item::Ganlon_Berry: return "Ganlon Berry";
		case Item::Ginema_Berry: return "Ginema Berry";
		case Item::Glitter_Mail: return "Glitter Mail";
		case Item::Green_Scarf: return "Green Scarf";
		case Item::Green_Shard: return "Green Shard";
		case Item::Grepa_Berry: return "Grepa Berry";
		case Item::Harbor_Mail: return "Harbor Mail";
		case Item::Heart_Scale: return "Heart Scale";
		case Item::Hondew_Berry: return "Hondew Berry";
		case Item::Iapapa_Berry: return "Iapapa Berry";
		case Item::Kelpsy_Berry: return "Kelpsy Berry";
		case Item::Kuo_Berry: return "Kuo Berry";
		case Item::Lansat_Berry: return "Lansat Berry";
		case Item::Lava_Cookie: return "Lava Cookie";
		case Item::Lax_Incense: return "Lax Incense";
		case Item::Leppa_Berry: return "Leppa Berry";
		case Item::Liechi_Berry: return "Liechi Berry";
		case Item::Lum_Berry: return "Lum Berry";
		case Item::Luxury_Ball: return "Luxury Ball";
		case Item::Macho_Brace: return "Macho Brace";
		case Item::Mago_Berry: return "Mago Berry";
		case Item::Magost_Berry: return "Magost Berry";
		case Item::Mech_Mail: return "Mech Mail";
		case Item::Mental_Herb: return "Mental Herb";
		case Item::Nanab_Berry: return "Nanab Berry";
		case Item::Nest_Ball: return "Nest Ball";
		case Item::Net_Ball: return "Net Ball";
		case Item::Niniku_Berry: return "Niniku Berry";
		case Item::Nomel_Berry: return "Nomel Berry";
		case Item::Nutpea_Berry: return "Nutpea Berry";
		case Item::Oran_Berry: return "Oran Berry";
		case Item::Orange_Mail: return "Orange Mail";
		case Item::Pamtre_Berry: return "Pamtre Berry";
		case Item::Pecha_Berry: return "Pecha Berry";
		case Item::Persim_Berry: return "Persim Berry";
		case Item::Petaya_Berry: return "Petaya Berry";
		case Item::Pinap_Berry: return "Pinap Berry";
		case Item::Pink_Scarf: return "Pink Scarf";
		case Item::Pomeg_Berry: return "Pomeg Berry";
		case Item::PP_Max: return "PP Max";
		case Item::Premier_Ball: return "Premier Ball";
		case Item::Pumkin_Berry: return "Pumkin Berry";
		case Item::Qualot_Berry: return "Qualot Berry";
		case Item::Rabuta_Berry: return "Rabuta Berry";
		case Item::Rawst_Berry: return "Rawst Berry";
		case Item::Razz_Berry: return "Razz Berry";
		case Item::Red_Flute: return "Red Flute";
		case Item::Red_Orb: return "Red Orb";
		case Item::Red_Scarf: return "Red Scarf";
		case Item::Red_Shard: return "Red Shard";
		case Item::Repeat_Ball: return "Repeat Ball";
		case Item::Retro_Mail: return "Retro Mail";
		case Item::Root_Fossil: return "Root Fossil";
		case Item::Salac_Berry: return "Salac Berry";
		case Item::Sea_Incense: return "Sea Incense";
		case Item::Shadow_Mail: return "Shadow Mail";
		case Item::Shell_Bell: return "Shell Bell";
		case Item::Shoal_Salt: return "Shoal Salt";
		case Item::Shoal_Shell: return "Shoal Shell";
		case Item::Silk_Scarf: return "Silk Scarf";
		case Item::Sitrus_Berry: return "Sitrus Berry";
		case Item::Soothe_Bell: return "Soothe Bell";
		case Item::Soul_Dew: return "Soul Dew";
		case Item::Spelon_Berry: return "Spelon Berry";
		case Item::Starf_Berry: return "Starf Berry";
		case Item::Strib_Berry: return "Strib Berry";
		case Item::Tamato_Berry: return "Tamato Berry";
		case Item::Timer_Ball: return "Timer Ball";
		case Item::Topo_Berry: return "Topo Berry";
		case Item::Touga_Berry: return "Touga Berry";
		case Item::Tropic_Mail: return "Tropic Mail";
		case Item::Watmel_Berry: return "Watmel Berry";
		case Item::Wave_Mail: return "Wave Mail";
		case Item::Wepear_Berry: return "Wepear Berry";
		case Item::White_Flute: return "White Flute";
		case Item::White_Herb: return "White Herb";
		case Item::Wiki_Berry: return "Wiki Berry";
		case Item::Wood_Mail: return "Wood Mail";
		case Item::Yago_Berry: return "Yago Berry";
		case Item::Yellow_Flute: return "Yellow Flute";
		case Item::Yellow_Scarf: return "Yellow Scarf";
		case Item::Yellow_Shard: return "Yellow Shard";
		case Item::Zinc: return "Zinc";

		// Generation 4
		case Item::Adamant_Orb: return "Adamant Orb";
		case Item::Air_Mail: return "Air Mail";
		case Item::Armor_Fossil: return "Armor Fossil";
		case Item::Babiri_Berry: return "Babiri Berry";
		case Item::Big_Root: return "Big Root";
		case Item::Black_Sludge: return "Black Sludge";
		case Item::Bloom_Mail: return "Bloom Mail";
		case Item::Brick_Mail: return "Brick Mail";
		case Item::Bubble_Mail: return "Bubble Mail";
		case Item::Charti_Berry: return "Charti Berry";
		case Item::Cherish_Ball: return "Cherish Ball";
		case Item::Chilan_Berry: return "Chilan Berry";
		case Item::Choice_Scarf: return "Choice Scarf";
		case Item::Choice_Specs: return "Choice Specs";
		case Item::Chople_Berry: return "Chople Berry";
		case Item::Coba_Berry: return "Coba Berry";
		case Item::Colbur_Berry: return "Colbur Berry";
		case Item::Custap_Berry: return "Custap Berry";
		case Item::Damp_Mulch: return "Damp Mulch";
		case Item::Damp_Rock: return "Damp Rock";
		case Item::Dawn_Stone: return "Dawn Stone";
		case Item::Destiny_Knot: return "Destiny Knot";
		case Item::Draco_Plate: return "Draco Plate";
		case Item::Dread_Plate: return "Dread Plate";
		case Item::Dubious_Disc: return "Dubious Disc";
		case Item::Dusk_Ball: return "Dusk Ball";
		case Item::Dusk_Stone: return "Dusk Stone";
		case Item::Earth_Plate: return "Earth Plate";
		case Item::Electirizer: return "Electirizer";
		case Item::Expert_Belt: return "Expert Belt";
		case Item::Fist_Plate: return "Fist Plate";
		case Item::Flame_Mail: return "Flame Mail";
		case Item::Flame_Orb: return "Flame Orb";
		case Item::Flame_Plate: return "Flame Plate";
		case Item::Focus_Sash: return "Focus Sash";
		case Item::Full_Incense: return "Full Incense";
		case Item::Gooey_Mulch: return "Gooey Mulch";
		case Item::Grass_Mail: return "Grass Mail";
		case Item::Grip_Claw: return "Grip Claw";
		case Item::Griseous_Orb: return "Griseous Orb";
		case Item::Growth_Mulch: return "Growth Mulch";
		case Item::Haban_Berry: return "Haban Berry";
		case Item::Heal_Ball: return "Heal Ball";
		case Item::Heart_Mail: return "Heart Mail";
		case Item::Heat_Rock: return "Heat Rock";
		case Item::Honey: return "Honey";
		case Item::Icicle_Plate: return "Icicle Plate";
		case Item::Icy_Rock: return "Icy Rock";
		case Item::Insect_Plate: return "Insect Plate";
		case Item::Iron_Ball: return "Iron Ball";
		case Item::Iron_Plate: return "Iron Plate";
		case Item::Jaboca_Berry: return "Jaboca Berry";
		case Item::Kasib_Berry: return "Kasib Berry";
		case Item::Kebia_Berry: return "Kebia Berry";
		case Item::Lagging_Tail: return "Lagging Tail";
		case Item::Life_Orb: return "Life Orb";
		case Item::Light_Clay: return "Light Clay";
		case Item::Luck_Incense: return "Luck Incense";
		case Item::Lustrous_Orb: return "Lustrous Orb";
		case Item::Magmarizer: return "Magmarizer";
		case Item::Meadow_Plate: return "Meadow Plate";
		case Item::Metronome: return "Metronome";
		case Item::Micle_Berry: return "Micle Berry";
		case Item::Mind_Plate: return "Mind Plate";
		case Item::Mosaic_Mail: return "Mosaic Mail";
		case Item::Muscle_Band: return "Muscle Band";
		case Item::Occa_Berry: return "Occa Berry";
		case Item::Odd_Incense: return "Odd Incense";
		case Item::Odd_Keystone: return "Odd Keystone";
		case Item::Old_Gateau: return "Old Gateau";
		case Item::Oval_Stone: return "Oval Stone";
		case Item::Park_Ball: return "Park Ball";
		case Item::Passho_Berry: return "Passho Berry";
		case Item::Payapa_Berry: return "Payapa Berry";
		case Item::Power_Anklet: return "Power Anklet";
		case Item::Power_Band: return "Power Band";
		case Item::Power_Belt: return "Power Belt";
		case Item::Power_Bracer: return "Power Bracer";
		case Item::Power_Herb: return "Power Herb";
		case Item::Power_Lens: return "Power Lens";
		case Item::Power_Weight: return "Power Weight";
		case Item::Protector: return "Protector";
		case Item::Pure_Incense: return "Pure Incense";
		case Item::Quick_Ball: return "Quick Ball";
		case Item::Quick_Powder: return "Quick Powder";
		case Item::Rare_Bone: return "Rare Bone";
		case Item::Razor_Claw: return "Razor Claw";
		case Item::Razor_Fang: return "Razor Fang";
		case Item::Reaper_Cloth: return "Reaper Cloth";
		case Item::Rindo_Berry: return "Rindo Berry";
		case Item::Rock_Incense: return "Rock Incense";
		case Item::Rose_Incense: return "Rose Incense";
		case Item::Rowap_Berry: return "Rowap Berry";
		case Item::Shed_Shell: return "Shed Shell";
		case Item::Shiny_Stone: return "Shiny Stone";
		case Item::Shuca_Berry: return "Shuca Berry";
		case Item::Skull_Fossil: return "Skull Fossil";
		case Item::Sky_Plate: return "Sky Plate";
		case Item::Smooth_Rock: return "Smooth Rock";
		case Item::Snow_Mail: return "Snow Mail";
		case Item::Space_Mail: return "Space Mail";
		case Item::Splash_Plate: return "Splash Plate";
		case Item::Spooky_Plate: return "Spooky Plate";
		case Item::Stable_Mulch: return "Stable Mulch";
		case Item::Steel_Mail: return "Steel Mail";
		case Item::Sticky_Barb: return "Sticky Barb";
		case Item::Stone_Plate: return "Stone Plate";
		case Item::Tanga_Berry: return "Tanga Berry";
		case Item::TM51: return "TM51";
		case Item::TM52: return "TM52";
		case Item::TM53: return "TM53";
		case Item::TM54: return "TM54";
		case Item::TM55: return "TM55";
		case Item::TM56: return "TM56";
		case Item::TM57: return "TM57";
		case Item::TM58: return "TM58";
		case Item::TM59: return "TM59";
		case Item::TM60: return "TM60";
		case Item::TM61: return "TM61";
		case Item::TM62: return "TM62";
		case Item::TM63: return "TM63";
		case Item::TM64: return "TM64";
		case Item::TM65: return "TM65";
		case Item::TM66: return "TM66";
		case Item::TM67: return "TM67";
		case Item::TM68: return "TM68";
		case Item::TM69: return "TM69";
		case Item::TM70: return "TM70";
		case Item::TM71: return "TM71";
		case Item::TM72: return "TM72";
		case Item::TM73: return "TM73";
		case Item::TM74: return "TM74";
		case Item::TM75: return "TM75";
		case Item::TM76: return "TM76";
		case Item::TM77: return "TM77";
		case Item::TM78: return "TM78";
		case Item::TM79: return "TM79";
		case Item::TM80: return "TM80";
		case Item::TM81: return "TM81";
		case Item::TM82: return "TM82";
		case Item::TM83: return "TM83";
		case Item::TM84: return "TM84";
		case Item::TM85: return "TM85";
		case Item::TM86: return "TM86";
		case Item::TM87: return "TM87";
		case Item::TM88: return "TM88";
		case Item::TM89: return "TM89";
		case Item::TM90: return "TM90";
		case Item::TM91: return "TM91";
		case Item::TM92: return "TM92";
		case Item::Toxic_Orb: return "Toxic Orb";
		case Item::Toxic_Plate: return "Toxic Plate";
		case Item::Tunnel_Mail: return "Tunnel Mail";
		case Item::Wacan_Berry: return "Wacan Berry";
		case Item::Wave_Incense: return "Wave Incense";
		case Item::Wide_Lens: return "Wide Lens";
		case Item::Wise_Glasses: return "Wise Glasses";
		case Item::X_Sp_Def: return "X Sp. Def";
		case Item::Yache_Berry: return "Yache Berry";
		case Item::Zap_Plate: return "Zap Plate";
		case Item::Zoom_Lens: return "Zoom Lens";

		// Generation 5
		case Item::Absorb_Bulb: return "Absorb Bulb";
		case Item::Air_Balloon: return "Air Balloon";
		case Item::Balm_Mushroom: return "Balm Mushroom";
		case Item::Big_Nugget: return "Big Nugget";
		case Item::Binding_Band: return "Binding Band";
		case Item::BridgeMail_D: return "BridgeMail D";
		case Item::BridgeMail_M: return "BridgeMail M";
		case Item::BridgeMail_S: return "BridgeMail S";
		case Item::BridgeMail_T: return "BridgeMail T";
		case Item::BridgeMail_V: return "BridgeMail V";
		case Item::Bug_Gem: return "Bug Gem";
		case Item::Burn_Drive: return "Burn Drive";
		case Item::Casteliacone: return "Casteliacone";
		case Item::Cell_Battery: return "Cell Battery";
		case Item::Chill_Drive: return "Chill Drive";
		case Item::Clever_Wing: return "Clever Wing";
		case Item::Comet_Shard: return "Comet Shard";
		case Item::Cover_Fossil: return "Cover Fossil";
		case Item::Dark_Gem: return "Dark Gem";
		case Item::Douse_Drive: return "Douse Drive";
		case Item::Dragon_Gem: return "Dragon Gem";
		case Item::Dream_Ball: return "Dream Ball";
		case Item::Eject_Button: return "Eject Button";
		case Item::Electric_Gem: return "Electric Gem";
		case Item::Eviolite: return "Eviolite";
		case Item::Favored_Mail: return "Favored Mail";
		case Item::Fighting_Gem: return "Fighting Gem";
		case Item::Fire_Gem: return "Fire Gem";
		case Item::Float_Stone: return "Float Stone";
		case Item::Flying_Gem: return "Flying Gem";
		case Item::Genius_Wing: return "Genius Wing";
		case Item::Ghost_Gem: return "Ghost Gem";
		case Item::Grass_Gem: return "Grass Gem";
		case Item::Greet_Mail: return "Greet Mail";
		case Item::Ground_Gem: return "Ground Gem";
		case Item::Health_Wing: return "Health Wing";
		case Item::Ice_Gem: return "Ice Gem";
		case Item::Inquiry_Mail: return "Inquiry Mail";
		case Item::Like_Mail: return "Like Mail";
		case Item::Muscle_Wing: return "Muscle Wing";
		case Item::Normal_Gem: return "Normal Gem";
		case Item::Pass_Orb: return "Pass Orb";
		case Item::Pearl_String: return "Pearl String";
		case Item::Plume_Fossil: return "Plume Fossil";
		case Item::Poison_Gem: return "Poison Gem";
		case Item::Poke_Toy: return "Poke Toy";
		case Item::Pretty_Wing: return "Pretty Wing";
		case Item::Prism_Scale: return "Prism Scale";
		case Item::Psychic_Gem: return "Psychic Gem";
		case Item::Red_Card: return "Red Card";
		case Item::Relic_Band: return "Relic Band";
		case Item::Relic_Copper: return "Relic Copper";
		case Item::Relic_Crown: return "Relic Crown";
		case Item::Relic_Gold: return "Relic Gold";
		case Item::Relic_Silver: return "Relic Silver";
		case Item::Relic_Statue: return "Relic Statue";
		case Item::Relic_Vase: return "Relic Vase";
		case Item::Reply_Mail: return "Reply Mail";
		case Item::Resist_Wing: return "Resist Wing";
		case Item::Ring_Target: return "Ring Target";
		case Item::Rock_Gem: return "Rock Gem";
		case Item::Rocky_Helmet: return "Rocky Helmet";
		case Item::RSVP_Mail: return "RSVP Mail";
		case Item::Shock_Drive: return "Shock Drive";
		case Item::Steel_Gem: return "Steel Gem";
		case Item::Sweet_Heart: return "Sweet Heart";
		case Item::Swift_Wing: return "Swift Wing";
		case Item::Thanks_Mail: return "Thanks Mail";
		case Item::Water_Gem: return "Water Gem";

		// Generation 6
		case Item::Ability_Capsule: return "Ability Capsule";
		case Item::Abomasite: return "Abomasite";
		case Item::Absolite: return "Absolite";
		case Item::Aerodactylite: return "Aerodactylite";
		case Item::Aggronite: return "Aggronite";
		case Item::Alakazite: return "Alakazite";
		case Item::Altarianite: return "Altarianite";
		case Item::Amaze_Mulch: return "Amaze Mulch";
		case Item::Ampharosite: return "Ampharosite";
		case Item::Assault_Vest: return "Assault Vest";
		case Item::Audinite: return "Audinite";
		case Item::Banettite: return "Banettite";
		case Item::Beedrillite: return "Beedrillite";
		case Item::Blastoisinite: return "Blastoisinite";
		case Item::Blazikenite: return "Blazikenite";
		case Item::Boost_Mulch: return "Boost Mulch";
		case Item::Cameruptite: return "Cameruptite";
		case Item::Charizardite_X: return "Charizardite X";
		case Item::Charizardite_Y: return "Charizardite Y";
		case Item::Diancite: return "Diancite";
		case Item::Discount_Coupon: return "Discount Coupon";
		case Item::Fairy_Gem: return "Fairy Gem";
		case Item::Galladite: return "Galladite";
		case Item::Garchompite: return "Garchompite";
		case Item::Gardevoirite: return "Gardevoirite";
		case Item::Gengarite: return "Gengarite";
		case Item::Glalitite: return "Glalitite";
		case Item::Gyaradosite: return "Gyaradosite";
		case Item::Heracronite: return "Heracronite";
		case Item::Houndoominite: return "Houndoominite";
		case Item::Jaw_Fossil: return "Jaw Fossil";
		case Item::Kangaskhanite: return "Kangaskhanite";
		case Item::Kee_Berry: return "Kee Berry";
		case Item::Latiasite: return "Latiasite";
		case Item::Latiosite: return "Latiosite";
		case Item::Lopunnite: return "Lopunnite";
		case Item::Lucarionite: return "Lucarionite";
		case Item::Luminous_Moss: return "Luminous Moss";
		case Item::Lumiose_Galette: return "Lumiose Galette";
		case Item::Manectite: return "Manectite";
		case Item::Maranga_Berry: return "Maranga Berry";
		case Item::Mawilite: return "Mawilite";
		case Item::Medichamite: return "Medichamite";
		case Item::Metagrossite: return "Metagrossite";
		case Item::Mewtwonite_X: return "Mewtwonite X";
		case Item::Mewtwonite_Y: return "Mewtwonite Y";
		case Item::Pidgeotite: return "Pidgeotite";
		case Item::Pinsirite: return "Pinsirite";
		case Item::Pixie_Plate: return "Pixie Plate";
		case Item::Rich_Mulch: return "Rich Mulch";
		case Item::Roseli_Berry: return "Roseli Berry";
		case Item::Sablenite: return "Sablenite";
		case Item::Sachet: return "Sachet";
		case Item::Safety_Goggles: return "Safety Goggles";
		case Item::Sail_Fossil: return "Sail Fossil";
		case Item::Salamencite: return "Salamencite";
		case Item::Sceptilite: return "Sceptilite";
		case Item::Scizorite: return "Scizorite";
		case Item::Shalour_Sable: return "Shalour Sable";
		case Item::Sharpedonite: return "Sharpedonite";
		case Item::Slowbronite: return "Slowbronite";
		case Item::Snowball: return "Snowball";
		case Item::Steelixite: return "Steelixite";
		case Item::Strange_Souvenir: return "Strange Souvenir";
		case Item::Surprise_Mulch: return "Surprise Mulch";
		case Item::Swampertite: return "Swampertite";
		case Item::Tyranitarite: return "Tyranitarite";
		case Item::Venusaurite: return "Venusaurite";
		case Item::Weakness_Policy: return "Weakness Policy";
		case Item::Whipped_Dream: return "Whipped Dream";
		case Item::X_Defense: return "X Defense";
		case Item::X_Sp_Atk: return "X Sp. Atk";

		// Generation 7
		case Item::Adrenaline_Orb: return "Adrenaline Orb";
		case Item::Aloraichium_Z: return "Aloraichium Z";
		case Item::Beast_Ball: return "Beast Ball";
		case Item::Big_Malasada: return "Big Malasada";
		case Item::Bottle_Cap: return "Bottle Cap";
		case Item::Bug_Memory: return "Bug Memory";
		case Item::Buginium_Z: return "Buginium Z";
		case Item::Dark_Memory: return "Dark Memory";
		case Item::Darkinium_Z: return "Darkinium Z";
		case Item::Decidium_Z: return "Decidium Z";
		case Item::Dragon_Memory: return "Dragon Memory";
		case Item::Dragonium_Z: return "Dragonium Z";
		case Item::Eevium_Z: return "Eevium Z";
		case Item::Electric_Memory: return "Electric Memory";
		case Item::Electric_Seed: return "Electric Seed";
		case Item::Electrium_Z: return "Electrium Z";
		case Item::Fairium_Z: return "Fairium Z";
		case Item::Fairy_Memory: return "Fairy Memory";
		case Item::Festival_Ticket: return "Festival Ticket";
		case Item::Fighting_Memory: return "Fighting Memory";
		case Item::Fightinium_Z: return "Fightinium Z";
		case Item::Fire_Memory: return "Fire Memory";
		case Item::Firium_Z: return "Firium Z";
		case Item::Flying_Memory: return "Flying Memory";
		case Item::Flyinium_Z: return "Flyinium Z";
		case Item::Ghost_Memory: return "Ghost Memory";
		case Item::Ghostium_Z: return "Ghostium Z";
		case Item::Gold_Bottle_Cap: return "Gold Bottle Cap";
		case Item::Grass_Memory: return "Grass Memory";
		case Item::Grassium_Z: return "Grassium Z";
		case Item::Grassy_Seed: return "Grassy Seed";
		case Item::Ground_Memory: return "Ground Memory";
		case Item::Groundium_Z: return "Groundium Z";
		case Item::Ice_Memory: return "Ice Memory";
		case Item::Ice_Stone: return "Ice Stone";
		case Item::Icium_Z: return "Icium Z";
		case Item::Incinium_Z: return "Incinium Z";
		case Item::Kommonium_Z: return "Kommonium Z";
		case Item::Lunalium_Z: return "Lunalium Z";
		case Item::Lycanium_Z: return "Lycanium Z";
		case Item::Marshadium_Z: return "Marshadium Z";
		case Item::Mewnium_Z: return "Mewnium Z";
		case Item::Mimikium_Z: return "Mimikium Z";
		case Item::Misty_Seed: return "Misty Seed";
		case Item::Normalium_Z: return "Normalium Z";
		case Item::Pewter_Crunchies: return "Pewter Crunchies";
		case Item::Pikanium_Z: return "Pikanium Z";
		case Item::Pikashunium_Z: return "Pikashunium Z";
		case Item::Pink_Nectar: return "Pink Nectar";
		case Item::Poison_Memory: return "Poison Memory";
		case Item::Poisonium_Z: return "Poisonium Z";
		case Item::Primarium_Z: return "Primarium Z";
		case Item::Protective_Pads: return "Protective Pads";
		case Item::Psychic_Memory: return "Psychic Memory";
		case Item::Psychic_Seed: return "Psychic Seed";
		case Item::Psychium_Z: return "Psychium Z";
		case Item::Purple_Nectar: return "Purple Nectar";
		case Item::Rock_Memory: return "Rock Memory";
		case Item::Rockium_Z: return "Rockium Z";
		case Item::Snorlium_Z: return "Snorlium Z";
		case Item::Solganium_Z: return "Solganium Z";
		case Item::Steel_Memory: return "Steel Memory";
		case Item::Steelium_Z: return "Steelium Z";
		case Item::Tapunium_Z: return "Tapunium Z";
		case Item::Terrain_Extender: return "Terrain Extender";
		case Item::Ultranecrozium_Z: return "Ultranecrozium Z";
		case Item::Water_Memory: return "Water Memory";
		case Item::Waterium_Z: return "Waterium Z";
		case Item::Yellow_Nectar: return "Yellow Nectar";

		// Generation 8
		case Item::Adamant_Mint: return "Adamant Mint";
		case Item::Bachs_Food_Tin: return "Bach's Food Tin";
		case Item::Berry_Sweet: return "Berry Sweet";
		case Item::Blunder_Policy: return "Blunder Policy";
		case Item::Bobs_Food_Tin: return "Bob's Food Tin";
		case Item::Boiled_Egg: return "Boiled Egg";
		case Item::Bold_Mint: return "Bold Mint";
		case Item::Brave_Mint: return "Brave Mint";
		case Item::Bread: return "Bread";
		case Item::Brittle_Bones: return "Brittle Bones";
		case Item::Calm_Mint: return "Calm Mint";
		case Item::Careful_Mint: return "Careful Mint";
		case Item::Chipped_Pot: return "Chipped Pot";
		case Item::Clover_Sweet: return "Clover Sweet";
		case Item::Coconut_Milk: return "Coconut Milk";
		case Item::Cracked_Pot: return "Cracked Pot";
		case Item::Eject_Pack: return "Eject Pack";
		case Item::Fancy_Apple: return "Fancy Apple";
		case Item::Flower_Sweet: return "Flower Sweet";
		case Item::Fresh_Cream: return "Fresh Cream";
		case Item::Fossilized_Bird: return "Fossilized Bird";
		case Item::Fossilized_Dino: return "Fossilized Dino";
		case Item::Fossilized_Drake: return "Fossilized Drake";
		case Item::Fossilized_Fish: return "Fossilized Fish";
		case Item::Fried_Food: return "Fried Food";
		case Item::Fruit_Bunch: return "Fruit Bunch";
		case Item::Gentle_Mint: return "Gentle Mint";
		case Item::Gigantamix: return "Gigantamix";
		case Item::Hasty_Mint: return "Hasty Mint";
		case Item::Heavy_Duty_Boots: return "Heavy-Duty Boots";
		case Item::Impish_Mint: return "Impish Mint";
		case Item::Instant_Noodles: return "Instant Noodles";
		case Item::Jolly_Mint: return "Jolly Mint";
		case Item::Large_Leek: return "Large Leek";
		case Item::Lax_Mint: return "Lax Mint";
		case Item::Leek: return "Leek";
		case Item::Lonely_Mint: return "Lonely Mint";
		case Item::Love_Sweet: return "Love Sweet";
		case Item::Mild_Mint: return "Mild Mint";
		case Item::Mixed_Mushrooms: return "Mixed Mushrooms";
		case Item::Modest_Mint: return "Modest Mint";
		case Item::Moomoo_Cheese: return "Moomoo Cheese";
		case Item::Naive_Mint: return "Naive Mint";
		case Item::Naughty_Mint: return "Naughty Mint";
		case Item::Pack_of_Potatoes: return "Pack of Potatoes";
		case Item::Packaged_Curry: return "Packaged Curry";
		case Item::Pasta: return "Pasta";
		case Item::Precooked_Burger: return "Precooked Burger";
		case Item::Pungent_Root: return "Pungent Root";
		case Item::Quiet_Mint: return "Quiet Mint";
		case Item::Rash_Mint: return "Rash Mint";
		case Item::Relaxed_Mint: return "Relaxed Mint";
		case Item::Ribbon_Sweet: return "Ribbon Sweet";
		case Item::Room_Service: return "Room Service";
		case Item::Rusted_Shield: return "Rusted Shield";
		case Item::Rusted_Sword: return "Rusted Sword";
		case Item::Salad_Mix: return "Salad Mix";
		case Item::Sassy_Mint: return "Sassy Mint";
		case Item::Sausages: return "Sausages";
		case Item::Serious_Mint: return "Serious Mint";
		case Item::Smoke_Poke_Tail: return "Smoke-Poke Tail";
		case Item::Spice_Mix: return "Spice Mix";
		case Item::Star_Sweet: return "Star Sweet";
		case Item::Strawberry_Sweet: return "Strawberry Sweet";
		case Item::Sweet_Apple: return "Sweet Apple";
		case Item::Tart_Apple: return "Tart Apple";
		case Item::Throat_Spray: return "Throat Spray";
		case Item::Timid_Mint: return "Timid Mint";
		case Item::Tin_of_Beans: return "Tin of Beans";
		case Item::TR00: return "TR00";
		case Item::TR01: return "TR01";
		case Item::TR02: return "TR02";
		case Item::TR03: return "TR03";
		case Item::TR04: return "TR04";
		case Item::TR05: return "TR05";
		case Item::TR06: return "TR06";
		case Item::TR07: return "TR07";
		case Item::TR08: return "TR08";
		case Item::TR09: return "TR09";
		case Item::TR10: return "TR10";
		case Item::TR11: return "TR11";
		case Item::TR12: return "TR12";
		case Item::TR13: return "TR13";
		case Item::TR14: return "TR14";
		case Item::TR15: return "TR15";
		case Item::TR16: return "TR16";
		case Item::TR17: return "TR17";
		case Item::TR18: return "TR18";
		case Item::TR19: return "TR19";
		case Item::TR20: return "TR20";
		case Item::TR21: return "TR21";
		case Item::TR22: return "TR22";
		case Item::TR23: return "TR23";
		case Item::TR24: return "TR24";
		case Item::TR25: return "TR25";
		case Item::TR26: return "TR26";
		case Item::TR27: return "TR27";
		case Item::TR28: return "TR28";
		case Item::TR29: return "TR29";
		case Item::TR30: return "TR30";
		case Item::TR31: return "TR31";
		case Item::TR32: return "TR32";
		case Item::TR33: return "TR33";
		case Item::TR34: return "TR34";
		case Item::TR35: return "TR35";
		case Item::TR36: return "TR36";
		case Item::TR37: return "TR37";
		case Item::TR38: return "TR38";
		case Item::TR39: return "TR39";
		case Item::TR40: return "TR40";
		case Item::TR41: return "TR41";
		case Item::TR42: return "TR42";
		case Item::TR43: return "TR43";
		case Item::TR44: return "TR44";
		case Item::TR45: return "TR45";
		case Item::TR46: return "TR46";
		case Item::TR47: return "TR47";
		case Item::TR48: return "TR48";
		case Item::TR49: return "TR49";
		case Item::TR50: return "TR50";
		case Item::TR51: return "TR51";
		case Item::TR52: return "TR52";
		case Item::TR53: return "TR53";
		case Item::TR54: return "TR54";
		case Item::TR55: return "TR55";
		case Item::TR56: return "TR56";
		case Item::TR57: return "TR57";
		case Item::TR58: return "TR58";
		case Item::TR59: return "TR59";
		case Item::TR60: return "TR60";
		case Item::TR61: return "TR61";
		case Item::TR62: return "TR62";
		case Item::TR63: return "TR63";
		case Item::TR64: return "TR64";
		case Item::TR65: return "TR65";
		case Item::TR66: return "TR66";
		case Item::TR67: return "TR67";
		case Item::TR68: return "TR68";
		case Item::TR69: return "TR69";
		case Item::TR70: return "TR70";
		case Item::TR71: return "TR71";
		case Item::TR72: return "TR72";
		case Item::TR73: return "TR73";
		case Item::TR74: return "TR74";
		case Item::TR75: return "TR75";
		case Item::TR76: return "TR76";
		case Item::TR77: return "TR77";
		case Item::TR78: return "TR78";
		case Item::TR79: return "TR79";
		case Item::TR80: return "TR80";
		case Item::TR81: return "TR81";
		case Item::TR82: return "TR82";
		case Item::TR83: return "TR83";
		case Item::TR84: return "TR84";
		case Item::TR85: return "TR85";
		case Item::TR86: return "TR86";
		case Item::TR87: return "TR87";
		case Item::TR88: return "TR88";
		case Item::TR89: return "TR89";
		case Item::TR90: return "TR90";
		case Item::TR91: return "TR91";
		case Item::TR92: return "TR92";
		case Item::TR93: return "TR93";
		case Item::TR94: return "TR94";
		case Item::TR95: return "TR95";
		case Item::TR96: return "TR96";
		case Item::TR97: return "TR97";
		case Item::TR98: return "TR98";
		case Item::TR99: return "TR99";
		case Item::Utility_Umbrella: return "Utility Umbrella";
	}
}

template<>
auto from_string(std::string_view const str) -> Item {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Item>>({
			{"", Item::None},
			{"abilitycapsule", Item::Ability_Capsule},
			{"abomasite", Item::Abomasite},
			{"absolite", Item::Absolite},
			{"absorbbulb", Item::Absorb_Bulb},
			{"adamantmint", Item::Adamant_Mint},
			{"adamantorb", Item::Adamant_Orb},
			{"adrenalineorb", Item::Adrenaline_Orb},
			{"aerodactylite", Item::Aerodactylite},
			{"aggronite", Item::Aggronite},
			{"aguavberry", Item::Aguav_Berry},
			{"airballoon", Item::Air_Balloon},
			{"airmail", Item::Air_Mail},
			{"alakazite", Item::Alakazite},
			{"aloraichiumz", Item::Aloraichium_Z},
			{"altarianite", Item::Altarianite},
			{"amazemulch", Item::Amaze_Mulch},
			{"ampharosite", Item::Ampharosite},
			{"amuletcoin", Item::Amulet_Coin},
			{"antidote", Item::Antidote},
			{"apicotberry", Item::Apicot_Berry},
			{"armorfossil", Item::Armor_Fossil},
			{"aspearberry", Item::Aspear_Berry},
			{"assaultvest", Item::Assault_Vest},
			{"audinite", Item::Audinite},
			{"awakening", Item::Awakening},
			{"babiriberry", Item::Babiri_Berry},
			{"bachsfoodtin", Item::Bachs_Food_Tin},
			{"balmmushroom", Item::Balm_Mushroom},
			{"banettite", Item::Banettite},
			{"beadmail", Item::Bead_Mail},
			{"beastball", Item::Beast_Ball},
			{"beedrillite", Item::Beedrillite},
			{"belueberry", Item::Belue_Berry},
			{"berry", Item::Berry},
			{"berryjuice", Item::Berry_Juice},
			{"berrysweet", Item::Berry_Sweet},
			{"berserkgene", Item::Berserk_Gene},
			{"bigmalasada", Item::Big_Malasada},
			{"bigmushroom", Item::Big_Mushroom},
			{"bignugget", Item::Big_Nugget},
			{"bigpearl", Item::Big_Pearl},
			{"bigroot", Item::Big_Root},
			{"bindingband", Item::Binding_Band},
			{"bitterberry", Item::Bitter_Berry},
			{"blackapricorn", Item::Black_Apricorn},
			{"blackbelt", Item::Black_Belt},
			{"blackflute", Item::Black_Flute},
			{"blackglasses", Item::Black_Glasses},
			{"blacksludge", Item::Black_Sludge},
			{"blastoisinite", Item::Blastoisinite},
			{"blazikenite", Item::Blazikenite},
			{"bloommail", Item::Bloom_Mail},
			{"blueapricorn", Item::Blue_Apricorn},
			{"blueflute", Item::Blue_Flute},
			{"blueorb", Item::Blue_Orb},
			{"bluescarf", Item::Blue_Scarf},
			{"blueshard", Item::Blue_Shard},
			{"blueskymail", Item::Bluesky_Mail},
			{"blukberry", Item::Bluk_Berry},
			{"blunderpolicy", Item::Blunder_Policy},
			{"bobsfoodtin", Item::Bobs_Food_Tin},
			{"boiledegg", Item::Boiled_Egg},
			{"boldmint", Item::Bold_Mint},
			{"boostmulch", Item::Boost_Mulch},
			{"bottlecap", Item::Bottle_Cap},
			{"bravemint", Item::Brave_Mint},
			{"bread", Item::Bread},
			{"brickmail", Item::Brick_Mail},
			{"brickpiece", Item::Brick_Piece},
			{"bridgemaild", Item::BridgeMail_D},
			{"bridgemailm", Item::BridgeMail_M},
			{"bridgemails", Item::BridgeMail_S},
			{"bridgemailt", Item::BridgeMail_T},
			{"bridgemailv", Item::BridgeMail_V},
			{"brightpowder", Item::Bright_Powder},
			{"brittlebones", Item::Brittle_Bones},
			{"bubblemail", Item::Bubble_Mail},
			{"buggem", Item::Bug_Gem},
			{"buginiumz", Item::Buginium_Z},
			{"bugmemory", Item::Bug_Memory},
			{"burndrive", Item::Burn_Drive},
			{"burnheal", Item::Burn_Heal},
			{"burntberry", Item::Burnt_Berry},
			{"calcium", Item::Calcium},
			{"calmmint", Item::Calm_Mint},
			{"cameruptite", Item::Cameruptite},
			{"carbos", Item::Carbos},
			{"carefulmint", Item::Careful_Mint},
			{"casteliacone", Item::Casteliacone},
			{"cellbattery", Item::Cell_Battery},
			{"charcoal", Item::Charcoal},
			{"charizarditex", Item::Charizardite_X},
			{"charizarditey", Item::Charizardite_Y},
			{"chartiberry", Item::Charti_Berry},
			{"cheriberry", Item::Cheri_Berry},
			{"cherishball", Item::Cherish_Ball},
			{"chestoberry", Item::Chesto_Berry},
			{"chilanberry", Item::Chilan_Berry},
			{"chilldrive", Item::Chill_Drive},
			{"chippedpot", Item::Chipped_Pot},
			{"choiceband", Item::Choice_Band},
			{"choicescarf", Item::Choice_Scarf},
			{"choicespecs", Item::Choice_Specs},
			{"chopleberry", Item::Chople_Berry},
			{"clawfossil", Item::Claw_Fossil},
			{"cleansetag", Item::Cleanse_Tag},
			{"cleverwing", Item::Clever_Wing},
			{"cloversweet", Item::Clover_Sweet},
			{"cobaberry", Item::Coba_Berry},
			{"coconutmilk", Item::Coconut_Milk},
			{"colburberry", Item::Colbur_Berry},
			{"cometshard", Item::Comet_Shard},
			{"cornnberry", Item::Cornn_Berry},
			{"coverfossil", Item::Cover_Fossil},
			{"crackedpot", Item::Cracked_Pot},
			{"custapberry", Item::Custap_Berry},
			{"dampmulch", Item::Damp_Mulch},
			{"damprock", Item::Damp_Rock},
			{"darkgem", Item::Dark_Gem},
			{"darkiniumz", Item::Darkinium_Z},
			{"darkmemory", Item::Dark_Memory},
			{"dawnstone", Item::Dawn_Stone},
			{"decidiumz", Item::Decidium_Z},
			{"deepseascale", Item::Deep_Sea_Scale},
			{"deepseatooth", Item::Deep_Sea_Tooth},
			{"destinyknot", Item::Destiny_Knot},
			{"diancite", Item::Diancite},
			{"direhit", Item::Dire_Hit},
			{"discountcoupon", Item::Discount_Coupon},
			{"diveball", Item::Dive_Ball},
			{"domefossil", Item::Dome_Fossil},
			{"dousedrive", Item::Douse_Drive},
			{"dracoplate", Item::Draco_Plate},
			{"dragonfang", Item::Dragon_Fang},
			{"dragongem", Item::Dragon_Gem},
			{"dragoniumz", Item::Dragonium_Z},
			{"dragonmemory", Item::Dragon_Memory},
			{"dragonscale", Item::Dragon_Scale},
			{"drashberry", Item::Drash_Berry},
			{"dreadplate", Item::Dread_Plate},
			{"dreamball", Item::Dream_Ball},
			{"dreammail", Item::Dream_Mail},
			{"dubiousdisc", Item::Dubious_Disc},
			{"durinberry", Item::Durin_Berry},
			{"duskball", Item::Dusk_Ball},
			{"duskstone", Item::Dusk_Stone},
			{"earthplate", Item::Earth_Plate},
			{"eeviumz", Item::Eevium_Z},
			{"eggantberry", Item::Eggant_Berry},
			{"ejectbutton", Item::Eject_Button},
			{"ejectpack", Item::Eject_Pack},
			{"electirizer", Item::Electirizer},
			{"electricgem", Item::Electric_Gem},
			{"electricmemory", Item::Electric_Memory},
			{"electricseed", Item::Electric_Seed},
			{"electriumz", Item::Electrium_Z},
			{"elixir", Item::Elixir},
			{"energypowder", Item::Energy_Powder},
			{"energyroot", Item::Energy_Root},
			{"enigmaberry", Item::Enigma_Berry},
			{"eonmail", Item::Eon_Mail},
			{"escaperope", Item::Escape_Rope},
			{"ether", Item::Ether},
			{"everstone", Item::Everstone},
			{"eviolite", Item::Eviolite},
			{"expertbelt", Item::Expert_Belt},
			{"expshare", Item::Exp_Share},
			{"fabmail", Item::Fab_Mail},
			{"fairiumz", Item::Fairium_Z},
			{"fairygem", Item::Fairy_Gem},
			{"fairymemory", Item::Fairy_Memory},
			{"fancyapple", Item::Fancy_Apple},
			{"fastball", Item::Fast_Ball},
			{"favoredmail", Item::Favored_Mail},
			{"festivalticket", Item::Festival_Ticket},
			{"fightinggem", Item::Fighting_Gem},
			{"fightingmemory", Item::Fighting_Memory},
			{"fightiniumz", Item::Fightinium_Z},
			{"figyberry", Item::Figy_Berry},
			{"firegem", Item::Fire_Gem},
			{"firememory", Item::Fire_Memory},
			{"firestone", Item::Fire_Stone},
			{"firiumz", Item::Firium_Z},
			{"fistplate", Item::Fist_Plate},
			{"flamemail", Item::Flame_Mail},
			{"flameorb", Item::Flame_Orb},
			{"flameplate", Item::Flame_Plate},
			{"floatstone", Item::Float_Stone},
			{"flowermail", Item::Flower_Mail},
			{"flowersweet", Item::Flower_Sweet},
			{"fluffytail", Item::Fluffy_Tail},
			{"flyinggem", Item::Flying_Gem},
			{"flyingmemory", Item::Flying_Memory},
			{"flyiniumz", Item::Flyinium_Z},
			{"focusband", Item::Focus_Band},
			{"focussash", Item::Focus_Sash},
			{"fossilizedbird", Item::Fossilized_Bird},
			{"fossilizeddino", Item::Fossilized_Dino},
			{"fossilizeddrake", Item::Fossilized_Drake},
			{"fossilizedfish", Item::Fossilized_Fish},
			{"freshcream", Item::Fresh_Cream},
			{"freshwater", Item::Fresh_Water},
			{"friedfood", Item::Fried_Food},
			{"friendball", Item::Friend_Ball},
			{"fruitbunch", Item::Fruit_Bunch},
			{"fullheal", Item::Full_Heal},
			{"fullincense", Item::Full_Incense},
			{"fullrestore", Item::Full_Restore},
			{"galladite", Item::Galladite},
			{"ganlonberry", Item::Ganlon_Berry},
			{"garchompite", Item::Garchompite},
			{"gardevoirite", Item::Gardevoirite},
			{"gengarite", Item::Gengarite},
			{"geniuswing", Item::Genius_Wing},
			{"gentlemint", Item::Gentle_Mint},
			{"ghostgem", Item::Ghost_Gem},
			{"ghostiumz", Item::Ghostium_Z},
			{"ghostmemory", Item::Ghost_Memory},
			{"gigantamix", Item::Gigantamix},
			{"ginemaberry", Item::Ginema_Berry},
			{"glalitite", Item::Glalitite},
			{"glittermail", Item::Glitter_Mail},
			{"goldberry", Item::Gold_Berry},
			{"goldbottlecap", Item::Gold_Bottle_Cap},
			{"goldleaf", Item::Gold_Leaf},
			{"gooeymulch", Item::Gooey_Mulch},
			{"gorgeousbox", Item::Gorgeous_Box},
			{"grassgem", Item::Grass_Gem},
			{"grassiumz", Item::Grassium_Z},
			{"grassmail", Item::Grass_Mail},
			{"grassmemory", Item::Grass_Memory},
			{"grassyseed", Item::Grassy_Seed},
			{"greatball", Item::Great_Ball},
			{"greenapricorn", Item::Green_Apricorn},
			{"greenscarf", Item::Green_Scarf},
			{"greenshard", Item::Green_Shard},
			{"greetmail", Item::Greet_Mail},
			{"grepaberry", Item::Grepa_Berry},
			{"gripclaw", Item::Grip_Claw},
			{"griseousorb", Item::Griseous_Orb},
			{"groundgem", Item::Ground_Gem},
			{"groundiumz", Item::Groundium_Z},
			{"groundmemory", Item::Ground_Memory},
			{"growthmulch", Item::Growth_Mulch},
			{"guardspec", Item::Guard_Spec},
			{"gyaradosite", Item::Gyaradosite},
			{"habanberry", Item::Haban_Berry},
			{"harbormail", Item::Harbor_Mail},
			{"hardstone", Item::Hard_Stone},
			{"hastymint", Item::Hasty_Mint},
			{"healball", Item::Heal_Ball},
			{"healpowder", Item::Heal_Powder},
			{"healthwing", Item::Health_Wing},
			{"heartmail", Item::Heart_Mail},
			{"heartscale", Item::Heart_Scale},
			{"heatrock", Item::Heat_Rock},
			{"heavyball", Item::Heavy_Ball},
			{"heavydutyboots", Item::Heavy_Duty_Boots},
			{"helixfossil", Item::Helix_Fossil},
			{"heracronite", Item::Heracronite},
			{"hondewberry", Item::Hondew_Berry},
			{"honey", Item::Honey},
			{"houndoominite", Item::Houndoominite},
			{"hpup", Item::HP_Up},
			{"hyperpotion", Item::Hyper_Potion},
			{"iapapaberry", Item::Iapapa_Berry},
			{"iceberry", Item::Ice_Berry},
			{"icegem", Item::Ice_Gem},
			{"iceheal", Item::Ice_Heal},
			{"icememory", Item::Ice_Memory},
			{"icestone", Item::Ice_Stone},
			{"icicleplate", Item::Icicle_Plate},
			{"iciumz", Item::Icium_Z},
			{"icyrock", Item::Icy_Rock},
			{"impishmint", Item::Impish_Mint},
			{"inciniumz", Item::Incinium_Z},
			{"inquirymail", Item::Inquiry_Mail},
			{"insectplate", Item::Insect_Plate},
			{"instantnoodles", Item::Instant_Noodles},
			{"iron", Item::Iron},
			{"ironball", Item::Iron_Ball},
			{"ironplate", Item::Iron_Plate},
			{"jabocaberry", Item::Jaboca_Berry},
			{"jawfossil", Item::Jaw_Fossil},
			{"jollymint", Item::Jolly_Mint},
			{"kangaskhanite", Item::Kangaskhanite},
			{"kasibberry", Item::Kasib_Berry},
			{"kebiaberry", Item::Kebia_Berry},
			{"keeberry", Item::Kee_Berry},
			{"kelpsyberry", Item::Kelpsy_Berry},
			{"kingsrock", Item::Kings_Rock},
			{"kommoniumz", Item::Kommonium_Z},
			{"kuoberry", Item::Kuo_Berry},
			{"laggingtail", Item::Lagging_Tail},
			{"lansatberry", Item::Lansat_Berry},
			{"largeleek", Item::Large_Leek},
			{"latiasite", Item::Latiasite},
			{"latiosite", Item::Latiosite},
			{"lavacookie", Item::Lava_Cookie},
			{"laxincense", Item::Lax_Incense},
			{"laxmint", Item::Lax_Mint},
			{"leafstone", Item::Leaf_Stone},
			{"leek", Item::Leek},
			{"leftovers", Item::Leftovers},
			{"lemonade", Item::Lemonade},
			{"leppaberry", Item::Leppa_Berry},
			{"levelball", Item::Level_Ball},
			{"liechiberry", Item::Liechi_Berry},
			{"lifeorb", Item::Life_Orb},
			{"lightball", Item::Light_Ball},
			{"lightclay", Item::Light_Clay},
			{"likemail", Item::Like_Mail},
			{"litebluemail", Item::Litebluemail},
			{"lonelymint", Item::Lonely_Mint},
			{"lopunnite", Item::Lopunnite},
			{"loveball", Item::Love_Ball},
			{"lovelymail", Item::Lovely_Mail},
			{"lovesweet", Item::Love_Sweet},
			{"lucarionite", Item::Lucarionite},
			{"luckincense", Item::Luck_Incense},
			{"luckyegg", Item::Lucky_Egg},
			{"luckypunch", Item::Lucky_Punch},
			{"lumberry", Item::Lum_Berry},
			{"luminousmoss", Item::Luminous_Moss},
			{"lumiosegalette", Item::Lumiose_Galette},
			{"lunaliumz", Item::Lunalium_Z},
			{"lureball", Item::Lure_Ball},
			{"lustrousorb", Item::Lustrous_Orb},
			{"luxuryball", Item::Luxury_Ball},
			{"lycaniumz", Item::Lycanium_Z},
			{"machobrace", Item::Macho_Brace},
			{"magmarizer", Item::Magmarizer},
			{"magnet", Item::Magnet},
			{"magoberry", Item::Mago_Berry},
			{"magostberry", Item::Magost_Berry},
			{"manectite", Item::Manectite},
			{"marangaberry", Item::Maranga_Berry},
			{"marshadiumz", Item::Marshadium_Z},
			{"masterball", Item::Master_Ball},
			{"mawilite", Item::Mawilite},
			{"maxelixir", Item::Max_Elixir},
			{"maxether", Item::Max_Ether},
			{"maxpotion", Item::Max_Potion},
			{"maxrepel", Item::Max_Repel},
			{"maxrevive", Item::Max_Revive},
			{"meadowplate", Item::Meadow_Plate},
			{"mechmail", Item::Mech_Mail},
			{"medichamite", Item::Medichamite},
			{"mentalherb", Item::Mental_Herb},
			{"metagrossite", Item::Metagrossite},
			{"metalcoat", Item::Metal_Coat},
			{"metalpowder", Item::Metal_Powder},
			{"metronome", Item::Metronome},
			{"mewniumz", Item::Mewnium_Z},
			{"mewtwonitex", Item::Mewtwonite_X},
			{"mewtwonitey", Item::Mewtwonite_Y},
			{"micleberry", Item::Micle_Berry},
			{"mildmint", Item::Mild_Mint},
			{"mimikiumz", Item::Mimikium_Z},
			{"mindplate", Item::Mind_Plate},
			{"mintberry", Item::Mint_Berry},
			{"miracleberry", Item::MiracleBerry},
			{"miracleseed", Item::Miracle_Seed},
			{"miragemail", Item::Mirage_Mail},
			{"mistyseed", Item::Misty_Seed},
			{"mixedmushrooms", Item::Mixed_Mushrooms},
			{"modestmint", Item::Modest_Mint},
			{"moomoocheese", Item::Moomoo_Cheese},
			{"moomoomilk", Item::Moomoo_Milk},
			{"moonball", Item::Moon_Ball},
			{"moonstone", Item::Moon_Stone},
			{"morphmail", Item::Morph_Mail},
			{"mosaicmail", Item::Mosaic_Mail},
			{"muscleband", Item::Muscle_Band},
			{"musclewing", Item::Muscle_Wing},
			{"musicmail", Item::Music_Mail},
			{"mysteryberry", Item::MysteryBerry},
			{"mysticwater", Item::Mystic_Water},
			{"naivemint", Item::Naive_Mint},
			{"nanabberry", Item::Nanab_Berry},
			{"naughtymint", Item::Naughty_Mint},
			{"nestball", Item::Nest_Ball},
			{"netball", Item::Net_Ball},
			{"nevermeltice", Item::Never_Melt_Ice},
			{"ninikuberry", Item::Niniku_Berry},
			{"nomelberry", Item::Nomel_Berry},
			{"none", Item::None},
			{"normalbox", Item::Normal_Box},
			{"normalgem", Item::Normal_Gem},
			{"normaliumz", Item::Normalium_Z},
			{"nugget", Item::Nugget},
			{"nutpeaberry", Item::Nutpea_Berry},
			{"occaberry", Item::Occa_Berry},
			{"oddincense", Item::Odd_Incense},
			{"oddkeystone", Item::Odd_Keystone},
			{"oldamber", Item::Old_Amber},
			{"oldgateau", Item::Old_Gateau},
			{"oranberry", Item::Oran_Berry},
			{"orangemail", Item::Orange_Mail},
			{"ovalstone", Item::Oval_Stone},
			{"packagedcurry", Item::Packaged_Curry},
			{"packofpotatoes", Item::Pack_of_Potatoes},
			{"pamtreberry", Item::Pamtre_Berry},
			{"paralyzeheal", Item::Paralyze_Heal},
			{"parkball", Item::Park_Ball},
			{"parlyzheal", Item::Paralyze_Heal},
			{"passhoberry", Item::Passho_Berry},
			{"passorb", Item::Pass_Orb},
			{"pasta", Item::Pasta},
			{"payapaberry", Item::Payapa_Berry},
			{"pearl", Item::Pearl},
			{"pearlstring", Item::Pearl_String},
			{"pechaberry", Item::Pecha_Berry},
			{"persimberry", Item::Persim_Berry},
			{"petayaberry", Item::Petaya_Berry},
			{"pewtercrunchies", Item::Pewter_Crunchies},
			{"pidgeotite", Item::Pidgeotite},
			{"pikaniumz", Item::Pikanium_Z},
			{"pikashuniumz", Item::Pikashunium_Z},
			{"pinapberry", Item::Pinap_Berry},
			{"pinkapricorn", Item::Pink_Apricorn},
			{"pinkbow", Item::Pink_Bow},
			{"pinknectar", Item::Pink_Nectar},
			{"pinkscarf", Item::Pink_Scarf},
			{"pinsirite", Item::Pinsirite},
			{"pixieplate", Item::Pixie_Plate},
			{"plumefossil", Item::Plume_Fossil},
			{"poisonbarb", Item::Poison_Barb},
			{"poisongem", Item::Poison_Gem},
			{"poisoniumz", Item::Poisonium_Z},
			{"poisonmemory", Item::Poison_Memory},
			{"pokeball", Item::Poke_Ball},
			{"pokedoll", Item::Poke_Doll},
			{"poketoy", Item::Poke_Toy},
			{"polkadotbow", Item::Polkadot_Bow},
			{"pomegberry", Item::Pomeg_Berry},
			{"portraitmail", Item::Portraitmail},
			{"potion", Item::Potion},
			{"poweranklet", Item::Power_Anklet},
			{"powerband", Item::Power_Band},
			{"powerbelt", Item::Power_Belt},
			{"powerbracer", Item::Power_Bracer},
			{"powerherb", Item::Power_Herb},
			{"powerlens", Item::Power_Lens},
			{"powerweight", Item::Power_Weight},
			{"ppmax", Item::PP_Max},
			{"ppup", Item::PP_Up},
			{"precookedburger", Item::Precooked_Burger},
			{"premierball", Item::Premier_Ball},
			{"prettywing", Item::Pretty_Wing},
			{"primariumz", Item::Primarium_Z},
			{"prismscale", Item::Prism_Scale},
			{"protectivepads", Item::Protective_Pads},
			{"protector", Item::Protector},
			{"protein", Item::Protein},
			{"przcureberry", Item::PRZCureBerry},
			{"psncureberry", Item::PSNCureBerry},
			{"psychicgem", Item::Psychic_Gem},
			{"psychicmemory", Item::Psychic_Memory},
			{"psychicseed", Item::Psychic_Seed},
			{"psychiumz", Item::Psychium_Z},
			{"pumkinberry", Item::Pumkin_Berry},
			{"pungentroot", Item::Pungent_Root},
			{"pureincense", Item::Pure_Incense},
			{"purplenectar", Item::Purple_Nectar},
			{"qualotberry", Item::Qualot_Berry},
			{"quickball", Item::Quick_Ball},
			{"quickclaw", Item::Quick_Claw},
			{"quickpowder", Item::Quick_Powder},
			{"quietmint", Item::Quiet_Mint},
			{"rabutaberry", Item::Rabuta_Berry},
			{"ragecandybar", Item::Rage_Candy_Bar},
			{"rarebone", Item::Rare_Bone},
			{"rarecandy", Item::Rare_Candy},
			{"rashmint", Item::Rash_Mint},
			{"rawstberry", Item::Rawst_Berry},
			{"razorclaw", Item::Razor_Claw},
			{"razorfang", Item::Razor_Fang},
			{"razzberry", Item::Razz_Berry},
			{"reapercloth", Item::Reaper_Cloth},
			{"redapricorn", Item::Red_Apricorn},
			{"redcard", Item::Red_Card},
			{"redflute", Item::Red_Flute},
			{"redorb", Item::Red_Orb},
			{"redscarf", Item::Red_Scarf},
			{"redshard", Item::Red_Shard},
			{"relaxedmint", Item::Relaxed_Mint},
			{"relicband", Item::Relic_Band},
			{"reliccopper", Item::Relic_Copper},
			{"reliccrown", Item::Relic_Crown},
			{"relicgold", Item::Relic_Gold},
			{"relicsilver", Item::Relic_Silver},
			{"relicstatue", Item::Relic_Statue},
			{"relicvase", Item::Relic_Vase},
			{"repeatball", Item::Repeat_Ball},
			{"repel", Item::Repel},
			{"replymail", Item::Reply_Mail},
			{"resistwing", Item::Resist_Wing},
			{"retromail", Item::Retro_Mail},
			{"revivalherb", Item::Revival_Herb},
			{"revive", Item::Revive},
			{"ribbonsweet", Item::Ribbon_Sweet},
			{"richmulch", Item::Rich_Mulch},
			{"rindoberry", Item::Rindo_Berry},
			{"ringtarget", Item::Ring_Target},
			{"rockgem", Item::Rock_Gem},
			{"rockincense", Item::Rock_Incense},
			{"rockiumz", Item::Rockium_Z},
			{"rockmemory", Item::Rock_Memory},
			{"rockyhelmet", Item::Rocky_Helmet},
			{"roomservice", Item::Room_Service},
			{"rootfossil", Item::Root_Fossil},
			{"roseincense", Item::Rose_Incense},
			{"roseliberry", Item::Roseli_Berry},
			{"rowapberry", Item::Rowap_Berry},
			{"rsvpmail", Item::RSVP_Mail},
			{"rustedshield", Item::Rusted_Shield},
			{"rustedsword", Item::Rusted_Sword},
			{"sablenite", Item::Sablenite},
			{"sachet", Item::Sachet},
			{"sacredash", Item::Sacred_Ash},
			{"safariball", Item::Safari_Ball},
			{"safetygoggles", Item::Safety_Goggles},
			{"sailfossil", Item::Sail_Fossil},
			{"salacberry", Item::Salac_Berry},
			{"saladmix", Item::Salad_Mix},
			{"salamencite", Item::Salamencite},
			{"sassymint", Item::Sassy_Mint},
			{"sausages", Item::Sausages},
			{"sceptilite", Item::Sceptilite},
			{"scizorite", Item::Scizorite},
			{"scopelens", Item::Scope_Lens},
			{"seaincense", Item::Sea_Incense},
			{"seriousmint", Item::Serious_Mint},
			{"shadowmail", Item::Shadow_Mail},
			{"shaloursable", Item::Shalour_Sable},
			{"sharpbeak", Item::Sharp_Beak},
			{"sharpedonite", Item::Sharpedonite},
			{"shedshell", Item::Shed_Shell},
			{"shellbell", Item::Shell_Bell},
			{"shinystone", Item::Shiny_Stone},
			{"shoalsalt", Item::Shoal_Salt},
			{"shoalshell", Item::Shoal_Shell},
			{"shockdrive", Item::Shock_Drive},
			{"shucaberry", Item::Shuca_Berry},
			{"silkscarf", Item::Silk_Scarf},
			{"silverleaf", Item::Silver_Leaf},
			{"silverpowder", Item::Silver_Powder},
			{"sitrusberry", Item::Sitrus_Berry},
			{"skullfossil", Item::Skull_Fossil},
			{"skyplate", Item::Sky_Plate},
			{"slowbronite", Item::Slowbronite},
			{"slowpoketail", Item::Slowpoke_Tail},
			{"smokeball", Item::Smoke_Ball},
			{"smokepoketail", Item::Smoke_Poke_Tail},
			{"smoothrock", Item::Smooth_Rock},
			{"snorliumz", Item::Snorlium_Z},
			{"snowball", Item::Snowball},
			{"snowmail", Item::Snow_Mail},
			{"sodapop", Item::Soda_Pop},
			{"softsand", Item::Soft_Sand},
			{"solganiumz", Item::Solganium_Z},
			{"soothebell", Item::Soothe_Bell},
			{"souldew", Item::Soul_Dew},
			{"spacemail", Item::Space_Mail},
			{"spelltag", Item::Spell_Tag},
			{"spelonberry", Item::Spelon_Berry},
			{"spicemix", Item::Spice_Mix},
			{"splashplate", Item::Splash_Plate},
			{"spookyplate", Item::Spooky_Plate},
			{"sportball", Item::Sport_Ball},
			{"stablemulch", Item::Stable_Mulch},
			{"stardust", Item::Stardust},
			{"starfberry", Item::Starf_Berry},
			{"starpiece", Item::Star_Piece},
			{"starsweet", Item::Star_Sweet},
			{"steelgem", Item::Steel_Gem},
			{"steeliumz", Item::Steelium_Z},
			{"steelixite", Item::Steelixite},
			{"steelmail", Item::Steel_Mail},
			{"steelmemory", Item::Steel_Memory},
			{"stick", Item::Stick},
			{"stickybarb", Item::Sticky_Barb},
			{"stoneplate", Item::Stone_Plate},
			{"strangesouvenir", Item::Strange_Souvenir},
			{"strawberrysweet", Item::Strawberry_Sweet},
			{"stribberry", Item::Strib_Berry},
			{"sunstone", Item::Sun_Stone},
			{"superpotion", Item::Super_Potion},
			{"superrepel", Item::Super_Repel},
			{"surfmail", Item::Surf_Mail},
			{"surprisemulch", Item::Surprise_Mulch},
			{"swampertite", Item::Swampertite},
			{"sweetapple", Item::Sweet_Apple},
			{"sweetheart", Item::Sweet_Heart},
			{"swiftwing", Item::Swift_Wing},
			{"tamatoberry", Item::Tamato_Berry},
			{"tangaberry", Item::Tanga_Berry},
			{"tapuniumz", Item::Tapunium_Z},
			{"tartapple", Item::Tart_Apple},
			{"terrainextender", Item::Terrain_Extender},
			{"thanksmail", Item::Thanks_Mail},
			{"thickclub", Item::Thick_Club},
			{"throatspray", Item::Throat_Spray},
			{"thunderstone", Item::Thunder_Stone},
			{"timerball", Item::Timer_Ball},
			{"timidmint", Item::Timid_Mint},
			{"tinofbeans", Item::Tin_of_Beans},
			{"tinymushroom", Item::Tiny_Mushroom},
			{"tm01", Item::TM01},
			{"tm02", Item::TM02},
			{"tm03", Item::TM03},
			{"tm04", Item::TM04},
			{"tm05", Item::TM05},
			{"tm06", Item::TM06},
			{"tm07", Item::TM07},
			{"tm08", Item::TM08},
			{"tm09", Item::TM09},
			{"tm10", Item::TM10},
			{"tm11", Item::TM11},
			{"tm12", Item::TM12},
			{"tm13", Item::TM13},
			{"tm14", Item::TM14},
			{"tm15", Item::TM15},
			{"tm16", Item::TM16},
			{"tm17", Item::TM17},
			{"tm18", Item::TM18},
			{"tm19", Item::TM19},
			{"tm20", Item::TM20},
			{"tm21", Item::TM21},
			{"tm22", Item::TM22},
			{"tm23", Item::TM23},
			{"tm24", Item::TM24},
			{"tm25", Item::TM25},
			{"tm26", Item::TM26},
			{"tm27", Item::TM27},
			{"tm28", Item::TM28},
			{"tm29", Item::TM29},
			{"tm30", Item::TM30},
			{"tm31", Item::TM31},
			{"tm32", Item::TM32},
			{"tm33", Item::TM33},
			{"tm34", Item::TM34},
			{"tm35", Item::TM35},
			{"tm36", Item::TM36},
			{"tm37", Item::TM37},
			{"tm38", Item::TM38},
			{"tm39", Item::TM39},
			{"tm40", Item::TM40},
			{"tm41", Item::TM41},
			{"tm42", Item::TM42},
			{"tm43", Item::TM43},
			{"tm44", Item::TM44},
			{"tm45", Item::TM45},
			{"tm46", Item::TM46},
			{"tm47", Item::TM47},
			{"tm48", Item::TM48},
			{"tm49", Item::TM49},
			{"tm50", Item::TM50},
			{"tm51", Item::TM51},
			{"tm52", Item::TM52},
			{"tm53", Item::TM53},
			{"tm54", Item::TM54},
			{"tm55", Item::TM55},
			{"tm56", Item::TM56},
			{"tm57", Item::TM57},
			{"tm58", Item::TM58},
			{"tm59", Item::TM59},
			{"tm60", Item::TM60},
			{"tm61", Item::TM61},
			{"tm62", Item::TM62},
			{"tm63", Item::TM63},
			{"tm64", Item::TM64},
			{"tm65", Item::TM65},
			{"tm66", Item::TM66},
			{"tm67", Item::TM67},
			{"tm68", Item::TM68},
			{"tm69", Item::TM69},
			{"tm70", Item::TM70},
			{"tm71", Item::TM71},
			{"tm72", Item::TM72},
			{"tm73", Item::TM73},
			{"tm74", Item::TM74},
			{"tm75", Item::TM75},
			{"tm76", Item::TM76},
			{"tm77", Item::TM77},
			{"tm78", Item::TM78},
			{"tm79", Item::TM79},
			{"tm80", Item::TM80},
			{"tm81", Item::TM81},
			{"tm82", Item::TM82},
			{"tm83", Item::TM83},
			{"tm84", Item::TM84},
			{"tm85", Item::TM85},
			{"tm86", Item::TM86},
			{"tm87", Item::TM87},
			{"tm88", Item::TM88},
			{"tm89", Item::TM89},
			{"tm90", Item::TM90},
			{"tm91", Item::TM91},
			{"tm92", Item::TM92},
			{"topoberry", Item::Topo_Berry},
			{"tougaberry", Item::Touga_Berry},
			{"toxicorb", Item::Toxic_Orb},
			{"toxicplate", Item::Toxic_Plate},
			{"tr00", Item::TR00},
			{"tr01", Item::TR01},
			{"tr02", Item::TR02},
			{"tr03", Item::TR03},
			{"tr04", Item::TR04},
			{"tr05", Item::TR05},
			{"tr06", Item::TR06},
			{"tr07", Item::TR07},
			{"tr08", Item::TR08},
			{"tr09", Item::TR09},
			{"tr10", Item::TR10},
			{"tr11", Item::TR11},
			{"tr12", Item::TR12},
			{"tr13", Item::TR13},
			{"tr14", Item::TR14},
			{"tr15", Item::TR15},
			{"tr16", Item::TR16},
			{"tr17", Item::TR17},
			{"tr18", Item::TR18},
			{"tr19", Item::TR19},
			{"tr20", Item::TR20},
			{"tr21", Item::TR21},
			{"tr22", Item::TR22},
			{"tr23", Item::TR23},
			{"tr24", Item::TR24},
			{"tr25", Item::TR25},
			{"tr26", Item::TR26},
			{"tr27", Item::TR27},
			{"tr28", Item::TR28},
			{"tr29", Item::TR29},
			{"tr30", Item::TR30},
			{"tr31", Item::TR31},
			{"tr32", Item::TR32},
			{"tr33", Item::TR33},
			{"tr34", Item::TR34},
			{"tr35", Item::TR35},
			{"tr36", Item::TR36},
			{"tr37", Item::TR37},
			{"tr38", Item::TR38},
			{"tr39", Item::TR39},
			{"tr40", Item::TR40},
			{"tr41", Item::TR41},
			{"tr42", Item::TR42},
			{"tr43", Item::TR43},
			{"tr44", Item::TR44},
			{"tr45", Item::TR45},
			{"tr46", Item::TR46},
			{"tr47", Item::TR47},
			{"tr48", Item::TR48},
			{"tr49", Item::TR49},
			{"tr50", Item::TR50},
			{"tr51", Item::TR51},
			{"tr52", Item::TR52},
			{"tr53", Item::TR53},
			{"tr54", Item::TR54},
			{"tr55", Item::TR55},
			{"tr56", Item::TR56},
			{"tr57", Item::TR57},
			{"tr58", Item::TR58},
			{"tr59", Item::TR59},
			{"tr60", Item::TR60},
			{"tr61", Item::TR61},
			{"tr62", Item::TR62},
			{"tr63", Item::TR63},
			{"tr64", Item::TR64},
			{"tr65", Item::TR65},
			{"tr66", Item::TR66},
			{"tr67", Item::TR67},
			{"tr68", Item::TR68},
			{"tr69", Item::TR69},
			{"tr70", Item::TR70},
			{"tr71", Item::TR71},
			{"tr72", Item::TR72},
			{"tr73", Item::TR73},
			{"tr74", Item::TR74},
			{"tr75", Item::TR75},
			{"tr76", Item::TR76},
			{"tr77", Item::TR77},
			{"tr78", Item::TR78},
			{"tr79", Item::TR79},
			{"tr80", Item::TR80},
			{"tr81", Item::TR81},
			{"tr82", Item::TR82},
			{"tr83", Item::TR83},
			{"tr84", Item::TR84},
			{"tr85", Item::TR85},
			{"tr86", Item::TR86},
			{"tr87", Item::TR87},
			{"tr88", Item::TR88},
			{"tr89", Item::TR89},
			{"tr90", Item::TR90},
			{"tr91", Item::TR91},
			{"tr92", Item::TR92},
			{"tr93", Item::TR93},
			{"tr94", Item::TR94},
			{"tr95", Item::TR95},
			{"tr96", Item::TR96},
			{"tr97", Item::TR97},
			{"tr98", Item::TR98},
			{"tr99", Item::TR99},
			{"tropicmail", Item::Tropic_Mail},
			{"tunnelmail", Item::Tunnel_Mail},
			{"twistedspoon", Item::Twisted_Spoon},
			{"tyranitarite", Item::Tyranitarite},
			{"ultraball", Item::Ultra_Ball},
			{"ultranecroziumz", Item::Ultranecrozium_Z},
			{"upgrade", Item::Up_Grade},
			{"utilityumbrella", Item::Utility_Umbrella},
			{"venusaurite", Item::Venusaurite},
			{"wacanberry", Item::Wacan_Berry},
			{"watergem", Item::Water_Gem},
			{"wateriumz", Item::Waterium_Z},
			{"watermemory", Item::Water_Memory},
			{"waterstone", Item::Water_Stone},
			{"watmelberry", Item::Watmel_Berry},
			{"waveincense", Item::Wave_Incense},
			{"wavemail", Item::Wave_Mail},
			{"weaknesspolicy", Item::Weakness_Policy},
			{"wepearberry", Item::Wepear_Berry},
			{"whippeddream", Item::Whipped_Dream},
			{"whiteapricorn", Item::White_Apricorn},
			{"whiteflute", Item::White_Flute},
			{"whiteherb", Item::White_Herb},
			{"widelens", Item::Wide_Lens},
			{"wikiberry", Item::Wiki_Berry},
			{"wiseglasses", Item::Wise_Glasses},
			{"woodmail", Item::Wood_Mail},
			{"xaccuracy", Item::X_Accuracy},
			{"xattack", Item::X_Attack},
			{"xdefend", Item::X_Defend},
			{"xdefense", Item::X_Defense},
			{"xspatk", Item::X_Sp_Atk},
			{"xspdef", Item::X_Sp_Def},
			{"xspecial", Item::X_Special},
			{"xspeed", Item::X_Speed},
			{"yacheberry", Item::Yache_Berry},
			{"yagoberry", Item::Yago_Berry},
			{"yellowapricorn", Item::Yellow_Apricorn},
			{"yellowflute", Item::Yellow_Flute},
			{"yellownectar", Item::Yellow_Nectar},
			{"yellowscarf", Item::Yellow_Scarf},
			{"yellowshard", Item::Yellow_Shard},
			{"zapplate", Item::Zap_Plate},
			{"zinc", Item::Zinc},
			{"zoomlens", Item::Zoom_Lens},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<15_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Item", str);
	}
	return *result;
}

}	// namespace technicalmachine
