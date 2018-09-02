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

#include "item.hpp"

#include "invalid_string_conversion.hpp"

#include "../item.hpp"

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
	static std::map<std::string_view, Item, lowercase_ordering> const converter {
		{ "No Item", Item::No_Item },
		{ "NoItem", Item::No_Item },
		{ "Adamant Orb", Item::Adamant_Orb },
		{ "AdamantOrb", Item::Adamant_Orb },
		{ "Aguav Berry", Item::Aguav_Berry },
		{ "AguavBerry", Item::Aguav_Berry },
		{ "Apicot Berry", Item::Apicot_Berry },
		{ "ApicotBerry", Item::Apicot_Berry },
		{ "Aspear Berry", Item::Aspear_Berry },
		{ "AspearBerry", Item::Aspear_Berry },
		{ "Babiri Berry", Item::Babiri_Berry },
		{ "BabiriBerry", Item::Babiri_Berry },
		{ "Belue Berry", Item::Belue_Berry },
		{ "BelueBerry", Item::Belue_Berry },
		{ "Berry Juice", Item::Berry_Juice },
		{ "BerryJuice", Item::Berry_Juice },
		{ "Big Root", Item::Big_Root },
		{ "BigRoot", Item::Big_Root },
		{ "Black Belt", Item::Black_Belt },
		{ "BlackBelt", Item::Black_Belt },
		{ "Black Sludge", Item::Black_Sludge },
		{ "BlackSludge", Item::Black_Sludge },
		{ "BlackGlasses", Item::BlackGlasses },
		{ "Bluk Berry", Item::Bluk_Berry },
		{ "BlukBerry", Item::Bluk_Berry },
		{ "BrightPowder", Item::BrightPowder },
		{ "Charcoal", Item::Charcoal },
		{ "Charti Berry", Item::Charti_Berry },
		{ "ChartiBerry", Item::Charti_Berry },
		{ "Cheri Berry", Item::Cheri_Berry },
		{ "CheriBerry", Item::Cheri_Berry },
		{ "Chesto Berry", Item::Chesto_Berry },
		{ "ChestoBerry", Item::Chesto_Berry },
		{ "Chilan Berry", Item::Chilan_Berry },
		{ "ChilanBerry", Item::Chilan_Berry },
		{ "Choice Band", Item::Choice_Band },
		{ "ChoiceBand", Item::Choice_Band },
		{ "Choice Scarf", Item::Choice_Scarf },
		{ "ChoiceScarf", Item::Choice_Scarf },
		{ "Choice Specs", Item::Choice_Specs },
		{ "ChoiceSpecs", Item::Choice_Specs },
		{ "Chople Berry", Item::Chople_Berry },
		{ "ChopleBerry", Item::Chople_Berry },
		{ "Coba Berry", Item::Coba_Berry },
		{ "CobaBerry", Item::Coba_Berry },
		{ "Colbur Berry", Item::Colbur_Berry },
		{ "ColburBerry", Item::Colbur_Berry },
		{ "Cornn Berry", Item::Cornn_Berry },
		{ "CornnBerry", Item::Cornn_Berry },
		{ "Custap Berry", Item::Custap_Berry },
		{ "CustapBerry", Item::Custap_Berry },
		{ "Damp Rock", Item::Damp_Rock },
		{ "DampRock", Item::Damp_Rock },
		{ "DeepSeaScale", Item::DeepSeaScale },
		{ "DeepSeaTooth", Item::DeepSeaTooth },
		{ "Destiny Knot", Item::Destiny_Knot },
		{ "DestinyKnot", Item::Destiny_Knot },
		{ "Draco Plate", Item::Draco_Plate },
		{ "DracoPlate", Item::Draco_Plate },
		{ "Dragon Fang", Item::Dragon_Fang },
		{ "DragonFang", Item::Dragon_Fang },
		{ "Dread Plate", Item::Dread_Plate },
		{ "DreadPlate", Item::Dread_Plate },
		{ "Durin Berry", Item::Durin_Berry },
		{ "DurinBerry", Item::Durin_Berry },
		{ "Earth Plate", Item::Earth_Plate },
		{ "EarthPlate", Item::Earth_Plate },
		{ "Enigma Berry", Item::Enigma_Berry },
		{ "EnigmaBerry", Item::Enigma_Berry },
		{ "Expert Belt", Item::Expert_Belt },
		{ "ExpertBelt", Item::Expert_Belt },
		{ "Figy Berry", Item::Figy_Berry },
		{ "FigyBerry", Item::Figy_Berry },
		{ "Fist Plate", Item::Fist_Plate },
		{ "FistPlate", Item::Fist_Plate },
		{ "Flame Orb", Item::Flame_Orb },
		{ "FlameOrb", Item::Flame_Orb },
		{ "Flame Plate", Item::Flame_Plate },
		{ "FlamePlate", Item::Flame_Plate },
		{ "Focus Band", Item::Focus_Band },
		{ "FocusBand", Item::Focus_Band },
		{ "Focus Sash", Item::Focus_Sash },
		{ "FocusSash", Item::Focus_Sash },
		{ "Ganlon Berry", Item::Ganlon_Berry },
		{ "GanlonBerry", Item::Ganlon_Berry },
		{ "Grepa Berry", Item::Grepa_Berry },
		{ "GrepaBerry", Item::Grepa_Berry },
		{ "Grip Claw", Item::Grip_Claw },
		{ "GripClaw", Item::Grip_Claw },
		{ "Griseous Orb", Item::Griseous_Orb },
		{ "GriseousOrb", Item::Griseous_Orb },
		{ "Haban Berry", Item::Haban_Berry },
		{ "HabanBerry", Item::Haban_Berry },
		{ "Hard Stone", Item::Hard_Stone },
		{ "HardStone", Item::Hard_Stone },
		{ "Heat Rock", Item::Heat_Rock },
		{ "HeatRock", Item::Heat_Rock },
		{ "Hondew Berry", Item::Hondew_Berry },
		{ "HondewBerry", Item::Hondew_Berry },
		{ "Iapapa Berry", Item::Iapapa_Berry },
		{ "IapapaBerry", Item::Iapapa_Berry },
		{ "Icicle Plate", Item::Icicle_Plate },
		{ "IciclePlate", Item::Icicle_Plate },
		{ "Icy Rock", Item::Icy_Rock },
		{ "IcyRock", Item::Icy_Rock },
		{ "Insect Plate", Item::Insect_Plate },
		{ "InsectPlate", Item::Insect_Plate },
		{ "Iron Ball", Item::Iron_Ball },
		{ "IronBall", Item::Iron_Ball },
		{ "Iron Plate", Item::Iron_Plate },
		{ "IronPlate", Item::Iron_Plate },
		{ "Jaboca Berry", Item::Jaboca_Berry },
		{ "JabocaBerry", Item::Jaboca_Berry },
		{ "Kasib Berry", Item::Kasib_Berry },
		{ "KasibBerry", Item::Kasib_Berry },
		{ "Kebia Berry", Item::Kebia_Berry },
		{ "KebiaBerry", Item::Kebia_Berry },
		{ "Kelpsy Berry", Item::Kelpsy_Berry },
		{ "KelpsyBerry", Item::Kelpsy_Berry },
		{ "King's Rock", Item::Kings_Rock },
		{ "King'sRock", Item::Kings_Rock },
		{ "Razor Fang", Item::Razor_Fang },
		{ "RazorFang", Item::Razor_Fang },
		{ "Full Incense", Item::Full_Incense },
		{ "FullIncense", Item::Full_Incense },
		{ "Lagging Tail", Item::Lagging_Tail },
		{ "LaggingTail", Item::Lagging_Tail },
		{ "Lansat Berry", Item::Lansat_Berry },
		{ "LansatBerry", Item::Lansat_Berry },
		{ "Lax Incense", Item::Lax_Incense },
		{ "LaxIncense", Item::Lax_Incense },
		{ "Leftovers", Item::Leftovers },
		{ "Leppa Berry", Item::Leppa_Berry },
		{ "LeppaBerry", Item::Leppa_Berry },
		{ "Liechi Berry", Item::Liechi_Berry },
		{ "LiechiBerry", Item::Liechi_Berry },
		{ "Life Orb", Item::Life_Orb },
		{ "LifeOrb", Item::Life_Orb },
		{ "Light Ball", Item::Light_Ball },
		{ "LightBall", Item::Light_Ball },
		{ "Light Clay", Item::Light_Clay },
		{ "LightClay", Item::Light_Clay },
		{ "Lucky Punch", Item::Lucky_Punch },
		{ "LuckyPunch", Item::Lucky_Punch },
		{ "Lum Berry", Item::Lum_Berry },
		{ "LumBerry", Item::Lum_Berry },
		{ "Lustrous Orb", Item::Lustrous_Orb },
		{ "LustrousOrb", Item::Lustrous_Orb },
		{ "Macho Brace", Item::Macho_Brace },
		{ "MachoBrace", Item::Macho_Brace },
		{ "Magnet", Item::Magnet },
		{ "Mago Berry", Item::Mago_Berry },
		{ "MagoBerry", Item::Mago_Berry },
		{ "Magost Berry", Item::Magost_Berry },
		{ "MagostBerry", Item::Magost_Berry },
		{ "Air Mail", Item::Air_Mail },
		{ "AirMail", Item::Air_Mail },
		{ "Bloom Mail", Item::Bloom_Mail },
		{ "BloomMail", Item::Bloom_Mail },
		{ "Brick Mail", Item::Brick_Mail },
		{ "BrickMail", Item::Brick_Mail },
		{ "Bubble Mail", Item::Bubble_Mail },
		{ "BubbleMail", Item::Bubble_Mail },
		{ "Flame Mail", Item::Flame_Mail },
		{ "FlameMail", Item::Flame_Mail },
		{ "Grass Mail", Item::Grass_Mail },
		{ "GrassMail", Item::Grass_Mail },
		{ "Heart Mail", Item::Heart_Mail },
		{ "HeartMail", Item::Heart_Mail },
		{ "Mosaic Mail", Item::Mosaic_Mail },
		{ "MosaicMail", Item::Mosaic_Mail },
		{ "Snow Mail", Item::Snow_Mail },
		{ "SnowMail", Item::Snow_Mail },
		{ "Space Mail", Item::Space_Mail },
		{ "SpaceMail", Item::Space_Mail },
		{ "Steel Mail", Item::Steel_Mail },
		{ "SteelMail", Item::Steel_Mail },
		{ "Tunnel Mail", Item::Tunnel_Mail },
		{ "TunnelMail", Item::Tunnel_Mail },
		{ "Meadow Plate", Item::Meadow_Plate },
		{ "MeadowPlate", Item::Meadow_Plate },
		{ "Mental Herb", Item::Mental_Herb },
		{ "MentalHerb", Item::Mental_Herb },
		{ "Metal Coat", Item::Metal_Coat },
		{ "MetalCoat", Item::Metal_Coat },
		{ "Metal Powder", Item::Metal_Powder },
		{ "MetalPowder", Item::Metal_Powder },
		{ "Metronome", Item::Metronome },
		{ "Micle Berry", Item::Micle_Berry },
		{ "MicleBerry", Item::Micle_Berry },
		{ "Mind Plate", Item::Mind_Plate },
		{ "MindPlate", Item::Mind_Plate },
		{ "Miracle Seed", Item::Miracle_Seed },
		{ "MiracleSeed", Item::Miracle_Seed },
		{ "Muscle Band", Item::Muscle_Band },
		{ "MuscleBand", Item::Muscle_Band },
		{ "Mystic Water", Item::Mystic_Water },
		{ "MysticWater", Item::Mystic_Water },
		{ "Nanab Berry", Item::Nanab_Berry },
		{ "NanabBerry", Item::Nanab_Berry },
		{ "NeverMeltIce", Item::NeverMeltIce },
		{ "Nomel Berry", Item::Nomel_Berry },
		{ "NomelBerry", Item::Nomel_Berry },
		{ "Occa Berry", Item::Occa_Berry },
		{ "OccaBerry", Item::Occa_Berry },
		{ "Odd Incense", Item::Odd_Incense },
		{ "OddIncense", Item::Odd_Incense },
		{ "Oran Berry", Item::Oran_Berry },
		{ "OranBerry", Item::Oran_Berry },
		{ "Pamtre Berry", Item::Pamtre_Berry },
		{ "PamtreBerry", Item::Pamtre_Berry },
		{ "Passho Berry", Item::Passho_Berry },
		{ "PasshoBerry", Item::Passho_Berry },
		{ "Payapa Berry", Item::Payapa_Berry },
		{ "PayapaBerry", Item::Payapa_Berry },
		{ "Pecha Berry", Item::Pecha_Berry },
		{ "PechaBerry", Item::Pecha_Berry },
		{ "Persim Berry", Item::Persim_Berry },
		{ "PersimBerry", Item::Persim_Berry },
		{ "Petaya Berry", Item::Petaya_Berry },
		{ "PetayaBerry", Item::Petaya_Berry },
		{ "Pinap Berry", Item::Pinap_Berry },
		{ "PinapBerry", Item::Pinap_Berry },
		{ "Poison Barb", Item::Poison_Barb },
		{ "PoisonBarb", Item::Poison_Barb },
		{ "Pomeg Berry", Item::Pomeg_Berry },
		{ "PomegBerry", Item::Pomeg_Berry },
		{ "Power Herb", Item::Power_Herb },
		{ "PowerHerb", Item::Power_Herb },
		{ "Power Anklet", Item::Power_Anklet },
		{ "PowerAnklet", Item::Power_Anklet },
		{ "Power Band", Item::Power_Band },
		{ "PowerBand", Item::Power_Band },
		{ "Power Belt", Item::Power_Belt },
		{ "PowerBelt", Item::Power_Belt },
		{ "Power Bracer", Item::Power_Bracer },
		{ "PowerBracer", Item::Power_Bracer },
		{ "Power Lens", Item::Power_Lens },
		{ "PowerLens", Item::Power_Lens },
		{ "Power Weight", Item::Power_Weight },
		{ "PowerWeight", Item::Power_Weight },
		{ "Qualot Berry", Item::Qualot_Berry },
		{ "QualotBerry", Item::Qualot_Berry },
		{ "Quick Claw", Item::Quick_Claw },
		{ "QuickClaw", Item::Quick_Claw },
		{ "Quick Powder", Item::Quick_Powder },
		{ "QuickPowder", Item::Quick_Powder },
		{ "Rabuta Berry", Item::Rabuta_Berry },
		{ "RabutaBerry", Item::Rabuta_Berry },
		{ "Rawst Berry", Item::Rawst_Berry },
		{ "RawstBerry", Item::Rawst_Berry },
		{ "Razor Claw", Item::Razor_Claw },
		{ "RazorClaw", Item::Razor_Claw },
		{ "Razz Berry", Item::Razz_Berry },
		{ "RazzBerry", Item::Razz_Berry },
		{ "Rindo Berry", Item::Rindo_Berry },
		{ "RindoBerry", Item::Rindo_Berry },
		{ "Rock Incense", Item::Rock_Incense },
		{ "RockIncense", Item::Rock_Incense },
		{ "Rose Incense", Item::Rose_Incense },
		{ "RoseIncense", Item::Rose_Incense },
		{ "Rowap Berry", Item::Rowap_Berry },
		{ "RowapBerry", Item::Rowap_Berry },
		{ "Salac Berry", Item::Salac_Berry },
		{ "SalacBerry", Item::Salac_Berry },
		{ "Scope Lens", Item::Scope_Lens },
		{ "ScopeLens", Item::Scope_Lens },
		{ "Sharp Beak", Item::Sharp_Beak },
		{ "SharpBeak", Item::Sharp_Beak },
		{ "Shed Shell", Item::Shed_Shell },
		{ "ShedShell", Item::Shed_Shell },
		{ "Shell Bell", Item::Shell_Bell },
		{ "ShellBell", Item::Shell_Bell },
		{ "Shuca Berry", Item::Shuca_Berry },
		{ "ShucaBerry", Item::Shuca_Berry },
		{ "Silk Scarf", Item::Silk_Scarf },
		{ "SilkScarf", Item::Silk_Scarf },
		{ "SilverPowder", Item::SilverPowder },
		{ "Sitrus Berry", Item::Sitrus_Berry },
		{ "SitrusBerry", Item::Sitrus_Berry },
		{ "Sky Plate", Item::Sky_Plate },
		{ "SkyPlate", Item::Sky_Plate },
		{ "Smooth Rock", Item::Smooth_Rock },
		{ "SmoothRock", Item::Smooth_Rock },
		{ "Soft Sand", Item::Soft_Sand },
		{ "SoftSand", Item::Soft_Sand },
		{ "Soul Dew", Item::Soul_Dew },
		{ "SoulDew", Item::Soul_Dew },
		{ "Spell Tag", Item::Spell_Tag },
		{ "SpellTag", Item::Spell_Tag },
		{ "Spelon Berry", Item::Spelon_Berry },
		{ "SpelonBerry", Item::Spelon_Berry },
		{ "Splash Plate", Item::Splash_Plate },
		{ "SplashPlate", Item::Splash_Plate },
		{ "Spooky Plate", Item::Spooky_Plate },
		{ "SpookyPlate", Item::Spooky_Plate },
		{ "Starf Berry", Item::Starf_Berry },
		{ "StarfBerry", Item::Starf_Berry },
		{ "Stick", Item::Stick },
		{ "Sticky Barb", Item::Sticky_Barb },
		{ "StickyBarb", Item::Sticky_Barb },
		{ "Stone Plate", Item::Stone_Plate },
		{ "StonePlate", Item::Stone_Plate },
		{ "Tamato Berry", Item::Tamato_Berry },
		{ "TamatoBerry", Item::Tamato_Berry },
		{ "Tanga Berry", Item::Tanga_Berry },
		{ "TangaBerry", Item::Tanga_Berry },
		{ "Thick Club", Item::Thick_Club },
		{ "ThickClub", Item::Thick_Club },
		{ "Toxic Orb", Item::Toxic_Orb },
		{ "ToxicOrb", Item::Toxic_Orb },
		{ "Toxic Plate", Item::Toxic_Plate },
		{ "ToxicPlate", Item::Toxic_Plate },
		{ "TwistedSpoon", Item::TwistedSpoon },
		{ "Twisted Spoon", Item::TwistedSpoon },
		{ "TwistedSpoon", Item::TwistedSpoon },
		{ "Wacan Berry", Item::Wacan_Berry },
		{ "WacanBerry", Item::Wacan_Berry },
		{ "Watmel Berry", Item::Watmel_Berry },
		{ "WatmelBerry", Item::Watmel_Berry },
		{ "Sea Incense", Item::Sea_Incense },
		{ "SeaIncense", Item::Sea_Incense },
		{ "Wave Incense", Item::Wave_Incense },
		{ "WaveIncense", Item::Wave_Incense },
		{ "Wepear Berry", Item::Wepear_Berry },
		{ "WepearBerry", Item::Wepear_Berry },
		{ "White Herb", Item::White_Herb },
		{ "WhiteHerb", Item::White_Herb },
		{ "Wide Lens", Item::Wide_Lens },
		{ "WideLens", Item::Wide_Lens },
		{ "Wiki Berry", Item::Wiki_Berry },
		{ "WikiBerry", Item::Wiki_Berry },
		{ "Wise Glasses", Item::Wise_Glasses },
		{ "WiseGlasses", Item::Wise_Glasses },
		{ "Yache Berry", Item::Yache_Berry },
		{ "YacheBerry", Item::Yache_Berry },
		{ "Zap Plate", Item::Zap_Plate },
		{ "ZapPlate", Item::Zap_Plate },
		{ "Zoom Lens", Item::Zoom_Lens },
		{ "ZoomLens", Item::Zoom_Lens },
		{ "Cherish Ball", Item::Cherish_Ball },
		{ "CherishBall", Item::Cherish_Ball },
		{ "Dive Ball", Item::Dive_Ball },
		{ "DiveBall", Item::Dive_Ball },
		{ "Dusk Ball", Item::Dusk_Ball },
		{ "DuskBall", Item::Dusk_Ball },
		{ "Great Ball", Item::Great_Ball },
		{ "GreatBall", Item::Great_Ball },
		{ "Heal Ball", Item::Heal_Ball },
		{ "HealBall", Item::Heal_Ball },
		{ "Luxury Ball", Item::Luxury_Ball },
		{ "LuxuryBall", Item::Luxury_Ball },
		{ "Master Ball", Item::Master_Ball },
		{ "MasterBall", Item::Master_Ball },
		{ "Nest Ball", Item::Nest_Ball },
		{ "NestBall", Item::Nest_Ball },
		{ "Net Ball", Item::Net_Ball },
		{ "NetBall", Item::Net_Ball },
		{ "Poke Ball", Item::Poke_Ball },
		{ "PokeBall", Item::Poke_Ball },
		{ "Premier Ball", Item::Premier_Ball },
		{ "PremierBall", Item::Premier_Ball },
		{ "Quick Ball", Item::Quick_Ball },
		{ "QuickBall", Item::Quick_Ball },
		{ "Repeat Ball", Item::Repeat_Ball },
		{ "RepeatBall", Item::Repeat_Ball },
		{ "Timer Ball", Item::Timer_Ball },
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
		{ "Ultra Ball", Item::Ultra_Ball },
		{ "UltraBall", Item::Ultra_Ball },
		{ "Blue Scarf", Item::Blue_Scarf },
		{ "BlueScarf", Item::Blue_Scarf },
		{ "Green Scarf", Item::Green_Scarf },
		{ "GreenScarf", Item::Green_Scarf },
		{ "Luck Incense", Item::Luck_Incense },
		{ "LuckIncense", Item::Luck_Incense },
		{ "Pink Scarf", Item::Pink_Scarf },
		{ "PinkScarf", Item::Pink_Scarf },
		{ "Pure Incense", Item::Pure_Incense },
		{ "PureIncense", Item::Pure_Incense },
		{ "Reaper Cloth", Item::Reaper_Cloth },
		{ "ReaperCloth", Item::Reaper_Cloth },
		{ "Red Scarf", Item::Red_Scarf },
		{ "RedScarf", Item::Red_Scarf },
		{ "Soothe Bell", Item::Soothe_Bell },
		{ "SootheBell", Item::Soothe_Bell },
		{ "Yellow Scarf", Item::Yellow_Scarf },
		{ "YellowScarf", Item::Yellow_Scarf },
		{ "Amulet Coin", Item::Amulet_Coin },
		{ "AmuletCoin", Item::Amulet_Coin },
		{ "Antidote", Item::Antidote },
		{ "Awakening", Item::Awakening },
		{ "Big Mushroom", Item::Big_Mushroom },
		{ "BigMushroom", Item::Big_Mushroom },
		{ "Big Pearl", Item::Big_Pearl },
		{ "BigPearl", Item::Big_Pearl },
		{ "Black Flute", Item::Black_Flute },
		{ "BlackFlute", Item::Black_Flute },
		{ "Blue Flute", Item::Blue_Flute },
		{ "BlueFlute", Item::Blue_Flute },
		{ "Blue Shard", Item::Blue_Shard },
		{ "BlueShard", Item::Blue_Shard },
		{ "Burn Heal", Item::Burn_Heal },
		{ "BurnHeal", Item::Burn_Heal },
		{ "Calcium", Item::Calcium },
		{ "Carbos", Item::Carbos },
		{ "Cleanse Tag", Item::Cleanse_Tag },
		{ "CleanseTag", Item::Cleanse_Tag },
		{ "Damp Mulch", Item::Damp_Mulch },
		{ "DampMulch", Item::Damp_Mulch },
		{ "Dire Hit", Item::Dire_Hit },
		{ "DireHit", Item::Dire_Hit },
		{ "Dragon Scale", Item::Dragon_Scale },
		{ "DragonScale", Item::Dragon_Scale },
		{ "Elixir", Item::Elixir },
		{ "Energy Root", Item::Energy_Root },
		{ "EnergyRoot", Item::Energy_Root },
		{ "EnergyPowder", Item::EnergyPowder },
		{ "Escape Rope", Item::Escape_Rope },
		{ "EscapeRope", Item::Escape_Rope },
		{ "Ether", Item::Ether },
		{ "Everstone", Item::Everstone },
		{ "Exp. Share", Item::Exp_Share },
		{ "Exp.Share", Item::Exp_Share },
		{ "Fire Stone", Item::Fire_Stone },
		{ "FireStone", Item::Fire_Stone },
		{ "Fluffy Tail", Item::Fluffy_Tail },
		{ "FluffyTail", Item::Fluffy_Tail },
		{ "Fresh Water", Item::Fresh_Water },
		{ "FreshWater", Item::Fresh_Water },
		{ "Full Heal", Item::Full_Heal },
		{ "FullHeal", Item::Full_Heal },
		{ "Full Restore", Item::Full_Restore },
		{ "FullRestore", Item::Full_Restore },
		{ "Gooey Mulch", Item::Gooey_Mulch },
		{ "GooeyMulch", Item::Gooey_Mulch },
		{ "Green Shard", Item::Green_Shard },
		{ "GreenShard", Item::Green_Shard },
		{ "Growth Mulch", Item::Growth_Mulch },
		{ "GrowthMulch", Item::Growth_Mulch },
		{ "Guard Spec.", Item::Guard_Spec },
		{ "GuardSpec.", Item::Guard_Spec },
		{ "Heal Powder", Item::Heal_Powder },
		{ "HealPowder", Item::Heal_Powder },
		{ "Heart Scale", Item::Heart_Scale },
		{ "HeartScale", Item::Heart_Scale },
		{ "Honey", Item::Honey },
		{ "HP Up", Item::HP_Up },
		{ "HPUp", Item::HP_Up },
		{ "Hyper Potion", Item::Hyper_Potion },
		{ "HyperPotion", Item::Hyper_Potion },
		{ "Ice Heal", Item::Ice_Heal },
		{ "IceHeal", Item::Ice_Heal },
		{ "Iron", Item::Iron },
		{ "Lava Cookie", Item::Lava_Cookie },
		{ "LavaCookie", Item::Lava_Cookie },
		{ "Leaf Stone", Item::Leaf_Stone },
		{ "LeafStone", Item::Leaf_Stone },
		{ "Lemonade", Item::Lemonade },
		{ "Lucky Egg", Item::Lucky_Egg },
		{ "LuckyEgg", Item::Lucky_Egg },
		{ "Max Elixir", Item::Max_Elixir },
		{ "MaxElixir", Item::Max_Elixir },
		{ "Max Ether", Item::Max_Ether },
		{ "MaxEther", Item::Max_Ether },
		{ "Max Potion", Item::Max_Potion },
		{ "MaxPotion", Item::Max_Potion },
		{ "Max Repel", Item::Max_Repel },
		{ "MaxRepel", Item::Max_Repel },
		{ "Max Revive", Item::Max_Revive },
		{ "MaxRevive", Item::Max_Revive },
		{ "MooMoo Milk", Item::MooMoo_Milk },
		{ "MooMooMilk", Item::MooMoo_Milk },
		{ "Moon Stone", Item::Moon_Stone },
		{ "MoonStone", Item::Moon_Stone },
		{ "Nugget", Item::Nugget },
		{ "Old Gateau", Item::Old_Gateau },
		{ "OldGateau", Item::Old_Gateau },
		{ "Parlyz Heal", Item::Parlyz_Heal },
		{ "ParlyzHeal", Item::Parlyz_Heal },
		{ "Pearl", Item::Pearl },
		{ "Poke Doll", Item::Poke_Doll },
		{ "PokeDoll", Item::Poke_Doll },
		{ "Potion", Item::Potion },
		{ "PP Max", Item::PP_Max },
		{ "PPMax", Item::PP_Max },
		{ "PP Up", Item::PP_Up },
		{ "PPUp", Item::PP_Up },
		{ "Protein", Item::Protein },
		{ "Rare Candy", Item::Rare_Candy },
		{ "RareCandy", Item::Rare_Candy },
		{ "Red Flute", Item::Red_Flute },
		{ "RedFlute", Item::Red_Flute },
		{ "Red Shard", Item::Red_Shard },
		{ "RedShard", Item::Red_Shard },
		{ "Repel", Item::Repel },
		{ "Revival Herb", Item::Revival_Herb },
		{ "RevivalHerb", Item::Revival_Herb },
		{ "Revive", Item::Revive },
		{ "Sacred Ash", Item::Sacred_Ash },
		{ "SacredAsh", Item::Sacred_Ash },
		{ "Shoal Salt", Item::Shoal_Salt },
		{ "ShoalSalt", Item::Shoal_Salt },
		{ "Shoal Shell", Item::Shoal_Shell },
		{ "ShoalShell", Item::Shoal_Shell },
		{ "Smoke Ball", Item::Smoke_Ball },
		{ "SmokeBall", Item::Smoke_Ball },
		{ "Soda Pop", Item::Soda_Pop },
		{ "SodaPop", Item::Soda_Pop },
		{ "Stable Mulch", Item::Stable_Mulch },
		{ "StableMulch", Item::Stable_Mulch },
		{ "Star Piece", Item::Star_Piece },
		{ "StarPiece", Item::Star_Piece },
		{ "Stardust", Item::Stardust },
		{ "Sun Stone", Item::Sun_Stone },
		{ "SunStone", Item::Sun_Stone },
		{ "Super Potion", Item::Super_Potion },
		{ "SuperPotion", Item::Super_Potion },
		{ "Super Repel", Item::Super_Repel },
		{ "SuperRepel", Item::Super_Repel },
		{ "Thunderstone", Item::Thunderstone },
		{ "TinyMushroom", Item::TinyMushroom },
		{ "Up-Grade", Item::Up_Grade },
		{ "Water Stone", Item::Water_Stone },
		{ "WaterStone", Item::Water_Stone },
		{ "White Flute", Item::White_Flute },
		{ "WhiteFlute", Item::White_Flute },
		{ "X Accuracy", Item::X_Accuracy },
		{ "XAccuracy", Item::X_Accuracy },
		{ "X Attack", Item::X_Attack },
		{ "XAttack", Item::X_Attack },
		{ "X Defend", Item::X_Defend },
		{ "XDefend", Item::X_Defend },
		{ "X Sp. Def", Item::X_Sp_Def },
		{ "XSp.Def", Item::X_Sp_Def },
		{ "X Special", Item::X_Special },
		{ "XSpecial", Item::X_Special },
		{ "X Speed", Item::X_Speed },
		{ "XSpeed", Item::X_Speed },
		{ "Yellow Flute", Item::Yellow_Flute },
		{ "YellowFlute", Item::Yellow_Flute },
		{ "Yellow Shard", Item::Yellow_Shard },
		{ "YellowShard", Item::Yellow_Shard },
		{ "Zinc", Item::Zinc },
		{ "Dubious Disc", Item::Dubious_Disc },
		{ "DubiousDisc", Item::Dubious_Disc },
		{ "Dawn Stone", Item::Dawn_Stone },
		{ "DawnStone", Item::Dawn_Stone },
		{ "Dusk Stone", Item::Dusk_Stone },
		{ "DuskStone", Item::Dusk_Stone },
		{ "Electirizer", Item::Electirizer },
		{ "Magmarizer", Item::Magmarizer },
		{ "Odd Keystone", Item::Odd_Keystone },
		{ "OddKeystone", Item::Odd_Keystone },
		{ "Oval Stone", Item::Oval_Stone },
		{ "OvalStone", Item::Oval_Stone },
		{ "Protector", Item::Protector },
		{ "Shiny Stone", Item::Shiny_Stone },
		{ "ShinyStone", Item::Shiny_Stone },
		{ "Armor Fossil", Item::Armor_Fossil },
		{ "ArmorFossil", Item::Armor_Fossil },
		{ "Claw Fossil", Item::Claw_Fossil },
		{ "ClawFossil", Item::Claw_Fossil },
		{ "Dome Fossil", Item::Dome_Fossil },
		{ "DomeFossil", Item::Dome_Fossil },
		{ "Helix Fossil", Item::Helix_Fossil },
		{ "HelixFossil", Item::Helix_Fossil },
		{ "Old Amber", Item::Old_Amber },
		{ "OldAmber", Item::Old_Amber },
		{ "Rare Bone", Item::Rare_Bone },
		{ "RareBone", Item::Rare_Bone },
		{ "Root Fossil", Item::Root_Fossil },
		{ "RootFossil", Item::Root_Fossil },
		{ "Skull Fossil", Item::Skull_Fossil },
		{ "SkullFossil", Item::Skull_Fossil },
		{ "Bicycle", Item::Bicycle },
		{ "Coin Case", Item::Coin_Case },
		{ "CoinCase", Item::Coin_Case },
		{ "Coupon 1", Item::Coupon_1 },
		{ "Coupon1", Item::Coupon_1 },
		{ "Coupon 2", Item::Coupon_2 },
		{ "Coupon2", Item::Coupon_2 },
		{ "Coupon 3", Item::Coupon_3 },
		{ "Coupon3", Item::Coupon_3 },
		{ "Explorer Kit", Item::Explorer_Kit },
		{ "ExplorerKit", Item::Explorer_Kit },
		{ "Fashion Case", Item::Fashion_Case },
		{ "FashionCase", Item::Fashion_Case },
		{ "Galactic Key", Item::Galactic_Key },
		{ "GalacticKey", Item::Galactic_Key },
		{ "Good Rod", Item::Good_Rod },
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
		{ "Loot Sack", Item::Loot_Sack },
		{ "LootSack", Item::Loot_Sack },
		{ "Lunar Wing", Item::Lunar_Wing },
		{ "LunarWing", Item::Lunar_Wing },
		{ "Member Card", Item::Member_Card},
		{ "MemberCard", Item::Member_Card},
		{ "Oak's Letter", Item::Oaks_Letter },
		{ "Oak'sLetter", Item::Oaks_Letter },
		{ "Old Charm", Item::Old_Charm },
		{ "OldCharm", Item::Old_Charm },
		{ "Old Rod", Item::Old_Rod },
		{ "OldRod", Item::Old_Rod },
		{ "Pal Pad", Item::Pal_Pad },
		{ "PalPad", Item::Pal_Pad },
		{ "Parcel", Item::Parcel },
		{ "Poffin Case", Item::Poffin_Case },
		{ "PoffinCase", Item::Poffin_Case },
		{ "Point Card", Item::Point_Card },
		{ "PointCard", Item::Point_Card },
		{ "Poke Radar", Item::Poke_Radar },
		{ "PokeRadar", Item::Poke_Radar },
		{ "Safari Ball", Item::Safari_Ball },
		{ "SafariBall", Item::Safari_Ball },
		{ "Seal Bag", Item::Seal_Bag },
		{ "SealBag", Item::Seal_Bag },
		{ "Seal Case", Item::Seal_Case },
		{ "SealCase", Item::Seal_Case },
		{ "SecretPotion", Item::SecretPotion },
		{ "Sprayduck", Item::Sprayduck },
		{ "Storage Key", Item::Storage_Key },
		{ "StorageKey", Item::Storage_Key },
		{ "Suite Key", Item::Suite_Key },
		{ "SuiteKey", Item::Suite_Key },
		{ "Super Rod", Item::Super_Rod },
		{ "SuperRod", Item::Super_Rod },
		{ "Town Map", Item::Town_Map },
		{ "TownMap", Item::Town_Map },
		{ "VS Seeker", Item::VS_Seeker },
		{ "VSSeeker", Item::VS_Seeker },
		{ "Works Key", Item::Works_Key }
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Item", str);
	}
}

}	// namespace technicalmachine
