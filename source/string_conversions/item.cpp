// Item string conversions
// Copyright (C) 2012 David Stone
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

#include "conversion.hpp"

#include <map>
#include <string>

#include "invalid_string_conversion.hpp"

#include "../item.hpp"

namespace technicalmachine {

template<>
std::string to_string(Item::Items const name) {
	static std::string const item_name [] = {
		"No Item", "Adamant Orb", "Aguav Berry", "Air Mail",
		"Amulet Coin", "Antidote", "Apicot Berry", "Armor Fossil",
		"Aspear Berry", "Awakening", "Babiri Berry", "Belue Berry",
		"Berry Juice", "Bicycle", "Big Mushroom", "Big Pearl",
		"Big Root", "Black Belt", "Black Flute", "Black Sludge",
		"BlackGlasses", "Bloom Mail", "Blue Flute", "Blue Scarf",
		"Blue Shard", "Bluk Berry", "Brick Mail", "BrightPowder",
		"Bubble Mail", "Burn Heal", "Calcium", "Carbos",
		"Charcoal", "Charti Berry", "Cheri Berry", "Cherish Ball",
		"Chesto Berry", "Chilan Berry", "Choice Band", "Choice Scarf",
		"Choice Specs", "Chople Berry", "Claw Fossil", "Cleanse Tag",
		"Coba Berry", "Coin Case", "Colbur Berry", "Cornn Berry",
		"Coupon 1", "Coupon 2", "Coupon 3", "Custap Berry",
		"Damp Mulch", "Damp Rock", "Dawn Stone", "DeepSeaScale",
		"DeepSeaTooth", "Destiny Knot", "Dire Hit", "Dive Ball",
		"Dome Fossil", "Draco Plate", "Dragon Fang", "Dragon Scale",
		"Dread Plate", "Dubious Disc", "Durin Berry", "Dusk Ball",
		"Dusk Stone", "Earth Plate", "Electirizer", "Elixir",
		"Energy Root", "EnergyPowder", "Enigma Berry", "Escape Rope",
		"Ether", "Everstone", "Exp. Share", "Expert Belt",
		"Explorer Kit", "Fashion Case", "Figy Berry", "Fire Stone",
		"Fist Plate", "Flame Mail", "Flame Orb", "Flame Plate",
		"Fluffy Tail", "Focus Band", "Focus Sash", "Fresh Water",
		"Full Heal", "Full Incense", "Full Restore", "Galactic Key",
		"Ganlon Berry", "Good Rod", "Gooey Mulch", "Grass Mail",
		"Great Ball", "Green Scarf", "Green Shard", "Grepa Berry",
		"Grip Claw", "Griseous Orb", "Growth Mulch", "Guard Spec.",
		"Haban Berry", "Hard Stone", "Heal Ball", "Heal Powder",
		"Heart Mail", "Heart Scale", "Heat Rock", "Helix Fossil",
		"HM01", "HM02", "HM03", "HM04",
		"HM05", "HM06", "HM07", "HM08",
		"Hondew Berry", "Honey", "HP Up", "Hyper Potion",
		"Iapapa Berry", "Ice Heal", "Icicle Plate", "Icy Rock",
		"Insect Plate", "Iron", "Iron Ball", "Iron Plate",
		"Jaboca Berry", "Journal", "Kasib Berry", "Kebia Berry",
		"Kelpsy Berry", "King's Rock", "Lagging Tail", "Lansat Berry",
		"Lava Cookie", "Lax Incense", "Leaf Stone", "Leftovers",
		"Lemonade", "Leppa Berry", "Liechi Berry", "Life Orb",
		"Light Ball", "Light Clay", "Loot Sack", "Luck Incense",
		"Lucky Egg", "Lucky Punch", "Lum Berry", "Lunar Wing",
		"Lustrous Orb", "Luxury Ball", "Macho Brace", "Magmarizer",
		"Magnet", "Mago Berry", "Magost Berry", "Master Ball",
		"Max Elixir", "Max Ether", "Max Potion", "Max Repel",
		"Max Revive", "Meadow Plate", "Member Card", "Mental Herb",
		"Metal Coat", "Metal Powder", "Metronome", "Micle Berry",
		"Mind Plate", "Miracle Seed", "MooMoo Milk", "Moon Stone",
		"Mosaic Mail", "Muscle Band", "Mystic Water", "Nanab Berry",
		"Nest Ball", "Net Ball", "NeverMeltIce", "Nomel Berry",
		"Nugget", "Oak's Letter", "Occa Berry", "Odd Incense",
		"Odd Keystone", "Old Amber", "Old Charm", "Old Gateau",
		"Old Rod", "Oran Berry", "Oval Stone", "Pal Pad",
		"Pamtre Berry", "Parcel", "Parlyz Heal", "Passho Berry",
		"Payapa Berry", "Pearl", "Pecha Berry", "Persim Berry",
		"Petaya Berry", "Pinap Berry", "Pink Scarf", "Poffin Case",
		"Point Card", "Poison Barb", "Poke Ball", "Poke Doll",
		"Poke Radar", "Pomeg Berry", "Potion", "Power Anklet",
		"Power Band", "Power Belt", "Power Bracer", "Power Herb",
		"Power Lens", "Power Weight", "PP Max", "PP Up",
		"Premier Ball", "Protector", "Protein", "Pure Incense",
		"Qualot Berry", "Quick Ball", "Quick Claw", "Quick Powder",
		"Rabuta Berry", "Rare Bone", "Rare Candy", "Rawst Berry",
		"Razor Claw", "Razor Fang", "Razz Berry", "Reaper Cloth",
		"Red Flute", "Red Scarf", "Red Shard", "Repeat Ball",
		"Repel", "Revival Herb", "Revive", "Rindo Berry",
		"Rock Incense", "Root Fossil", "Rose Incense", "Rowap Berry",
		"Sacred Ash", "Safari Ball", "Salac Berry", "Scope Lens",
		"Sea Incense", "Seal Bag", "Seal Case", "SecretPotion",
		"Sharp Beak", "Shed Shell", "Shell Bell", "Shiny Stone",
		"Shoal Salt", "Shoal Shell", "Shuca Berry", "Silk Scarf",
		"SilverPowder", "Sitrus Berry", "Skull Fossil", "Sky Plate",
		"Smoke Ball", "Smooth Rock", "Snow Mail", "Soda Pop",
		"Soft Sand", "Soothe Bell", "Soul Dew", "Space Mail",
		"Spell Tag", "Spelon Berry", "Splash Plate", "Spooky Plate",
		"Sprayduck", "Stable Mulch", "Star Piece", "Stardust",
		"Starf Berry", "Steel Mail", "Stick", "Sticky Barb",
		"Stone Plate", "Storage Key", "Suite Key", "Sun Stone",
		"Super Potion", "Super Repel", "Super Rod", "Tamato Berry",
		"Tanga Berry", "Thick Club", "Thunderstone", "Timer Ball",
		"TinyMushroom", "TM01", "TM02", "TM03",
		"TM04", "TM05", "TM06", "TM07",
		"TM08", "TM09", "TM10", "TM11",
		"TM12", "TM13", "TM14", "TM15",
		"TM16", "TM17", "TM18", "TM19",
		"TM20", "TM21", "TM22", "TM23",
		"TM24", "TM25", "TM26", "TM27",
		"TM28", "TM29", "TM30", "TM31",
		"TM32", "TM33", "TM34", "TM35",
		"TM36", "TM37", "TM38", "TM39",
		"TM40", "TM41", "TM42", "TM43",
		"TM44", "TM45", "TM46", "TM47",
		"TM48", "TM49", "TM50", "TM51",
		"TM52", "TM53", "TM54", "TM55",
		"TM56", "TM57", "TM58", "TM59",
		"TM60", "TM61", "TM62", "TM63",
		"TM64", "TM65", "TM66", "TM67",
		"TM68", "TM69", "TM70", "TM71",
		"TM72", "TM73", "TM74", "TM75",
		"TM76", "TM77", "TM78", "TM79",
		"TM80", "TM81", "TM82", "TM83",
		"TM84", "TM85", "TM86", "TM87",
		"TM88", "TM89", "TM90", "TM91",
		"TM92", "Town Map", "Toxic Orb", "Toxic Plate",
		"Tunnel Mail", "TwistedSpoon", "Ultra Ball", "Up-Grade",
		"VS Seeker", "Wacan Berry", "Water Stone", "Watmel Berry",
		"Wave Incense", "Wepear Berry", "White Flute", "White Herb",
		"Wide Lens", "Wiki Berry", "Wise Glasses", "Works Key",
		"X Accuracy", "X Attack", "X Defend", "X Sp. Def",
		"X Special", "X Speed", "Yache Berry", "Yellow Flute",
		"Yellow Scarf", "Yellow Shard", "Zap Plate", "Zinc",
		"Zoom Lens", "END ITEM"
	};
	return item_name [name];
}

template<>
Item::Items from_string(std::string const & str) {
	static std::map <std::string, Item::Items> const converter {
		{ "No Item", Item::NO_ITEM },
		{ "Adamant Orb", Item::ADAMANT_ORB },
		{ "Aguav Berry", Item::AGUAV_BERRY },
		{ "Apicot Berry", Item::APICOT_BERRY },
		{ "Aspear Berry", Item::ASPEAR_BERRY },
		{ "Babiri Berry", Item::BABIRI_BERRY },
		{ "Belue Berry", Item::BELUE_BERRY },
		{ "Berry Juice", Item::BERRY_JUICE },
		{ "Big Root", Item::BIG_ROOT },
		{ "Black Belt", Item::BLACK_BELT },
		{ "Black Sludge", Item::BLACK_SLUDGE },
		{ "BlackGlasses", Item::BLACKGLASSES },
		{ "Bluk Berry", Item::BLUK_BERRY },
		{ "BrightPowder", Item::BRIGHTPOWDER },
		{ "Brightpowder", Item::BRIGHTPOWDER },
		{ "Charcoal", Item::CHARCOAL },
		{ "Charti Berry", Item::CHARTI_BERRY },
		{ "Cheri Berry", Item::CHERI_BERRY },
		{ "Chesto Berry", Item::CHESTO_BERRY },
		{ "Chilan Berry", Item::CHILAN_BERRY },
		{ "Choice Band", Item::CHOICE_BAND },
		{ "Choice Scarf", Item::CHOICE_SCARF },
		{ "Choice Specs", Item::CHOICE_SPECS },
		{ "Chople Berry", Item::CHOPLE_BERRY },
		{ "Coba Berry", Item::COBA_BERRY },
		{ "Colbur Berry", Item::COLBUR_BERRY },
		{ "Cornn Berry", Item::CORNN_BERRY },
		{ "Custap Berry", Item::CUSTAP_BERRY },
		{ "Damp Rock", Item::DAMP_ROCK },
		{ "DeepSeaScale", Item::DEEPSEASCALE },
		{ "Deepseascale", Item::DEEPSEASCALE },
		{ "DeepSeaTooth", Item::DEEPSEATOOTH },
		{ "Deepseatooth", Item::DEEPSEATOOTH },
		{ "Destiny Knot", Item::DESTINY_KNOT },
		{ "Draco Plate", Item::DRACO_PLATE },
		{ "Dragon Fang", Item::DRAGON_FANG },
		{ "Dread Plate", Item::DREAD_PLATE },
		{ "Durin Berry", Item::DURIN_BERRY },
		{ "Earth Plate", Item::EARTH_PLATE },
		{ "Enigma Berry", Item::ENIGMA_BERRY },
		{ "Expert Belt", Item::EXPERT_BELT },
		{ "Figy Berry", Item::FIGY_BERRY },
		{ "Fist Plate", Item::FIST_PLATE },
		{ "Flame Orb", Item::FLAME_ORB },
		{ "Flame Plate", Item::FLAME_PLATE },
		{ "Focus Band", Item::FOCUS_BAND },
		{ "Focus Sash", Item::FOCUS_SASH },
		{ "Ganlon Berry", Item::GANLON_BERRY },
		{ "Grepa Berry", Item::GREPA_BERRY },
		{ "Grip Claw", Item::GRIP_CLAW },
		{ "Griseous Orb", Item::GRISEOUS_ORB },
		{ "Haban Berry", Item::HABAN_BERRY },
		{ "Hard Stone", Item::HARD_STONE },
		{ "Heat Rock", Item::HEAT_ROCK },
		{ "Hondew Berry", Item::HONDEW_BERRY },
		{ "Iapapa Berry", Item::IAPAPA_BERRY },
		{ "Icicle Plate", Item::ICICLE_PLATE },
		{ "Icy Rock", Item::ICY_ROCK },
		{ "Insect Plate", Item::INSECT_PLATE },
		{ "Iron Ball", Item::IRON_BALL },
		{ "Iron Plate", Item::IRON_PLATE },
		{ "Jaboca Berry", Item::JABOCA_BERRY },
		{ "Kasib Berry", Item::KASIB_BERRY },
		{ "Kebia Berry", Item::KEBIA_BERRY },
		{ "Kelpsy Berry", Item::KELPSY_BERRY },
		{ "King's Rock", Item::KINGS_ROCK },
		{ "Razor Fang", Item::RAZOR_FANG },
		{ "Full Incense", Item::FULL_INCENSE },
		{ "Lagging Tail", Item::LAGGING_TAIL },
		{ "Lansat Berry", Item::LANSAT_BERRY },
		{ "Lax Incense", Item::LAX_INCENSE },
		{ "Leftovers", Item::LEFTOVERS },
		{ "Leppa Berry", Item::LEPPA_BERRY },
		{ "Liechi Berry", Item::LIECHI_BERRY },
		{ "Life Orb", Item::LIFE_ORB },
		{ "Light Ball", Item::LIGHT_BALL },
		{ "Light Clay", Item::LIGHT_CLAY },
		{ "Lucky Punch", Item::LUCKY_PUNCH },
		{ "Lum Berry", Item::LUM_BERRY },
		{ "Lustrous Orb", Item::LUSTROUS_ORB },
		{ "Macho Brace", Item::MACHO_BRACE },
		{ "Magnet", Item::MAGNET },
		{ "Mago Berry", Item::MAGO_BERRY },
		{ "Magost Berry", Item::MAGOST_BERRY },
		{ "Air Mail", Item::AIR_MAIL },
		{ "Bloom Mail", Item::BLOOM_MAIL },
		{ "Brick Mail", Item::BRICK_MAIL },
		{ "Bubble Mail", Item::BUBBLE_MAIL },
		{ "Flame Mail", Item::FLAME_MAIL },
		{ "Grass Mail", Item::GRASS_MAIL },
		{ "Heart Mail", Item::HEART_MAIL },
		{ "Mosaic Mail", Item::MOSAIC_MAIL },
		{ "Snow Mail", Item::SNOW_MAIL },
		{ "Space Mail", Item::SPACE_MAIL },
		{ "Steel Mail", Item::STEEL_MAIL },
		{ "Tunnel Mail", Item::TUNNEL_MAIL },
		{ "Meadow Plate", Item::MEADOW_PLATE },
		{ "Mental Herb", Item::MENTAL_HERB },
		{ "Metal Coat", Item::METAL_COAT },
		{ "Metal Powder", Item::METAL_POWDER },
		{ "Metronome", Item::METRONOME },
		{ "Micle Berry", Item::MICLE_BERRY },
		{ "Mind Plate", Item::MIND_PLATE },
		{ "Miracle Seed", Item::MIRACLE_SEED },
		{ "Muscle Band", Item::MUSCLE_BAND },
		{ "Mystic Water", Item::MYSTIC_WATER },
		{ "Nanab Berry", Item::NANAB_BERRY },
		{ "NeverMeltIce", Item::NEVERMELTICE },
		{ "Nomel Berry", Item::NOMEL_BERRY },
		{ "Occa Berry", Item::OCCA_BERRY },
		{ "Odd Incense", Item::ODD_INCENSE },
		{ "Oran Berry", Item::ORAN_BERRY },
		{ "Pamtre Berry", Item::PAMTRE_BERRY },
		{ "Passho Berry", Item::PASSHO_BERRY },
		{ "Payapa Berry", Item::PAYAPA_BERRY },
		{ "Pecha Berry", Item::PECHA_BERRY },
		{ "Persim Berry", Item::PERSIM_BERRY },
		{ "Petaya Berry", Item::PETAYA_BERRY },
		{ "Pinap Berry", Item::PINAP_BERRY },
		{ "Poison Barb", Item::POISON_BARB },
		{ "Pomeg Berry", Item::POMEG_BERRY },
		{ "Power Herb", Item::POWER_HERB },
		{ "Power Anklet", Item::POWER_ANKLET },
		{ "Power Band", Item::POWER_BAND },
		{ "Power Belt", Item::POWER_BELT },
		{ "Power Bracer", Item::POWER_BRACER },
		{ "Power Lens", Item::POWER_LENS },
		{ "Power Weight", Item::POWER_WEIGHT },
		{ "Qualot Berry", Item::QUALOT_BERRY },
		{ "Quick Claw", Item::QUICK_CLAW },
		{ "Quick Powder", Item::QUICK_POWDER },
		{ "Rabuta Berry", Item::RABUTA_BERRY },
		{ "Rawst Berry", Item::RAWST_BERRY },
		{ "Razor Claw", Item::RAZOR_CLAW },
		{ "Razz Berry", Item::RAZZ_BERRY },
		{ "Rindo Berry", Item::RINDO_BERRY },
		{ "Rock Incense", Item::ROCK_INCENSE },
		{ "Rose Incense", Item::ROSE_INCENSE },
		{ "Rowap Berry", Item::ROWAP_BERRY },
		{ "Salac Berry", Item::SALAC_BERRY },
		{ "Scope Lens", Item::SCOPE_LENS },
		{ "Sharp Beak", Item::SHARP_BEAK },
		{ "Shed Shell", Item::SHED_SHELL },
		{ "Shell Bell", Item::SHELL_BELL },
		{ "Shuca Berry", Item::SHUCA_BERRY },
		{ "Silk Scarf", Item::SILK_SCARF },
		{ "SilverPowder", Item::SILVERPOWDER },
		{ "Silverpowder", Item::SILVERPOWDER },
		{ "Sitrus Berry", Item::SITRUS_BERRY },
		{ "Sky Plate", Item::SKY_PLATE },
		{ "Smooth Rock", Item::SMOOTH_ROCK },
		{ "Soft Sand", Item::SOFT_SAND },
		{ "Soul Dew", Item::SOUL_DEW },
		{ "Spell Tag", Item::SPELL_TAG },
		{ "Spelon Berry", Item::SPELON_BERRY },
		{ "Splash Plate", Item::SPLASH_PLATE },
		{ "Spooky Plate", Item::SPOOKY_PLATE },
		{ "Starf Berry", Item::STARF_BERRY },
		{ "Stick", Item::STICK },
		{ "Sticky Barb", Item::STICKY_BARB },
		{ "Stone Plate", Item::STONE_PLATE },
		{ "Tamato Berry", Item::TAMATO_BERRY },
		{ "Tanga Berry", Item::TANGA_BERRY },
		{ "Thick Club", Item::THICK_CLUB },
		{ "Toxic Orb", Item::TOXIC_ORB },
		{ "Toxic Plate", Item::TOXIC_PLATE },
		{ "TwistedSpoon", Item::TWISTEDSPOON },
		{ "Twisted Spoon", Item::TWISTEDSPOON },
		{ "Wacan Berry", Item::WACAN_BERRY },
		{ "Watmel Berry", Item::WATMEL_BERRY },
		{ "Sea Incense", Item::SEA_INCENSE },
		{ "Wave Incense", Item::WAVE_INCENSE },
		{ "Wepear Berry", Item::WEPEAR_BERRY },
		{ "White Herb", Item::WHITE_HERB },
		{ "Wide Lens", Item::WIDE_LENS },
		{ "Wiki Berry", Item::WIKI_BERRY },
		{ "Wise Glasses", Item::WISE_GLASSES },
		{ "Yache Berry", Item::YACHE_BERRY },
		{ "Zap Plate", Item::ZAP_PLATE },
		{ "Zoom Lens", Item::ZOOM_LENS },
		{ "Cherish Ball", Item::CHERISH_BALL },
		{ "Dive Ball", Item::DIVE_BALL },
		{ "Dusk Ball", Item::DUSK_BALL },
		{ "Great Ball", Item::GREAT_BALL },
		{ "Heal Ball", Item::HEAL_BALL },
		{ "Luxury Ball", Item::LUXURY_BALL },
		{ "Master Ball", Item::MASTER_BALL },
		{ "Nest Ball", Item::NEST_BALL },
		{ "Net Ball", Item::NET_BALL },
		{ "Poke Ball", Item::POKE_BALL },
		{ "Premier Ball", Item::PREMIER_BALL },
		{ "Quick Ball", Item::QUICK_BALL },
		{ "Repeat Ball", Item::REPEAT_BALL },
		{ "Timer Ball", Item::TIMER_BALL },
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
		{ "Ultra Ball", Item::ULTRA_BALL },
		{ "Blue Scarf", Item::BLUE_SCARF },
		{ "Green Scarf", Item::GREEN_SCARF },
		{ "Luck Incense", Item::LUCK_INCENSE },
		{ "Pink Scarf", Item::PINK_SCARF },
		{ "Pure Incense", Item::PURE_INCENSE },
		{ "Reaper Cloth", Item::REAPER_CLOTH },
		{ "Red Scarf", Item::RED_SCARF },
		{ "Soothe Bell", Item::SOOTHE_BELL },
		{ "Yellow Scarf", Item::YELLOW_SCARF },
		{ "Amulet Coin", Item::AMULET_COIN },
		{ "Antidote", Item::ANTIDOTE },
		{ "Awakening", Item::AWAKENING },
		{ "Big Mushroom", Item::BIG_MUSHROOM },
		{ "Big Pearl", Item::BIG_PEARL },
		{ "Black Flute", Item::BLACK_FLUTE },
		{ "Blue Flute", Item::BLUE_FLUTE },
		{ "Blue Shard", Item::BLUE_SHARD },
		{ "Burn Heal", Item::BURN_HEAL },
		{ "Calcium", Item::CALCIUM },
		{ "Carbos", Item::CARBOS },
		{ "Cleanse Tag", Item::CLEANSE_TAG },
		{ "Damp Mulch", Item::DAMP_MULCH },
		{ "Dire Hit", Item::DIRE_HIT },
		{ "Dragon Scale", Item::DRAGON_SCALE },
		{ "Elixir", Item::ELIXIR },
		{ "Energy Root", Item::ENERGY_ROOT },
		{ "EnergyPowder", Item::ENERGYPOWDER },
		{ "Escape Rope", Item::ESCAPE_ROPE },
		{ "Ether", Item::ETHER },
		{ "Everstone", Item::EVERSTONE },
		{ "Exp. Share", Item::EXP_SHARE },
		{ "Fire Stone", Item::FIRE_STONE },
		{ "Fluffy Tail", Item::FLUFFY_TAIL },
		{ "Fresh Water", Item::FRESH_WATER },
		{ "Full Heal", Item::FULL_HEAL },
		{ "Full Restore", Item::FULL_RESTORE },
		{ "Gooey Mulch", Item::GOOEY_MULCH },
		{ "Green Shard", Item::GREEN_SHARD },
		{ "Growth Mulch", Item::GROWTH_MULCH },
		{ "Guard Spec.", Item::GUARD_SPEC },
		{ "Heal Powder", Item::HEAL_POWDER },
		{ "Heart Scale", Item::HEART_SCALE },
		{ "Honey", Item::HONEY },
		{ "HP Up", Item::HP_UP },
		{ "Hyper Potion", Item::HYPER_POTION },
		{ "Ice Heal", Item::ICE_HEAL },
		{ "Iron", Item::IRON },
		{ "Lava Cookie", Item::LAVA_COOKIE },
		{ "Leaf Stone", Item::LEAF_STONE },
		{ "Lemonade", Item::LEMONADE },
		{ "Lucky Egg", Item::LUCKY_EGG },
		{ "Max Elixir", Item::MAX_ELIXIR },
		{ "Max Ether", Item::MAX_ETHER },
		{ "Max Potion", Item::MAX_POTION },
		{ "Max Repel", Item::MAX_REPEL },
		{ "Max Revive", Item::MAX_REVIVE },
		{ "MooMoo Milk", Item::MOOMOO_MILK },
		{ "Moon Stone", Item::MOON_STONE },
		{ "Nugget", Item::NUGGET },
		{ "Old Gateau", Item::OLD_GATEAU },
		{ "Parlyz Heal", Item::PARLYZ_HEAL },
		{ "Pearl", Item::PEARL },
		{ "Poke Doll", Item::POKE_DOLL },
		{ "Potion", Item::POTION },
		{ "PP Max", Item::PP_MAX },
		{ "PP Up", Item::PP_UP },
		{ "Protein", Item::PROTEIN },
		{ "Rare Candy", Item::RARE_CANDY },
		{ "Red Flute", Item::RED_FLUTE },
		{ "Red Shard", Item::RED_SHARD },
		{ "Repel", Item::REPEL },
		{ "Revival Herb", Item::REVIVAL_HERB },
		{ "Revive", Item::REVIVE },
		{ "Sacred Ash", Item::SACRED_ASH },
		{ "Shoal Salt", Item::SHOAL_SALT },
		{ "Shoal Shell", Item::SHOAL_SHELL },
		{ "Smoke Ball", Item::SMOKE_BALL },
		{ "Soda Pop", Item::SODA_POP },
		{ "Stable Mulch", Item::STABLE_MULCH },
		{ "Star Piece", Item::STAR_PIECE },
		{ "Stardust", Item::STARDUST },
		{ "Sun Stone", Item::SUN_STONE },
		{ "Super Potion", Item::SUPER_POTION },
		{ "Super Repel", Item::SUPER_REPEL },
		{ "Thunderstone", Item::THUNDERSTONE },
		{ "TinyMushroom", Item::TINYMUSHROOM },
		{ "Up-Grade", Item::UP_GRADE },
		{ "Water Stone", Item::WATER_STONE },
		{ "White Flute", Item::WHITE_FLUTE },
		{ "X Accuracy", Item::X_ACCURACY },
		{ "X Attack", Item::X_ATTACK },
		{ "X Defend", Item::X_DEFEND },
		{ "X Sp. Def", Item::X_SP_DEF },
		{ "X Special", Item::X_SPECIAL },
		{ "X Speed", Item::X_SPEED },
		{ "Yellow Flute", Item::YELLOW_FLUTE },
		{ "Yellow Shard", Item::YELLOW_SHARD },
		{ "Zinc", Item::ZINC },
		{ "Dubious Disc", Item::DUBIOUS_DISC },
		{ "Dawn Stone", Item::DAWN_STONE },
		{ "Dusk Stone", Item::DUSK_STONE },
		{ "Electirizer", Item::ELECTIRIZER },
		{ "Magmarizer", Item::MAGMARIZER },
		{ "Odd Keystone", Item::ODD_KEYSTONE },
		{ "Oval Stone", Item::OVAL_STONE },
		{ "Protector", Item::PROTECTOR },
		{ "Shiny Stone", Item::SHINY_STONE },
		{ "Armor Fossil", Item::ARMOR_FOSSIL },
		{ "Claw Fossil", Item::CLAW_FOSSIL },
		{ "Dome Fossil", Item::DOME_FOSSIL },
		{ "Helix Fossil", Item::HELIX_FOSSIL },
		{ "Old Amber", Item::OLD_AMBER },
		{ "Rare Bone", Item::RARE_BONE },
		{ "Root Fossil", Item::ROOT_FOSSIL },
		{ "Skull Fossil", Item::SKULL_FOSSIL },
		{ "Bicycle", Item::BICYCLE },
		{ "Coin Case", Item::COIN_CASE },
		{ "Coupon 1", Item::COUPON_1 },
		{ "Coupon 2", Item::COUPON_2 },
		{ "Coupon 3", Item::COUPON_3 },
		{ "Explorer Kit", Item::EXPLORER_KIT },
		{ "Fashion Case", Item::FASHION_CASE },
		{ "Galactic Key", Item::GALACTIC_KEY },
		{ "Good Rod", Item::GOOD_ROD },
		{ "HM01", Item::HM01 },
		{ "HM02", Item::HM02 },
		{ "HM03", Item::HM03 },
		{ "HM04", Item::HM04 },
		{ "HM05", Item::HM05 },
		{ "HM06", Item::HM06 },
		{ "HM07", Item::HM07 },
		{ "HM08", Item::HM08 },
		{ "Journal", Item::JOURNAL },
		{ "Loot Sack", Item::LOOT_SACK },
		{ "Lunar Wing", Item::LUNAR_WING },
		{ "Member Card", Item::MEMBER_CARD},
		{ "Oak's Letter", Item::OAKS_LETTER },
		{ "Old Charm", Item::OLD_CHARM },
		{ "Old Rod", Item::OLD_ROD },
		{ "Pal Pad", Item::PAL_PAD },
		{ "Parcel", Item::PARCEL },
		{ "Poffin Case", Item::POFFIN_CASE },
		{ "Point Card", Item::POINT_CARD },
		{ "Poke Radar", Item::POKE_RADAR },
		{ "Safari Ball", Item::SAFARI_BALL },
		{ "Seal Bag", Item::SEAL_BAG },
		{ "Seal Case", Item::SEAL_CASE },
		{ "SecretPotion", Item::SECRETPOTION },
		{ "Sprayduck", Item::SPRAYDUCK },
		{ "Storage Key", Item::STORAGE_KEY },
		{ "Suite Key", Item::SUITE_KEY },
		{ "Super Rod", Item::SUPER_ROD },
		{ "Town Map", Item::TOWN_MAP },
		{ "VS Seeker", Item::VS_SEEKER },
		{ "Works Key", Item::WORKS_KEY }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Item", str);
}

}	// namespace technicalmachine
