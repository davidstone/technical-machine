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
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/item.hpp>

#include <containers/array/array.hpp>
#include <containers/flat_map.hpp>

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
	using Storage = containers::array<containers::map_value_type<std::string_view, Item>, 437>;
	constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "adamantorb", Item::Adamant_Orb },
			{ "aguavberry", Item::Aguav_Berry },
			{ "airmail", Item::Air_Mail },
			{ "amuletcoin", Item::Amulet_Coin },
			{ "antidote", Item::Antidote },
			{ "apicotberry", Item::Apicot_Berry },
			{ "armorfossil", Item::Armor_Fossil },
			{ "aspearberry", Item::Aspear_Berry },
			{ "awakening", Item::Awakening },
			{ "babiriberry", Item::Babiri_Berry },
			{ "belueberry", Item::Belue_Berry },
			{ "berryjuice", Item::Berry_Juice },
			{ "bicycle", Item::Bicycle },
			{ "bigmushroom", Item::Big_Mushroom },
			{ "bigpearl", Item::Big_Pearl },
			{ "bigroot", Item::Big_Root },
			{ "blackbelt", Item::Black_Belt },
			{ "blackflute", Item::Black_Flute },
			{ "blackglasses", Item::BlackGlasses },
			{ "blacksludge", Item::Black_Sludge },
			{ "bloommail", Item::Bloom_Mail },
			{ "blueflute", Item::Blue_Flute },
			{ "bluescarf", Item::Blue_Scarf },
			{ "blueshard", Item::Blue_Shard },
			{ "blukberry", Item::Bluk_Berry },
			{ "brickmail", Item::Brick_Mail },
			{ "brightpowder", Item::BrightPowder },
			{ "bubblemail", Item::Bubble_Mail },
			{ "burnheal", Item::Burn_Heal },
			{ "calcium", Item::Calcium },
			{ "carbos", Item::Carbos },
			{ "charcoal", Item::Charcoal },
			{ "chartiberry", Item::Charti_Berry },
			{ "cheriberry", Item::Cheri_Berry },
			{ "cherishball", Item::Cherish_Ball },
			{ "chestoberry", Item::Chesto_Berry },
			{ "chilanberry", Item::Chilan_Berry },
			{ "choiceband", Item::Choice_Band },
			{ "choicescarf", Item::Choice_Scarf },
			{ "choicespecs", Item::Choice_Specs },
			{ "chopleberry", Item::Chople_Berry },
			{ "clawfossil", Item::Claw_Fossil },
			{ "cleansetag", Item::Cleanse_Tag },
			{ "cobaberry", Item::Coba_Berry },
			{ "coincase", Item::Coin_Case },
			{ "colburberry", Item::Colbur_Berry },
			{ "cornnberry", Item::Cornn_Berry },
			{ "coupon1", Item::Coupon_1 },
			{ "coupon2", Item::Coupon_2 },
			{ "coupon3", Item::Coupon_3 },
			{ "custapberry", Item::Custap_Berry },
			{ "dampmulch", Item::Damp_Mulch },
			{ "damprock", Item::Damp_Rock },
			{ "dawnstone", Item::Dawn_Stone },
			{ "deepseascale", Item::DeepSeaScale },
			{ "deepseatooth", Item::DeepSeaTooth },
			{ "destinyknot", Item::Destiny_Knot },
			{ "direhit", Item::Dire_Hit },
			{ "diveball", Item::Dive_Ball },
			{ "domefossil", Item::Dome_Fossil },
			{ "dracoplate", Item::Draco_Plate },
			{ "dragonfang", Item::Dragon_Fang },
			{ "dragonscale", Item::Dragon_Scale },
			{ "dreadplate", Item::Dread_Plate },
			{ "dubiousdisc", Item::Dubious_Disc },
			{ "durinberry", Item::Durin_Berry },
			{ "duskball", Item::Dusk_Ball },
			{ "duskstone", Item::Dusk_Stone },
			{ "earthplate", Item::Earth_Plate },
			{ "electirizer", Item::Electirizer },
			{ "elixir", Item::Elixir },
			{ "energypowder", Item::EnergyPowder },
			{ "energyroot", Item::Energy_Root },
			{ "enigmaberry", Item::Enigma_Berry },
			{ "escaperope", Item::Escape_Rope },
			{ "ether", Item::Ether },
			{ "everstone", Item::Everstone },
			{ "expertbelt", Item::Expert_Belt },
			{ "explorerkit", Item::Explorer_Kit },
			{ "expshare", Item::Exp_Share },
			{ "fashioncase", Item::Fashion_Case },
			{ "figyberry", Item::Figy_Berry },
			{ "firestone", Item::Fire_Stone },
			{ "fistplate", Item::Fist_Plate },
			{ "flamemail", Item::Flame_Mail },
			{ "flameorb", Item::Flame_Orb },
			{ "flameplate", Item::Flame_Plate },
			{ "fluffytail", Item::Fluffy_Tail },
			{ "focusband", Item::Focus_Band },
			{ "focussash", Item::Focus_Sash },
			{ "freshwater", Item::Fresh_Water },
			{ "fullheal", Item::Full_Heal },
			{ "fullincense", Item::Full_Incense },
			{ "fullrestore", Item::Full_Restore },
			{ "galactickey", Item::Galactic_Key },
			{ "ganlonberry", Item::Ganlon_Berry },
			{ "goodrod", Item::Good_Rod },
			{ "gooeymulch", Item::Gooey_Mulch },
			{ "grassmail", Item::Grass_Mail },
			{ "greatball", Item::Great_Ball },
			{ "greenscarf", Item::Green_Scarf },
			{ "greenshard", Item::Green_Shard },
			{ "grepaberry", Item::Grepa_Berry },
			{ "gripclaw", Item::Grip_Claw },
			{ "griseousorb", Item::Griseous_Orb },
			{ "growthmulch", Item::Growth_Mulch },
			{ "guardspec", Item::Guard_Spec },
			{ "habanberry", Item::Haban_Berry },
			{ "hardstone", Item::Hard_Stone },
			{ "healball", Item::Heal_Ball },
			{ "healpowder", Item::Heal_Powder },
			{ "heartmail", Item::Heart_Mail },
			{ "heartscale", Item::Heart_Scale },
			{ "heatrock", Item::Heat_Rock },
			{ "helixfossil", Item::Helix_Fossil },
			{ "hm01", Item::HM01 },
			{ "hm02", Item::HM02 },
			{ "hm03", Item::HM03 },
			{ "hm04", Item::HM04 },
			{ "hm05", Item::HM05 },
			{ "hm06", Item::HM06 },
			{ "hm07", Item::HM07 },
			{ "hm08", Item::HM08 },
			{ "hondewberry", Item::Hondew_Berry },
			{ "honey", Item::Honey },
			{ "hpup", Item::HP_Up },
			{ "hyperpotion", Item::Hyper_Potion },
			{ "iapapaberry", Item::Iapapa_Berry },
			{ "iceheal", Item::Ice_Heal },
			{ "icicleplate", Item::Icicle_Plate },
			{ "icyrock", Item::Icy_Rock },
			{ "insectplate", Item::Insect_Plate },
			{ "iron", Item::Iron },
			{ "ironball", Item::Iron_Ball },
			{ "ironplate", Item::Iron_Plate },
			{ "jabocaberry", Item::Jaboca_Berry },
			{ "journal", Item::Journal },
			{ "kasibberry", Item::Kasib_Berry },
			{ "kebiaberry", Item::Kebia_Berry },
			{ "kelpsyberry", Item::Kelpsy_Berry },
			{ "kingsrock", Item::Kings_Rock },
			{ "laggingtail", Item::Lagging_Tail },
			{ "lansatberry", Item::Lansat_Berry },
			{ "lavacookie", Item::Lava_Cookie },
			{ "laxincense", Item::Lax_Incense },
			{ "leafstone", Item::Leaf_Stone },
			{ "leftovers", Item::Leftovers },
			{ "lemonade", Item::Lemonade },
			{ "leppaberry", Item::Leppa_Berry },
			{ "liechiberry", Item::Liechi_Berry },
			{ "lifeorb", Item::Life_Orb },
			{ "lightball", Item::Light_Ball },
			{ "lightclay", Item::Light_Clay },
			{ "lootsack", Item::Loot_Sack },
			{ "luckincense", Item::Luck_Incense },
			{ "luckyegg", Item::Lucky_Egg },
			{ "luckypunch", Item::Lucky_Punch },
			{ "lumberry", Item::Lum_Berry },
			{ "lunarwing", Item::Lunar_Wing },
			{ "lustrousorb", Item::Lustrous_Orb },
			{ "luxuryball", Item::Luxury_Ball },
			{ "machobrace", Item::Macho_Brace },
			{ "magmarizer", Item::Magmarizer },
			{ "magnet", Item::Magnet },
			{ "magoberry", Item::Mago_Berry },
			{ "magostberry", Item::Magost_Berry },
			{ "masterball", Item::Master_Ball },
			{ "maxelixir", Item::Max_Elixir },
			{ "maxether", Item::Max_Ether },
			{ "maxpotion", Item::Max_Potion },
			{ "maxrepel", Item::Max_Repel },
			{ "maxrevive", Item::Max_Revive },
			{ "meadowplate", Item::Meadow_Plate },
			{ "membercard", Item::Member_Card},
			{ "mentalherb", Item::Mental_Herb },
			{ "metalcoat", Item::Metal_Coat },
			{ "metalpowder", Item::Metal_Powder },
			{ "metronome", Item::Metronome },
			{ "micleberry", Item::Micle_Berry },
			{ "mindplate", Item::Mind_Plate },
			{ "miracleseed", Item::Miracle_Seed },
			{ "moomoomilk", Item::MooMoo_Milk },
			{ "moonstone", Item::Moon_Stone },
			{ "mosaicmail", Item::Mosaic_Mail },
			{ "muscleband", Item::Muscle_Band },
			{ "mysticwater", Item::Mystic_Water },
			{ "nanabberry", Item::Nanab_Berry },
			{ "nestball", Item::Nest_Ball },
			{ "netball", Item::Net_Ball },
			{ "nevermeltice", Item::NeverMeltIce },
			{ "noitem", Item::No_Item },
			{ "nomelberry", Item::Nomel_Berry },
			{ "nugget", Item::Nugget },
			{ "oaksletter", Item::Oaks_Letter },
			{ "occaberry", Item::Occa_Berry },
			{ "oddincense", Item::Odd_Incense },
			{ "oddkeystone", Item::Odd_Keystone },
			{ "oldamber", Item::Old_Amber },
			{ "oldcharm", Item::Old_Charm },
			{ "oldgateau", Item::Old_Gateau },
			{ "oldrod", Item::Old_Rod },
			{ "oranberry", Item::Oran_Berry },
			{ "ovalstone", Item::Oval_Stone },
			{ "palpad", Item::Pal_Pad },
			{ "pamtreberry", Item::Pamtre_Berry },
			{ "parcel", Item::Parcel },
			{ "parlyzheal", Item::Parlyz_Heal },
			{ "passhoberry", Item::Passho_Berry },
			{ "payapaberry", Item::Payapa_Berry },
			{ "pearl", Item::Pearl },
			{ "pechaberry", Item::Pecha_Berry },
			{ "persimberry", Item::Persim_Berry },
			{ "petayaberry", Item::Petaya_Berry },
			{ "pinapberry", Item::Pinap_Berry },
			{ "pinkscarf", Item::Pink_Scarf },
			{ "poffincase", Item::Poffin_Case },
			{ "pointcard", Item::Point_Card },
			{ "poisonbarb", Item::Poison_Barb },
			{ "pokeball", Item::Poke_Ball },
			{ "pokedoll", Item::Poke_Doll },
			{ "pokeradar", Item::Poke_Radar },
			{ "pomegberry", Item::Pomeg_Berry },
			{ "potion", Item::Potion },
			{ "poweranklet", Item::Power_Anklet },
			{ "powerband", Item::Power_Band },
			{ "powerbelt", Item::Power_Belt },
			{ "powerbracer", Item::Power_Bracer },
			{ "powerherb", Item::Power_Herb },
			{ "powerlens", Item::Power_Lens },
			{ "powerweight", Item::Power_Weight },
			{ "ppmax", Item::PP_Max },
			{ "ppup", Item::PP_Up },
			{ "premierball", Item::Premier_Ball },
			{ "protector", Item::Protector },
			{ "protein", Item::Protein },
			{ "pureincense", Item::Pure_Incense },
			{ "qualotberry", Item::Qualot_Berry },
			{ "quickball", Item::Quick_Ball },
			{ "quickclaw", Item::Quick_Claw },
			{ "quickpowder", Item::Quick_Powder },
			{ "rabutaberry", Item::Rabuta_Berry },
			{ "rarebone", Item::Rare_Bone },
			{ "rarecandy", Item::Rare_Candy },
			{ "rawstberry", Item::Rawst_Berry },
			{ "razorclaw", Item::Razor_Claw },
			{ "razorfang", Item::Razor_Fang },
			{ "razzberry", Item::Razz_Berry },
			{ "reapercloth", Item::Reaper_Cloth },
			{ "redflute", Item::Red_Flute },
			{ "redscarf", Item::Red_Scarf },
			{ "redshard", Item::Red_Shard },
			{ "repeatball", Item::Repeat_Ball },
			{ "repel", Item::Repel },
			{ "revivalherb", Item::Revival_Herb },
			{ "revive", Item::Revive },
			{ "rindoberry", Item::Rindo_Berry },
			{ "rockincense", Item::Rock_Incense },
			{ "rootfossil", Item::Root_Fossil },
			{ "roseincense", Item::Rose_Incense },
			{ "rowapberry", Item::Rowap_Berry },
			{ "sacredash", Item::Sacred_Ash },
			{ "safariball", Item::Safari_Ball },
			{ "salacberry", Item::Salac_Berry },
			{ "scopelens", Item::Scope_Lens },
			{ "seaincense", Item::Sea_Incense },
			{ "sealbag", Item::Seal_Bag },
			{ "sealcase", Item::Seal_Case },
			{ "secretpotion", Item::SecretPotion },
			{ "sharpbeak", Item::Sharp_Beak },
			{ "shedshell", Item::Shed_Shell },
			{ "shellbell", Item::Shell_Bell },
			{ "shinystone", Item::Shiny_Stone },
			{ "shoalsalt", Item::Shoal_Salt },
			{ "shoalshell", Item::Shoal_Shell },
			{ "shucaberry", Item::Shuca_Berry },
			{ "silkscarf", Item::Silk_Scarf },
			{ "silverpowder", Item::SilverPowder },
			{ "sitrusberry", Item::Sitrus_Berry },
			{ "skullfossil", Item::Skull_Fossil },
			{ "skyplate", Item::Sky_Plate },
			{ "smokeball", Item::Smoke_Ball },
			{ "smoothrock", Item::Smooth_Rock },
			{ "snowmail", Item::Snow_Mail },
			{ "sodapop", Item::Soda_Pop },
			{ "softsand", Item::Soft_Sand },
			{ "soothebell", Item::Soothe_Bell },
			{ "souldew", Item::Soul_Dew },
			{ "spacemail", Item::Space_Mail },
			{ "spelltag", Item::Spell_Tag },
			{ "spelonberry", Item::Spelon_Berry },
			{ "splashplate", Item::Splash_Plate },
			{ "spookyplate", Item::Spooky_Plate },
			{ "sprayduck", Item::Sprayduck },
			{ "stablemulch", Item::Stable_Mulch },
			{ "stardust", Item::Stardust },
			{ "starfberry", Item::Starf_Berry },
			{ "starpiece", Item::Star_Piece },
			{ "steelmail", Item::Steel_Mail },
			{ "stick", Item::Stick },
			{ "stickybarb", Item::Sticky_Barb },
			{ "stoneplate", Item::Stone_Plate },
			{ "storagekey", Item::Storage_Key },
			{ "suitekey", Item::Suite_Key },
			{ "sunstone", Item::Sun_Stone },
			{ "superpotion", Item::Super_Potion },
			{ "superrepel", Item::Super_Repel },
			{ "superrod", Item::Super_Rod },
			{ "tamatoberry", Item::Tamato_Berry },
			{ "tangaberry", Item::Tanga_Berry },
			{ "thickclub", Item::Thick_Club },
			{ "thunderstone", Item::Thunderstone },
			{ "timerball", Item::Timer_Ball },
			{ "tinymushroom", Item::TinyMushroom },
			{ "tm01", Item::TM01 },
			{ "tm02", Item::TM02 },
			{ "tm03", Item::TM03 },
			{ "tm04", Item::TM04 },
			{ "tm05", Item::TM05 },
			{ "tm06", Item::TM06 },
			{ "tm07", Item::TM07 },
			{ "tm08", Item::TM08 },
			{ "tm09", Item::TM09 },
			{ "tm10", Item::TM10 },
			{ "tm11", Item::TM11 },
			{ "tm12", Item::TM12 },
			{ "tm13", Item::TM13 },
			{ "tm14", Item::TM14 },
			{ "tm15", Item::TM15 },
			{ "tm16", Item::TM16 },
			{ "tm17", Item::TM17 },
			{ "tm18", Item::TM18 },
			{ "tm19", Item::TM19 },
			{ "tm20", Item::TM20 },
			{ "tm21", Item::TM21 },
			{ "tm22", Item::TM22 },
			{ "tm23", Item::TM23 },
			{ "tm24", Item::TM24 },
			{ "tm25", Item::TM25 },
			{ "tm26", Item::TM26 },
			{ "tm27", Item::TM27 },
			{ "tm28", Item::TM28 },
			{ "tm29", Item::TM29 },
			{ "tm30", Item::TM30 },
			{ "tm31", Item::TM31 },
			{ "tm32", Item::TM32 },
			{ "tm33", Item::TM33 },
			{ "tm34", Item::TM34 },
			{ "tm35", Item::TM35 },
			{ "tm36", Item::TM36 },
			{ "tm37", Item::TM37 },
			{ "tm38", Item::TM38 },
			{ "tm39", Item::TM39 },
			{ "tm40", Item::TM40 },
			{ "tm41", Item::TM41 },
			{ "tm42", Item::TM42 },
			{ "tm43", Item::TM43 },
			{ "tm44", Item::TM44 },
			{ "tm45", Item::TM45 },
			{ "tm46", Item::TM46 },
			{ "tm47", Item::TM47 },
			{ "tm48", Item::TM48 },
			{ "tm49", Item::TM49 },
			{ "tm50", Item::TM50 },
			{ "tm51", Item::TM51 },
			{ "tm52", Item::TM52 },
			{ "tm53", Item::TM53 },
			{ "tm54", Item::TM54 },
			{ "tm55", Item::TM55 },
			{ "tm56", Item::TM56 },
			{ "tm57", Item::TM57 },
			{ "tm58", Item::TM58 },
			{ "tm59", Item::TM59 },
			{ "tm60", Item::TM60 },
			{ "tm61", Item::TM61 },
			{ "tm62", Item::TM62 },
			{ "tm63", Item::TM63 },
			{ "tm64", Item::TM64 },
			{ "tm65", Item::TM65 },
			{ "tm66", Item::TM66 },
			{ "tm67", Item::TM67 },
			{ "tm68", Item::TM68 },
			{ "tm69", Item::TM69 },
			{ "tm70", Item::TM70 },
			{ "tm71", Item::TM71 },
			{ "tm72", Item::TM72 },
			{ "tm73", Item::TM73 },
			{ "tm74", Item::TM74 },
			{ "tm75", Item::TM75 },
			{ "tm76", Item::TM76 },
			{ "tm77", Item::TM77 },
			{ "tm78", Item::TM78 },
			{ "tm79", Item::TM79 },
			{ "tm80", Item::TM80 },
			{ "tm81", Item::TM81 },
			{ "tm82", Item::TM82 },
			{ "tm83", Item::TM83 },
			{ "tm84", Item::TM84 },
			{ "tm85", Item::TM85 },
			{ "tm86", Item::TM86 },
			{ "tm87", Item::TM87 },
			{ "tm88", Item::TM88 },
			{ "tm89", Item::TM89 },
			{ "tm90", Item::TM90 },
			{ "tm91", Item::TM91 },
			{ "tm92", Item::TM92 },
			{ "townmap", Item::Town_Map },
			{ "toxicorb", Item::Toxic_Orb },
			{ "toxicplate", Item::Toxic_Plate },
			{ "tunnelmail", Item::Tunnel_Mail },
			{ "twistedspoon", Item::TwistedSpoon },
			{ "ultraball", Item::Ultra_Ball },
			{ "upgrade", Item::Up_Grade },
			{ "vsseeker", Item::VS_Seeker },
			{ "wacanberry", Item::Wacan_Berry },
			{ "waterstone", Item::Water_Stone },
			{ "watmelberry", Item::Watmel_Berry },
			{ "waveincense", Item::Wave_Incense },
			{ "wepearberry", Item::Wepear_Berry },
			{ "whiteflute", Item::White_Flute },
			{ "whiteherb", Item::White_Herb },
			{ "widelens", Item::Wide_Lens },
			{ "wikiberry", Item::Wiki_Berry },
			{ "wiseglasses", Item::Wise_Glasses },
			{ "workskey", Item::Works_Key },
			{ "xaccuracy", Item::X_Accuracy },
			{ "xattack", Item::X_Attack },
			{ "xdefend", Item::X_Defend },
			{ "xspdef", Item::X_Sp_Def },
			{ "xspecial", Item::X_Special },
			{ "xspeed", Item::X_Speed },
			{ "yacheberry", Item::Yache_Berry },
			{ "yellowflute", Item::Yellow_Flute },
			{ "yellowscarf", Item::Yellow_Scarf },
			{ "yellowshard", Item::Yellow_Shard },
			{ "zapplate", Item::Zap_Plate },
			{ "zinc", Item::Zinc },
			{ "zoomlens", Item::Zoom_Lens },
		}}
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<12>(str);
	auto const it = converter.find(converted);
	if (it != end(converter)) {
		return it->mapped();
	} else {
		throw InvalidFromStringConversion("Item", str);
	}
}

}	// namespace technicalmachine
