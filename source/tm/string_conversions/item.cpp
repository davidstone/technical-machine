// Item string conversions
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

#include <tm/string_conversions/item.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>

#include <tm/item.hpp>

#include <map>

namespace technicalmachine {

std::string_view to_string(Item const item) {
	switch (item) {
		case Item::No_Item: return "No Item";
		case Item::Adamant_Orb: return "Adamant Orb";
		case Item::Aguav_Berry: return "Aguav Berry";
		case Item::Air_Mail: return "Air Mail";
		case Item::Amulet_Coin: return "Amulet Coin";
		case Item::Antidote: return "Antidote";
		case Item::Apicot_Berry: return "Apicot Berry";
		case Item::Armor_Fossil: return "Armor Fossil";
		case Item::Aspear_Berry: return "Aspear Berry";
		case Item::Awakening: return "Awakening";
		case Item::Babiri_Berry: return "Babiri Berry";
		case Item::Belue_Berry: return "Belue Berry";
		case Item::Berry_Juice: return "Berry Juice";
		case Item::Bicycle: return "Bicycle";
		case Item::Big_Mushroom: return "Big Mushroom";
		case Item::Big_Pearl: return "Big Pearl";
		case Item::Big_Root: return "Big Root";
		case Item::Black_Belt: return "Black Belt";
		case Item::Black_Flute: return "Black Flute";
		case Item::Black_Sludge: return "Black Sludge";
		case Item::BlackGlasses: return "BlackGlasses";
		case Item::Bloom_Mail: return "Bloom Mail";
		case Item::Blue_Flute: return "Blue Flute";
		case Item::Blue_Scarf: return "Blue Scarf";
		case Item::Blue_Shard: return "Blue Shard";
		case Item::Bluk_Berry: return "Bluk Berry";
		case Item::Brick_Mail: return "Brick Mail";
		case Item::BrightPowder: return "BrightPowder";
		case Item::Bubble_Mail: return "Bubble Mail";
		case Item::Burn_Heal: return "Burn Heal";
		case Item::Calcium: return "Calcium";
		case Item::Carbos: return "Carbos";
		case Item::Charcoal: return "Charcoal";
		case Item::Charti_Berry: return "Charti Berry";
		case Item::Cheri_Berry: return "Cheri Berry";
		case Item::Cherish_Ball: return "Cherish Ball";
		case Item::Chesto_Berry: return "Chesto Berry";
		case Item::Chilan_Berry: return "Chilan Berry";
		case Item::Choice_Band: return "Choice Band";
		case Item::Choice_Scarf: return "Choice Scarf";
		case Item::Choice_Specs: return "Choice Specs";
		case Item::Chople_Berry: return "Chople Berry";
		case Item::Claw_Fossil: return "Claw Fossil";
		case Item::Cleanse_Tag: return "Cleanse Tag";
		case Item::Coba_Berry: return "Coba Berry";
		case Item::Coin_Case: return "Coin Case";
		case Item::Colbur_Berry: return "Colbur Berry";
		case Item::Cornn_Berry: return "Cornn Berry";
		case Item::Coupon_1: return "Coupon 1";
		case Item::Coupon_2: return "Coupon 2";
		case Item::Coupon_3: return "Coupon 3";
		case Item::Custap_Berry: return "Custap Berry";
		case Item::Damp_Mulch: return "Damp Mulch";
		case Item::Damp_Rock: return "Damp Rock";
		case Item::Dawn_Stone: return "Dawn Stone";
		case Item::DeepSeaScale: return "DeepSeaScale";
		case Item::DeepSeaTooth: return "DeepSeaTooth";
		case Item::Destiny_Knot: return "Destiny Knot";
		case Item::Dire_Hit: return "Dire Hit";
		case Item::Dive_Ball: return "Dive Ball";
		case Item::Dome_Fossil: return "Dome Fossil";
		case Item::Draco_Plate: return "Draco Plate";
		case Item::Dragon_Fang: return "Dragon Fang";
		case Item::Dragon_Scale: return "Dragon Scale";
		case Item::Dread_Plate: return "Dread Plate";
		case Item::Dubious_Disc: return "Dubious Disc";
		case Item::Durin_Berry: return "Durin Berry";
		case Item::Dusk_Ball: return "Dusk Ball";
		case Item::Dusk_Stone: return "Dusk Stone";
		case Item::Earth_Plate: return "Earth Plate";
		case Item::Electirizer: return "Electirizer";
		case Item::Elixir: return "Elixir";
		case Item::Energy_Root: return "Energy Root";
		case Item::EnergyPowder: return "EnergyPowder";
		case Item::Enigma_Berry: return "Enigma Berry";
		case Item::Escape_Rope: return "Escape Rope";
		case Item::Ether: return "Ether";
		case Item::Everstone: return "Everstone";
		case Item::Exp_Share: return "Exp. Share";
		case Item::Expert_Belt: return "Expert Belt";
		case Item::Explorer_Kit: return "Explorer Kit";
		case Item::Fashion_Case: return "Fashion Case";
		case Item::Figy_Berry: return "Figy Berry";
		case Item::Fire_Stone: return "Fire Stone";
		case Item::Fist_Plate: return "Fist Plate";
		case Item::Flame_Mail: return "Flame Mail";
		case Item::Flame_Orb: return "Flame Orb";
		case Item::Flame_Plate: return "Flame Plate";
		case Item::Fluffy_Tail: return "Fluffy Tail";
		case Item::Focus_Band: return "Focus Band";
		case Item::Focus_Sash: return "Focus Sash";
		case Item::Fresh_Water: return "Fresh Water";
		case Item::Full_Heal: return "Full Heal";
		case Item::Full_Incense: return "Full Incense";
		case Item::Full_Restore: return "Full Restore";
		case Item::Galactic_Key: return "Galactic Key";
		case Item::Ganlon_Berry: return "Ganlon Berry";
		case Item::Good_Rod: return "Good Rod";
		case Item::Gooey_Mulch: return "Gooey Mulch";
		case Item::Grass_Mail: return "Grass Mail";
		case Item::Great_Ball: return "Great Ball";
		case Item::Green_Scarf: return "Green Scarf";
		case Item::Green_Shard: return "Green Shard";
		case Item::Grepa_Berry: return "Grepa Berry";
		case Item::Grip_Claw: return "Grip Claw";
		case Item::Griseous_Orb: return "Griseous Orb";
		case Item::Growth_Mulch: return "Growth Mulch";
		case Item::Guard_Spec: return "Guard Spec.";
		case Item::Haban_Berry: return "Haban Berry";
		case Item::Hard_Stone: return "Hard Stone";
		case Item::Heal_Ball: return "Heal Ball";
		case Item::Heal_Powder: return "Heal Powder";
		case Item::Heart_Mail: return "Heart Mail";
		case Item::Heart_Scale: return "Heart Scale";
		case Item::Heat_Rock: return "Heat Rock";
		case Item::Helix_Fossil: return "Helix Fossil";
		case Item::HM01: return "HM01";
		case Item::HM02: return "HM02";
		case Item::HM03: return "HM03";
		case Item::HM04: return "HM04";
		case Item::HM05: return "HM05";
		case Item::HM06: return "HM06";
		case Item::HM07: return "HM07";
		case Item::HM08: return "HM08";
		case Item::Hondew_Berry: return "Hondew Berry";
		case Item::Honey: return "Honey";
		case Item::HP_Up: return "HP Up";
		case Item::Hyper_Potion: return "Hyper Potion";
		case Item::Iapapa_Berry: return "Iapapa Berry";
		case Item::Ice_Heal: return "Ice Heal";
		case Item::Icicle_Plate: return "Icicle Plate";
		case Item::Icy_Rock: return "Icy Rock";
		case Item::Insect_Plate: return "Insect Plate";
		case Item::Iron: return "Iron";
		case Item::Iron_Ball: return "Iron Ball";
		case Item::Iron_Plate: return "Iron Plate";
		case Item::Jaboca_Berry: return "Jaboca Berry";
		case Item::Journal: return "Journal";
		case Item::Kasib_Berry: return "Kasib Berry";
		case Item::Kebia_Berry: return "Kebia Berry";
		case Item::Kelpsy_Berry: return "Kelpsy Berry";
		case Item::Kings_Rock: return "King's Rock";
		case Item::Lagging_Tail: return "Lagging Tail";
		case Item::Lansat_Berry: return "Lansat Berry";
		case Item::Lava_Cookie: return "Lava Cookie";
		case Item::Lax_Incense: return "Lax Incense";
		case Item::Leaf_Stone: return "Leaf Stone";
		case Item::Leftovers: return "Leftovers";
		case Item::Lemonade: return "Lemonade";
		case Item::Leppa_Berry: return "Leppa Berry";
		case Item::Liechi_Berry: return "Liechi Berry";
		case Item::Life_Orb: return "Life Orb";
		case Item::Light_Ball: return "Light Ball";
		case Item::Light_Clay: return "Light Clay";
		case Item::Loot_Sack: return "Loot Sack";
		case Item::Luck_Incense: return "Luck Incense";
		case Item::Lucky_Egg: return "Lucky Egg";
		case Item::Lucky_Punch: return "Lucky Punch";
		case Item::Lum_Berry: return "Lum Berry";
		case Item::Lunar_Wing: return "Lunar Wing";
		case Item::Lustrous_Orb: return "Lustrous Orb";
		case Item::Luxury_Ball: return "Luxury Ball";
		case Item::Macho_Brace: return "Macho Brace";
		case Item::Magmarizer: return "Magmarizer";
		case Item::Magnet: return "Magnet";
		case Item::Mago_Berry: return "Mago Berry";
		case Item::Magost_Berry: return "Magost Berry";
		case Item::Master_Ball: return "Master Ball";
		case Item::Max_Elixir: return "Max Elixir";
		case Item::Max_Ether: return "Max Ether";
		case Item::Max_Potion: return "Max Potion";
		case Item::Max_Repel: return "Max Repel";
		case Item::Max_Revive: return "Max Revive";
		case Item::Meadow_Plate: return "Meadow Plate";
		case Item::Member_Card: return "Member Card";
		case Item::Mental_Herb: return "Mental Herb";
		case Item::Metal_Coat: return "Metal Coat";
		case Item::Metal_Powder: return "Metal Powder";
		case Item::Metronome: return "Metronome";
		case Item::Micle_Berry: return "Micle Berry";
		case Item::Mind_Plate: return "Mind Plate";
		case Item::Miracle_Seed: return "Miracle Seed";
		case Item::MooMoo_Milk: return "MooMoo Milk";
		case Item::Moon_Stone: return "Moon Stone";
		case Item::Mosaic_Mail: return "Mosaic Mail";
		case Item::Muscle_Band: return "Muscle Band";
		case Item::Mystic_Water: return "Mystic Water";
		case Item::Nanab_Berry: return "Nanab Berry";
		case Item::Nest_Ball: return "Nest Ball";
		case Item::Net_Ball: return "Net Ball";
		case Item::NeverMeltIce: return "NeverMeltIce";
		case Item::Nomel_Berry: return "Nomel Berry";
		case Item::Nugget: return "Nugget";
		case Item::Oaks_Letter: return "Oak's Letter";
		case Item::Occa_Berry: return "Occa Berry";
		case Item::Odd_Incense: return "Odd Incense";
		case Item::Odd_Keystone: return "Odd Keystone";
		case Item::Old_Amber: return "Old Amber";
		case Item::Old_Charm: return "Old Charm";
		case Item::Old_Gateau: return "Old Gateau";
		case Item::Old_Rod: return "Old Rod";
		case Item::Oran_Berry: return "Oran Berry";
		case Item::Oval_Stone: return "Oval Stone";
		case Item::Pal_Pad: return "Pal Pad";
		case Item::Pamtre_Berry: return "Pamtre Berry";
		case Item::Parcel: return "Parcel";
		case Item::Parlyz_Heal: return "Parlyz Heal";
		case Item::Passho_Berry: return "Passho Berry";
		case Item::Payapa_Berry: return "Payapa Berry";
		case Item::Pearl: return "Pearl";
		case Item::Pecha_Berry: return "Pecha Berry";
		case Item::Persim_Berry: return "Persim Berry";
		case Item::Petaya_Berry: return "Petaya Berry";
		case Item::Pinap_Berry: return "Pinap Berry";
		case Item::Pink_Scarf: return "Pink Scarf";
		case Item::Poffin_Case: return "Poffin Case";
		case Item::Point_Card: return "Point Card";
		case Item::Poison_Barb: return "Poison Barb";
		case Item::Poke_Ball: return "Poke Ball";
		case Item::Poke_Doll: return "Poke Doll";
		case Item::Poke_Radar: return "Poke Radar";
		case Item::Pomeg_Berry: return "Pomeg Berry";
		case Item::Potion: return "Potion";
		case Item::Power_Anklet: return "Power Anklet";
		case Item::Power_Band: return "Power Band";
		case Item::Power_Belt: return "Power Belt";
		case Item::Power_Bracer: return "Power Bracer";
		case Item::Power_Herb: return "Power Herb";
		case Item::Power_Lens: return "Power Lens";
		case Item::Power_Weight: return "Power Weight";
		case Item::PP_Max: return "PP Max";
		case Item::PP_Up: return "PP Up";
		case Item::Premier_Ball: return "Premier Ball";
		case Item::Protector: return "Protector";
		case Item::Protein: return "Protein";
		case Item::Pure_Incense: return "Pure Incense";
		case Item::Qualot_Berry: return "Qualot Berry";
		case Item::Quick_Ball: return "Quick Ball";
		case Item::Quick_Claw: return "Quick Claw";
		case Item::Quick_Powder: return "Quick Powder";
		case Item::Rabuta_Berry: return "Rabuta Berry";
		case Item::Rare_Bone: return "Rare Bone";
		case Item::Rare_Candy: return "Rare Candy";
		case Item::Rawst_Berry: return "Rawst Berry";
		case Item::Razor_Claw: return "Razor Claw";
		case Item::Razor_Fang: return "Razor Fang";
		case Item::Razz_Berry: return "Razz Berry";
		case Item::Reaper_Cloth: return "Reaper Cloth";
		case Item::Red_Flute: return "Red Flute";
		case Item::Red_Scarf: return "Red Scarf";
		case Item::Red_Shard: return "Red Shard";
		case Item::Repeat_Ball: return "Repeat Ball";
		case Item::Repel: return "Repel";
		case Item::Revival_Herb: return "Revival Herb";
		case Item::Revive: return "Revive";
		case Item::Rindo_Berry: return "Rindo Berry";
		case Item::Rock_Incense: return "Rock Incense";
		case Item::Root_Fossil: return "Root Fossil";
		case Item::Rose_Incense: return "Rose Incense";
		case Item::Rowap_Berry: return "Rowap Berry";
		case Item::Sacred_Ash: return "Sacred Ash";
		case Item::Safari_Ball: return "Safari Ball";
		case Item::Salac_Berry: return "Salac Berry";
		case Item::Scope_Lens: return "Scope Lens";
		case Item::Sea_Incense: return "Sea Incense";
		case Item::Seal_Bag: return "Seal Bag";
		case Item::Seal_Case: return "Seal Case";
		case Item::SecretPotion: return "SecretPotion";
		case Item::Sharp_Beak: return "Sharp Beak";
		case Item::Shed_Shell: return "Shed Shell";
		case Item::Shell_Bell: return "Shell Bell";
		case Item::Shiny_Stone: return "Shiny Stone";
		case Item::Shoal_Salt: return "Shoal Salt";
		case Item::Shoal_Shell: return "Shoal Shell";
		case Item::Shuca_Berry: return "Shuca Berry";
		case Item::Silk_Scarf: return "Silk Scarf";
		case Item::SilverPowder: return "SilverPowder";
		case Item::Sitrus_Berry: return "Sitrus Berry";
		case Item::Skull_Fossil: return "Skull Fossil";
		case Item::Sky_Plate: return "Sky Plate";
		case Item::Smoke_Ball: return "Smoke Ball";
		case Item::Smooth_Rock: return "Smooth Rock";
		case Item::Snow_Mail: return "Snow Mail";
		case Item::Soda_Pop: return "Soda Pop";
		case Item::Soft_Sand: return "Soft Sand";
		case Item::Soothe_Bell: return "Soothe Bell";
		case Item::Soul_Dew: return "Soul Dew";
		case Item::Space_Mail: return "Space Mail";
		case Item::Spell_Tag: return "Spell Tag";
		case Item::Spelon_Berry: return "Spelon Berry";
		case Item::Splash_Plate: return "Splash Plate";
		case Item::Spooky_Plate: return "Spooky Plate";
		case Item::Sprayduck: return "Sprayduck";
		case Item::Stable_Mulch: return "Stable Mulch";
		case Item::Star_Piece: return "Star Piece";
		case Item::Stardust: return "Stardust";
		case Item::Starf_Berry: return "Starf Berry";
		case Item::Steel_Mail: return "Steel Mail";
		case Item::Stick: return "Stick";
		case Item::Sticky_Barb: return "Sticky Barb";
		case Item::Stone_Plate: return "Stone Plate";
		case Item::Storage_Key: return "Storage Key";
		case Item::Suite_Key: return "Suite Key";
		case Item::Sun_Stone: return "Sun Stone";
		case Item::Super_Potion: return "Super Potion";
		case Item::Super_Repel: return "Super Repel";
		case Item::Super_Rod: return "Super Rod";
		case Item::Tamato_Berry: return "Tamato Berry";
		case Item::Tanga_Berry: return "Tanga Berry";
		case Item::Thick_Club: return "Thick Club";
		case Item::Thunderstone: return "Thunderstone";
		case Item::Timer_Ball: return "Timer Ball";
		case Item::TinyMushroom: return "TinyMushroom";
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
		case Item::Town_Map: return "Town Map";
		case Item::Toxic_Orb: return "Toxic Orb";
		case Item::Toxic_Plate: return "Toxic Plate";
		case Item::Tunnel_Mail: return "Tunnel Mail";
		case Item::TwistedSpoon: return "TwistedSpoon";
		case Item::Ultra_Ball: return "Ultra Ball";
		case Item::Up_Grade: return "Up-Grade";
		case Item::VS_Seeker: return "VS Seeker";
		case Item::Wacan_Berry: return "Wacan Berry";
		case Item::Water_Stone: return "Water Stone";
		case Item::Watmel_Berry: return "Watmel Berry";
		case Item::Wave_Incense: return "Wave Incense";
		case Item::Wepear_Berry: return "Wepear Berry";
		case Item::White_Flute: return "White Flute";
		case Item::White_Herb: return "White Herb";
		case Item::Wide_Lens: return "Wide Lens";
		case Item::Wiki_Berry: return "Wiki Berry";
		case Item::Wise_Glasses: return "Wise Glasses";
		case Item::Works_Key: return "Works Key";
		case Item::X_Accuracy: return "X Accuracy";
		case Item::X_Attack: return "X Attack";
		case Item::X_Defend: return "X Defend";
		case Item::X_Sp_Def: return "X Sp. Def";
		case Item::X_Special: return "X Special";
		case Item::X_Speed: return "X Speed";
		case Item::Yache_Berry: return "Yache Berry";
		case Item::Yellow_Flute: return "Yellow Flute";
		case Item::Yellow_Scarf: return "Yellow Scarf";
		case Item::Yellow_Shard: return "Yellow Shard";
		case Item::Zap_Plate: return "Zap Plate";
		case Item::Zinc: return "Zinc";
		case Item::Zoom_Lens: return "Zoom Lens";
	}
}

template<>
Item from_string(std::string_view const str) {
	static std::map<std::string_view, Item, lowercase_alphanumeric> const converter {
		{ "NoItem", Item::No_Item },
		{ "AdamantOrb", Item::Adamant_Orb },
		{ "AguavBerry", Item::Aguav_Berry },
		{ "ApicotBerry", Item::Apicot_Berry },
		{ "AspearBerry", Item::Aspear_Berry },
		{ "BabiriBerry", Item::Babiri_Berry },
		{ "BelueBerry", Item::Belue_Berry },
		{ "BerryJuice", Item::Berry_Juice },
		{ "BigRoot", Item::Big_Root },
		{ "BlackBelt", Item::Black_Belt },
		{ "BlackSludge", Item::Black_Sludge },
		{ "BlackGlasses", Item::BlackGlasses },
		{ "BlukBerry", Item::Bluk_Berry },
		{ "BrightPowder", Item::BrightPowder },
		{ "Charcoal", Item::Charcoal },
		{ "ChartiBerry", Item::Charti_Berry },
		{ "CheriBerry", Item::Cheri_Berry },
		{ "ChestoBerry", Item::Chesto_Berry },
		{ "ChilanBerry", Item::Chilan_Berry },
		{ "ChoiceBand", Item::Choice_Band },
		{ "ChoiceScarf", Item::Choice_Scarf },
		{ "ChoiceSpecs", Item::Choice_Specs },
		{ "ChopleBerry", Item::Chople_Berry },
		{ "CobaBerry", Item::Coba_Berry },
		{ "ColburBerry", Item::Colbur_Berry },
		{ "CornnBerry", Item::Cornn_Berry },
		{ "CustapBerry", Item::Custap_Berry },
		{ "DampRock", Item::Damp_Rock },
		{ "DeepSeaScale", Item::DeepSeaScale },
		{ "DeepSeaTooth", Item::DeepSeaTooth },
		{ "DestinyKnot", Item::Destiny_Knot },
		{ "DracoPlate", Item::Draco_Plate },
		{ "DragonFang", Item::Dragon_Fang },
		{ "DreadPlate", Item::Dread_Plate },
		{ "DurinBerry", Item::Durin_Berry },
		{ "EarthPlate", Item::Earth_Plate },
		{ "EnigmaBerry", Item::Enigma_Berry },
		{ "ExpertBelt", Item::Expert_Belt },
		{ "FigyBerry", Item::Figy_Berry },
		{ "FistPlate", Item::Fist_Plate },
		{ "FlameOrb", Item::Flame_Orb },
		{ "FlamePlate", Item::Flame_Plate },
		{ "FocusBand", Item::Focus_Band },
		{ "FocusSash", Item::Focus_Sash },
		{ "GanlonBerry", Item::Ganlon_Berry },
		{ "GrepaBerry", Item::Grepa_Berry },
		{ "GripClaw", Item::Grip_Claw },
		{ "GriseousOrb", Item::Griseous_Orb },
		{ "HabanBerry", Item::Haban_Berry },
		{ "HardStone", Item::Hard_Stone },
		{ "HeatRock", Item::Heat_Rock },
		{ "HondewBerry", Item::Hondew_Berry },
		{ "IapapaBerry", Item::Iapapa_Berry },
		{ "IciclePlate", Item::Icicle_Plate },
		{ "IcyRock", Item::Icy_Rock },
		{ "InsectPlate", Item::Insect_Plate },
		{ "IronBall", Item::Iron_Ball },
		{ "IronPlate", Item::Iron_Plate },
		{ "JabocaBerry", Item::Jaboca_Berry },
		{ "KasibBerry", Item::Kasib_Berry },
		{ "KebiaBerry", Item::Kebia_Berry },
		{ "KelpsyBerry", Item::Kelpsy_Berry },
		{ "King'sRock", Item::Kings_Rock },
		{ "RazorFang", Item::Razor_Fang },
		{ "FullIncense", Item::Full_Incense },
		{ "LaggingTail", Item::Lagging_Tail },
		{ "LansatBerry", Item::Lansat_Berry },
		{ "LaxIncense", Item::Lax_Incense },
		{ "Leftovers", Item::Leftovers },
		{ "LeppaBerry", Item::Leppa_Berry },
		{ "LiechiBerry", Item::Liechi_Berry },
		{ "LifeOrb", Item::Life_Orb },
		{ "LightBall", Item::Light_Ball },
		{ "LightClay", Item::Light_Clay },
		{ "LuckyPunch", Item::Lucky_Punch },
		{ "LumBerry", Item::Lum_Berry },
		{ "LustrousOrb", Item::Lustrous_Orb },
		{ "MachoBrace", Item::Macho_Brace },
		{ "Magnet", Item::Magnet },
		{ "MagoBerry", Item::Mago_Berry },
		{ "MagostBerry", Item::Magost_Berry },
		{ "AirMail", Item::Air_Mail },
		{ "BloomMail", Item::Bloom_Mail },
		{ "BrickMail", Item::Brick_Mail },
		{ "BubbleMail", Item::Bubble_Mail },
		{ "FlameMail", Item::Flame_Mail },
		{ "GrassMail", Item::Grass_Mail },
		{ "HeartMail", Item::Heart_Mail },
		{ "MosaicMail", Item::Mosaic_Mail },
		{ "SnowMail", Item::Snow_Mail },
		{ "SpaceMail", Item::Space_Mail },
		{ "SteelMail", Item::Steel_Mail },
		{ "TunnelMail", Item::Tunnel_Mail },
		{ "MeadowPlate", Item::Meadow_Plate },
		{ "MentalHerb", Item::Mental_Herb },
		{ "MetalCoat", Item::Metal_Coat },
		{ "MetalPowder", Item::Metal_Powder },
		{ "Metronome", Item::Metronome },
		{ "MicleBerry", Item::Micle_Berry },
		{ "MindPlate", Item::Mind_Plate },
		{ "MiracleSeed", Item::Miracle_Seed },
		{ "MuscleBand", Item::Muscle_Band },
		{ "MysticWater", Item::Mystic_Water },
		{ "NanabBerry", Item::Nanab_Berry },
		{ "NeverMeltIce", Item::NeverMeltIce },
		{ "NomelBerry", Item::Nomel_Berry },
		{ "OccaBerry", Item::Occa_Berry },
		{ "OddIncense", Item::Odd_Incense },
		{ "OranBerry", Item::Oran_Berry },
		{ "PamtreBerry", Item::Pamtre_Berry },
		{ "PasshoBerry", Item::Passho_Berry },
		{ "PayapaBerry", Item::Payapa_Berry },
		{ "PechaBerry", Item::Pecha_Berry },
		{ "PersimBerry", Item::Persim_Berry },
		{ "PetayaBerry", Item::Petaya_Berry },
		{ "PinapBerry", Item::Pinap_Berry },
		{ "PoisonBarb", Item::Poison_Barb },
		{ "PomegBerry", Item::Pomeg_Berry },
		{ "PowerHerb", Item::Power_Herb },
		{ "PowerAnklet", Item::Power_Anklet },
		{ "PowerBand", Item::Power_Band },
		{ "PowerBelt", Item::Power_Belt },
		{ "PowerBracer", Item::Power_Bracer },
		{ "PowerLens", Item::Power_Lens },
		{ "PowerWeight", Item::Power_Weight },
		{ "QualotBerry", Item::Qualot_Berry },
		{ "QuickClaw", Item::Quick_Claw },
		{ "QuickPowder", Item::Quick_Powder },
		{ "RabutaBerry", Item::Rabuta_Berry },
		{ "RawstBerry", Item::Rawst_Berry },
		{ "RazorClaw", Item::Razor_Claw },
		{ "RazzBerry", Item::Razz_Berry },
		{ "RindoBerry", Item::Rindo_Berry },
		{ "RockIncense", Item::Rock_Incense },
		{ "RoseIncense", Item::Rose_Incense },
		{ "RowapBerry", Item::Rowap_Berry },
		{ "SalacBerry", Item::Salac_Berry },
		{ "ScopeLens", Item::Scope_Lens },
		{ "SharpBeak", Item::Sharp_Beak },
		{ "ShedShell", Item::Shed_Shell },
		{ "ShellBell", Item::Shell_Bell },
		{ "ShucaBerry", Item::Shuca_Berry },
		{ "SilkScarf", Item::Silk_Scarf },
		{ "SilverPowder", Item::SilverPowder },
		{ "SitrusBerry", Item::Sitrus_Berry },
		{ "SkyPlate", Item::Sky_Plate },
		{ "SmoothRock", Item::Smooth_Rock },
		{ "SoftSand", Item::Soft_Sand },
		{ "SoulDew", Item::Soul_Dew },
		{ "SpellTag", Item::Spell_Tag },
		{ "SpelonBerry", Item::Spelon_Berry },
		{ "SplashPlate", Item::Splash_Plate },
		{ "SpookyPlate", Item::Spooky_Plate },
		{ "StarfBerry", Item::Starf_Berry },
		{ "Stick", Item::Stick },
		{ "StickyBarb", Item::Sticky_Barb },
		{ "StonePlate", Item::Stone_Plate },
		{ "TamatoBerry", Item::Tamato_Berry },
		{ "TangaBerry", Item::Tanga_Berry },
		{ "ThickClub", Item::Thick_Club },
		{ "ToxicOrb", Item::Toxic_Orb },
		{ "ToxicPlate", Item::Toxic_Plate },
		{ "TwistedSpoon", Item::TwistedSpoon },
		{ "TwistedSpoon", Item::TwistedSpoon },
		{ "WacanBerry", Item::Wacan_Berry },
		{ "WatmelBerry", Item::Watmel_Berry },
		{ "SeaIncense", Item::Sea_Incense },
		{ "WaveIncense", Item::Wave_Incense },
		{ "WepearBerry", Item::Wepear_Berry },
		{ "WhiteHerb", Item::White_Herb },
		{ "WideLens", Item::Wide_Lens },
		{ "WikiBerry", Item::Wiki_Berry },
		{ "WiseGlasses", Item::Wise_Glasses },
		{ "YacheBerry", Item::Yache_Berry },
		{ "ZapPlate", Item::Zap_Plate },
		{ "ZoomLens", Item::Zoom_Lens },
		{ "CherishBall", Item::Cherish_Ball },
		{ "DiveBall", Item::Dive_Ball },
		{ "DuskBall", Item::Dusk_Ball },
		{ "GreatBall", Item::Great_Ball },
		{ "HealBall", Item::Heal_Ball },
		{ "LuxuryBall", Item::Luxury_Ball },
		{ "MasterBall", Item::Master_Ball },
		{ "NestBall", Item::Nest_Ball },
		{ "NetBall", Item::Net_Ball },
		{ "PokeBall", Item::Poke_Ball },
		{ "PremierBall", Item::Premier_Ball },
		{ "QuickBall", Item::Quick_Ball },
		{ "RepeatBall", Item::Repeat_Ball },
		{ "TimerBall", Item::Timer_Ball },
		{ "TM01", Item::TM01 },
		{ "TM02", Item::TM02 },
		{ "TM03", Item::TM03 },
		{ "TM04", Item::TM04 },
		{ "TM05", Item::TM05 },
		{ "TM06", Item::TM06 },
		{ "TM07", Item::TM07 },
		{ "TM08", Item::TM08 },
		{ "TM09", Item::TM09 },
		{ "TM10", Item::TM10 },
		{ "TM11", Item::TM11 },
		{ "TM12", Item::TM12 },
		{ "TM13", Item::TM13 },
		{ "TM14", Item::TM14 },
		{ "TM15", Item::TM15 },
		{ "TM16", Item::TM16 },
		{ "TM17", Item::TM17 },
		{ "TM18", Item::TM18 },
		{ "TM19", Item::TM19 },
		{ "TM20", Item::TM20 },
		{ "TM21", Item::TM21 },
		{ "TM22", Item::TM22 },
		{ "TM23", Item::TM23 },
		{ "TM24", Item::TM24 },
		{ "TM25", Item::TM25 },
		{ "TM26", Item::TM26 },
		{ "TM27", Item::TM27 },
		{ "TM28", Item::TM28 },
		{ "TM29", Item::TM29 },
		{ "TM30", Item::TM30 },
		{ "TM31", Item::TM31 },
		{ "TM32", Item::TM32 },
		{ "TM33", Item::TM33 },
		{ "TM34", Item::TM34 },
		{ "TM35", Item::TM35 },
		{ "TM36", Item::TM36 },
		{ "TM37", Item::TM37 },
		{ "TM38", Item::TM38 },
		{ "TM39", Item::TM39 },
		{ "TM40", Item::TM40 },
		{ "TM41", Item::TM41 },
		{ "TM42", Item::TM42 },
		{ "TM43", Item::TM43 },
		{ "TM44", Item::TM44 },
		{ "TM45", Item::TM45 },
		{ "TM46", Item::TM46 },
		{ "TM47", Item::TM47 },
		{ "TM48", Item::TM48 },
		{ "TM49", Item::TM49 },
		{ "TM50", Item::TM50 },
		{ "TM51", Item::TM51 },
		{ "TM52", Item::TM52 },
		{ "TM53", Item::TM53 },
		{ "TM54", Item::TM54 },
		{ "TM55", Item::TM55 },
		{ "TM56", Item::TM56 },
		{ "TM57", Item::TM57 },
		{ "TM58", Item::TM58 },
		{ "TM59", Item::TM59 },
		{ "TM60", Item::TM60 },
		{ "TM61", Item::TM61 },
		{ "TM62", Item::TM62 },
		{ "TM63", Item::TM63 },
		{ "TM64", Item::TM64 },
		{ "TM65", Item::TM65 },
		{ "TM66", Item::TM66 },
		{ "TM67", Item::TM67 },
		{ "TM68", Item::TM68 },
		{ "TM69", Item::TM69 },
		{ "TM70", Item::TM70 },
		{ "TM71", Item::TM71 },
		{ "TM72", Item::TM72 },
		{ "TM73", Item::TM73 },
		{ "TM74", Item::TM74 },
		{ "TM75", Item::TM75 },
		{ "TM76", Item::TM76 },
		{ "TM77", Item::TM77 },
		{ "TM78", Item::TM78 },
		{ "TM79", Item::TM79 },
		{ "TM80", Item::TM80 },
		{ "TM81", Item::TM81 },
		{ "TM82", Item::TM82 },
		{ "TM83", Item::TM83 },
		{ "TM84", Item::TM84 },
		{ "TM85", Item::TM85 },
		{ "TM86", Item::TM86 },
		{ "TM87", Item::TM87 },
		{ "TM88", Item::TM88 },
		{ "TM89", Item::TM89 },
		{ "TM90", Item::TM90 },
		{ "TM91", Item::TM91 },
		{ "TM92", Item::TM92 },
		{ "UltraBall", Item::Ultra_Ball },
		{ "BlueScarf", Item::Blue_Scarf },
		{ "GreenScarf", Item::Green_Scarf },
		{ "LuckIncense", Item::Luck_Incense },
		{ "PinkScarf", Item::Pink_Scarf },
		{ "PureIncense", Item::Pure_Incense },
		{ "ReaperCloth", Item::Reaper_Cloth },
		{ "RedScarf", Item::Red_Scarf },
		{ "SootheBell", Item::Soothe_Bell },
		{ "YellowScarf", Item::Yellow_Scarf },
		{ "AmuletCoin", Item::Amulet_Coin },
		{ "Antidote", Item::Antidote },
		{ "Awakening", Item::Awakening },
		{ "BigMushroom", Item::Big_Mushroom },
		{ "BigPearl", Item::Big_Pearl },
		{ "BlackFlute", Item::Black_Flute },
		{ "BlueFlute", Item::Blue_Flute },
		{ "BlueShard", Item::Blue_Shard },
		{ "BurnHeal", Item::Burn_Heal },
		{ "Calcium", Item::Calcium },
		{ "Carbos", Item::Carbos },
		{ "CleanseTag", Item::Cleanse_Tag },
		{ "DampMulch", Item::Damp_Mulch },
		{ "DireHit", Item::Dire_Hit },
		{ "DragonScale", Item::Dragon_Scale },
		{ "Elixir", Item::Elixir },
		{ "EnergyRoot", Item::Energy_Root },
		{ "EnergyPowder", Item::EnergyPowder },
		{ "EscapeRope", Item::Escape_Rope },
		{ "Ether", Item::Ether },
		{ "Everstone", Item::Everstone },
		{ "Exp.Share", Item::Exp_Share },
		{ "FireStone", Item::Fire_Stone },
		{ "FluffyTail", Item::Fluffy_Tail },
		{ "FreshWater", Item::Fresh_Water },
		{ "FullHeal", Item::Full_Heal },
		{ "FullRestore", Item::Full_Restore },
		{ "GooeyMulch", Item::Gooey_Mulch },
		{ "GreenShard", Item::Green_Shard },
		{ "GrowthMulch", Item::Growth_Mulch },
		{ "GuardSpec.", Item::Guard_Spec },
		{ "HealPowder", Item::Heal_Powder },
		{ "HeartScale", Item::Heart_Scale },
		{ "Honey", Item::Honey },
		{ "HPUp", Item::HP_Up },
		{ "HyperPotion", Item::Hyper_Potion },
		{ "IceHeal", Item::Ice_Heal },
		{ "Iron", Item::Iron },
		{ "LavaCookie", Item::Lava_Cookie },
		{ "LeafStone", Item::Leaf_Stone },
		{ "Lemonade", Item::Lemonade },
		{ "LuckyEgg", Item::Lucky_Egg },
		{ "MaxElixir", Item::Max_Elixir },
		{ "MaxEther", Item::Max_Ether },
		{ "MaxPotion", Item::Max_Potion },
		{ "MaxRepel", Item::Max_Repel },
		{ "MaxRevive", Item::Max_Revive },
		{ "MooMooMilk", Item::MooMoo_Milk },
		{ "MoonStone", Item::Moon_Stone },
		{ "Nugget", Item::Nugget },
		{ "OldGateau", Item::Old_Gateau },
		{ "ParlyzHeal", Item::Parlyz_Heal },
		{ "Pearl", Item::Pearl },
		{ "PokeDoll", Item::Poke_Doll },
		{ "Potion", Item::Potion },
		{ "PPMax", Item::PP_Max },
		{ "PPUp", Item::PP_Up },
		{ "Protein", Item::Protein },
		{ "RareCandy", Item::Rare_Candy },
		{ "RedFlute", Item::Red_Flute },
		{ "RedShard", Item::Red_Shard },
		{ "Repel", Item::Repel },
		{ "RevivalHerb", Item::Revival_Herb },
		{ "Revive", Item::Revive },
		{ "SacredAsh", Item::Sacred_Ash },
		{ "ShoalSalt", Item::Shoal_Salt },
		{ "ShoalShell", Item::Shoal_Shell },
		{ "SmokeBall", Item::Smoke_Ball },
		{ "SodaPop", Item::Soda_Pop },
		{ "StableMulch", Item::Stable_Mulch },
		{ "StarPiece", Item::Star_Piece },
		{ "Stardust", Item::Stardust },
		{ "SunStone", Item::Sun_Stone },
		{ "SuperPotion", Item::Super_Potion },
		{ "SuperRepel", Item::Super_Repel },
		{ "Thunderstone", Item::Thunderstone },
		{ "TinyMushroom", Item::TinyMushroom },
		{ "UpGrade", Item::Up_Grade },
		{ "WaterStone", Item::Water_Stone },
		{ "WhiteFlute", Item::White_Flute },
		{ "WorksKey", Item::Works_Key },
		{ "XAccuracy", Item::X_Accuracy },
		{ "XAttack", Item::X_Attack },
		{ "XDefend", Item::X_Defend },
		{ "XSp.Def", Item::X_Sp_Def },
		{ "XSpecial", Item::X_Special },
		{ "XSpeed", Item::X_Speed },
		{ "YellowFlute", Item::Yellow_Flute },
		{ "YellowShard", Item::Yellow_Shard },
		{ "Zinc", Item::Zinc },
		{ "DubiousDisc", Item::Dubious_Disc },
		{ "DawnStone", Item::Dawn_Stone },
		{ "DuskStone", Item::Dusk_Stone },
		{ "Electirizer", Item::Electirizer },
		{ "Magmarizer", Item::Magmarizer },
		{ "OddKeystone", Item::Odd_Keystone },
		{ "OvalStone", Item::Oval_Stone },
		{ "Protector", Item::Protector },
		{ "ShinyStone", Item::Shiny_Stone },
		{ "ArmorFossil", Item::Armor_Fossil },
		{ "ClawFossil", Item::Claw_Fossil },
		{ "DomeFossil", Item::Dome_Fossil },
		{ "HelixFossil", Item::Helix_Fossil },
		{ "OldAmber", Item::Old_Amber },
		{ "RareBone", Item::Rare_Bone },
		{ "RootFossil", Item::Root_Fossil },
		{ "SkullFossil", Item::Skull_Fossil },
		{ "Bicycle", Item::Bicycle },
		{ "CoinCase", Item::Coin_Case },
		{ "Coupon1", Item::Coupon_1 },
		{ "Coupon2", Item::Coupon_2 },
		{ "Coupon3", Item::Coupon_3 },
		{ "ExplorerKit", Item::Explorer_Kit },
		{ "FashionCase", Item::Fashion_Case },
		{ "GalacticKey", Item::Galactic_Key },
		{ "GoodRod", Item::Good_Rod },
		{ "HM01", Item::HM01 },
		{ "HM02", Item::HM02 },
		{ "HM03", Item::HM03 },
		{ "HM04", Item::HM04 },
		{ "HM05", Item::HM05 },
		{ "HM06", Item::HM06 },
		{ "HM07", Item::HM07 },
		{ "HM08", Item::HM08 },
		{ "Journal", Item::Journal },
		{ "LootSack", Item::Loot_Sack },
		{ "LunarWing", Item::Lunar_Wing },
		{ "MemberCard", Item::Member_Card},
		{ "Oak'sLetter", Item::Oaks_Letter },
		{ "OldCharm", Item::Old_Charm },
		{ "OldRod", Item::Old_Rod },
		{ "PalPad", Item::Pal_Pad },
		{ "Parcel", Item::Parcel },
		{ "PoffinCase", Item::Poffin_Case },
		{ "PointCard", Item::Point_Card },
		{ "PokeRadar", Item::Poke_Radar },
		{ "SafariBall", Item::Safari_Ball },
		{ "SealBag", Item::Seal_Bag },
		{ "SealCase", Item::Seal_Case },
		{ "SecretPotion", Item::SecretPotion },
		{ "Sprayduck", Item::Sprayduck },
		{ "StorageKey", Item::Storage_Key },
		{ "SuiteKey", Item::Suite_Key },
		{ "SuperRod", Item::Super_Rod },
		{ "TownMap", Item::Town_Map },
		{ "VSSeeker", Item::VS_Seeker },
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Item", str);
	}
}

}	// namespace technicalmachine
