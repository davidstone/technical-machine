// Item header
// Copyright (C) 2011 David Stone
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

#include <map>
#include <string>

namespace technicalmachine {

Item::Item ():
	name (END_ITEM) {
}

Item::Item (Items item):
	name (item) {
}

bool Item::is_set () const {
	return name != END_ITEM;
}

bool Item::is_choice_item () const {
	switch (name) {
		case CHOICE_BAND:
		case CHOICE_SCARF:
		case CHOICE_SPECS:
			return true;
		default:
			return false;
	}
}

int Item::get_berry_power () const {
	switch (name) {
		case AGUAV_BERRY:
		case ASPEAR_BERRY:
		case BABIRI_BERRY:
		case CHARTI_BERRY:
		case CHERI_BERRY:
		case CHESTO_BERRY:
		case CHILAN_BERRY:
		case CHOPLE_BERRY:
		case COBA_BERRY:
		case COLBUR_BERRY:
		case FIGY_BERRY:
		case HABAN_BERRY:
		case IAPAPA_BERRY:
		case KASIB_BERRY:
		case KEBIA_BERRY:
		case LEPPA_BERRY:
		case LUM_BERRY:
		case MAGO_BERRY:
		case OCCA_BERRY:
		case ORAN_BERRY:
		case PASSHO_BERRY:
		case PAYAPA_BERRY:
		case PECHA_BERRY:
		case PERSIM_BERRY:
		case RAWST_BERRY:
		case RAZZ_BERRY:
		case RINDO_BERRY:
		case SHUCA_BERRY:
		case SITRUS_BERRY:
		case TANGA_BERRY:
		case WACAN_BERRY:
		case WIKI_BERRY:
		case YACHE_BERRY:
			return 60;
		case BLUK_BERRY:
		case CORNN_BERRY:
		case GREPA_BERRY:
		case HONDEW_BERRY:
		case KELPSY_BERRY:
		case MAGOST_BERRY:
		case NANAB_BERRY:
		case NOMEL_BERRY:
		case PAMTRE_BERRY:
		case PINAP_BERRY:
		case POMEG_BERRY:
		case QUALOT_BERRY:
		case RABUTA_BERRY:
		case SPELON_BERRY:
		case TAMATO_BERRY:
		case WEPEAR_BERRY:
			return 70;
		case APICOT_BERRY:
		case BELUE_BERRY:
		case CUSTAP_BERRY:
		case DURIN_BERRY:
		case ENIGMA_BERRY:
		case GANLON_BERRY:
		case JABOCA_BERRY:
		case LANSAT_BERRY:
		case LIECHI_BERRY:
		case MICLE_BERRY:
		case PETAYA_BERRY:
		case ROWAP_BERRY:
		case SALAC_BERRY:
		case STARF_BERRY:
		case WATMEL_BERRY:
			return 80;
		default:
			return 0;
	}
}

int Item::get_fling_power () const {
	switch (name) {
		case IRON_BALL:
			return 130;
		case HARD_STONE:
		case ARMOR_FOSSIL:
		case CLAW_FOSSIL:
		case DOME_FOSSIL:
		case HELIX_FOSSIL:
		case OLD_AMBER:
		case RARE_BONE:
		case ROOT_FOSSIL:
		case SKULL_FOSSIL:
			return 100;
		case DEEPSEATOOTH:
		case DRACO_PLATE:
		case DREAD_PLATE:
		case EARTH_PLATE:
		case FIST_PLATE:
		case FLAME_PLATE:
		case GRIP_CLAW:
		case ICICLE_PLATE:
		case INSECT_PLATE:
		case IRON_PLATE:
		case MEADOW_PLATE:
		case MIND_PLATE:
		case SKY_PLATE:
		case SPLASH_PLATE:
		case SPOOKY_PLATE:
		case STONE_PLATE:
		case THICK_CLUB:
		case TOXIC_PLATE:
		case ZAP_PLATE:
			return 90;
		case QUICK_CLAW:
		case RAZOR_CLAW:
		case STICKY_BARB:
		case DAWN_STONE:
		case DUSK_STONE:
		case ELECTIRIZER:
		case MAGMARIZER:
		case ODD_KEYSTONE:
		case OVAL_STONE:
		case PROTECTOR:
		case SHINY_STONE:
			return 80;
		case DRAGON_FANG:
		case POISON_BARB:
		case POWER_ANKLET:
		case POWER_BAND:
		case POWER_BELT:
		case POWER_BRACER:
		case POWER_LENS:
		case POWER_WEIGHT:
			return 70;
		case ADAMANT_ORB:
		case DAMP_ROCK:
		case HEAT_ROCK:
		case LUSTROUS_ORB:
		case MACHO_BRACE:
		case STICK:
			return 60;
		case SHARP_BEAK:
		case DUBIOUS_DISC:
			return 50;
		case ICY_ROCK:
		case LUCKY_PUNCH:
			return 40;
		case BERRY_JUICE:
		case BLACK_BELT:
		case BLACK_SLUDGE:
		case BLACKGLASSES:
		case CHARCOAL:
		case DEEPSEASCALE:
		case FLAME_ORB:
		case KINGS_ROCK:
		case LIFE_ORB:
		case LIGHT_BALL:
		case LIGHT_CLAY:
		case MAGNET:
		case METAL_COAT:
		case METRONOME:
		case MIRACLE_SEED:
		case MYSTIC_WATER:
		case NEVERMELTICE:
		case RAZOR_FANG:
		case SCOPE_LENS:
		case SHELL_BELL:
		case SOUL_DEW:
		case SPELL_TAG:
		case TOXIC_ORB:
		case TWISTEDSPOON:
		case AMULET_COIN:
		case ANTIDOTE:
		case AWAKENING:
		case BIG_MUSHROOM:
		case BIG_PEARL:
		case BLACK_FLUTE:
		case BLUE_FLUTE:
		case BLUE_SHARD:
		case BURN_HEAL:
		case CALCIUM:
		case CARBOS:
		case CLEANSE_TAG:
		case DAMP_MULCH:
		case DIRE_HIT:
		case DRAGON_SCALE:
		case ELIXIR:
		case ENERGY_ROOT:
		case ENERGYPOWDER:
		case ESCAPE_ROPE:
		case ETHER:
		case EVERSTONE:
		case EXP_SHARE:
		case FIRE_STONE:
		case FLUFFY_TAIL:
		case FRESH_WATER:
		case FULL_HEAL:
		case FULL_RESTORE:
		case GOOEY_MULCH:
		case GREEN_SHARD:
		case GROWTH_MULCH:
		case GUARD_SPEC:
		case HEAL_POWDER:
		case HEART_SCALE:
		case HONEY:
		case HP_UP:
		case HYPER_POTION:
		case ICE_HEAL:
		case IRON:
		case LAVA_COOKIE:
		case LEAF_STONE:
		case LEMONADE:
		case LUCKY_EGG:
		case MAX_ELIXIR:
		case MAX_ETHER:
		case MAX_POTION:
		case MAX_REPEL:
		case MAX_REVIVE:
		case MOOMOO_MILK:
		case MOON_STONE:
		case NUGGET:
		case OLD_GATEAU:
		case PARLYZ_HEAL:
		case PEARL:
		case POKE_DOLL:
		case POTION:
		case PP_MAX:
		case PP_UP:
		case PROTEIN:
		case RARE_CANDY:
		case RED_FLUTE:
		case RED_SHARD:
		case REPEL:
		case REVIVAL_HERB:
		case REVIVE:
		case SACRED_ASH:
		case SHOAL_SALT:
		case SHOAL_SHELL:
		case SMOKE_BALL:
		case SODA_POP:
		case STABLE_MULCH:
		case STAR_PIECE:
		case STARDUST:
		case SUN_STONE:
		case SUPER_POTION:
		case SUPER_REPEL:
		case THUNDERSTONE:
		case TINYMUSHROOM:
		case UP_GRADE:
		case WATER_STONE:
		case WHITE_FLUTE:
		case X_ACCURACY:
		case X_ATTACK:
		case X_DEFEND:
		case X_SP_DEF:
		case X_SPECIAL:
		case X_SPEED:
		case YELLOW_FLUTE:
		case YELLOW_SHARD:
		case ZINC:
			return 30;
		case BIG_ROOT:
		case BLUE_SCARF:
		case BRIGHTPOWDER:
		case CHOICE_BAND:
		case CHOICE_SCARF:
		case CHOICE_SPECS:
		case DESTINY_KNOT:
		case EXPERT_BELT:
		case FOCUS_BAND:
		case FOCUS_SASH:
		case FULL_INCENSE:
		case GREEN_SCARF:
		case LAGGING_TAIL:
		case LAX_INCENSE:
		case LEFTOVERS:
		case LUCK_INCENSE:
		case MENTAL_HERB:
		case METAL_POWDER:
		case MUSCLE_BAND:
		case ODD_INCENSE:
		case PINK_SCARF:
		case POWER_HERB:
		case PURE_INCENSE:
		case QUICK_POWDER:
		case REAPER_CLOTH:
		case RED_SCARF:
		case ROCK_INCENSE:
		case ROSE_INCENSE:
		case SEA_INCENSE:
		case SHED_SHELL:
		case SILK_SCARF:
		case SILVERPOWDER:
		case SMOOTH_ROCK:
		case SOFT_SAND:
		case SOOTHE_BELL:
		case WAVE_INCENSE:
		case WHITE_HERB:
		case WIDE_LENS:
		case WISE_GLASSES:
		case YELLOW_SCARF:
		case ZOOM_LENS:
		case AGUAV_BERRY:
		case ASPEAR_BERRY:
		case BABIRI_BERRY:
		case CHARTI_BERRY:
		case CHERI_BERRY:
		case CHESTO_BERRY:
		case CHILAN_BERRY:
		case CHOPLE_BERRY:
		case COBA_BERRY:
		case COLBUR_BERRY:
		case FIGY_BERRY:
		case HABAN_BERRY:
		case IAPAPA_BERRY:
		case KASIB_BERRY:
		case KEBIA_BERRY:
		case LEPPA_BERRY:
		case LUM_BERRY:
		case MAGO_BERRY:
		case OCCA_BERRY:
		case ORAN_BERRY:
		case PASSHO_BERRY:
		case PAYAPA_BERRY:
		case PECHA_BERRY:
		case PERSIM_BERRY:
		case RAWST_BERRY:
		case RAZZ_BERRY:
		case RINDO_BERRY:
		case SHUCA_BERRY:
		case SITRUS_BERRY:
		case TANGA_BERRY:
		case WACAN_BERRY:
		case WIKI_BERRY:
		case YACHE_BERRY:
		case BLUK_BERRY:
		case CORNN_BERRY:
		case GREPA_BERRY:
		case HONDEW_BERRY:
		case KELPSY_BERRY:
		case MAGOST_BERRY:
		case NANAB_BERRY:
		case NOMEL_BERRY:
		case PAMTRE_BERRY:
		case PINAP_BERRY:
		case POMEG_BERRY:
		case QUALOT_BERRY:
		case RABUTA_BERRY:
		case SPELON_BERRY:
		case TAMATO_BERRY:
		case WEPEAR_BERRY:
		case APICOT_BERRY:
		case BELUE_BERRY:
		case CUSTAP_BERRY:
		case DURIN_BERRY:
		case ENIGMA_BERRY:
		case GANLON_BERRY:
		case JABOCA_BERRY:
		case LANSAT_BERRY:
		case LIECHI_BERRY:
		case MICLE_BERRY:
		case PETAYA_BERRY:
		case ROWAP_BERRY:
		case SALAC_BERRY:
		case STARF_BERRY:
		case WATMEL_BERRY:
			return 10;
		default:
			return 0;
	}
}

bool Item::blocks_trick () const {
	switch (name) {
		case AIR_MAIL:
		case BLOOM_MAIL:
		case BRICK_MAIL:
		case BUBBLE_MAIL:
		case FLAME_MAIL:
		case GRASS_MAIL:
		case HEART_MAIL:
		case MOSAIC_MAIL:
		case SNOW_MAIL:
		case SPACE_MAIL:
		case STEEL_MAIL:
		case TUNNEL_MAIL:
			return true;
		default:
			return false;
	}
}

std::string Item::to_string (Items name) {
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

std::string Item::to_string () const {
	return to_string (name);
}

Item::Items Item::from_string (std::string const & str) {
	static std::map <std::string, Items> const converter {
		{ "No Item", NO_ITEM },
		{ "Adamant Orb", ADAMANT_ORB },
		{ "Aguav Berry", AGUAV_BERRY },
		{ "Apicot Berry", APICOT_BERRY },
		{ "Aspear Berry", ASPEAR_BERRY },
		{ "Babiri Berry", BABIRI_BERRY },
		{ "Belue Berry", BELUE_BERRY },
		{ "Berry Juice", BERRY_JUICE },
		{ "Big Root", BIG_ROOT },
		{ "Black Belt", BLACK_BELT },
		{ "Black Sludge", BLACK_SLUDGE },
		{ "BlackGlasses", BLACKGLASSES },
		{ "Bluk Berry", BLUK_BERRY },
		{ "BrightPowder", BRIGHTPOWDER },
		{ "Charcoal", CHARCOAL },
		{ "Charti Berry", CHARTI_BERRY },
		{ "Cheri Berry", CHERI_BERRY },
		{ "Chesto Berry", CHESTO_BERRY },
		{ "Chilan Berry", CHILAN_BERRY },
		{ "Choice Band", CHOICE_BAND },
		{ "Choice Scarf", CHOICE_SCARF },
		{ "Choice Specs", CHOICE_SPECS },
		{ "Chople Berry", CHOPLE_BERRY },
		{ "Coba Berry", COBA_BERRY },
		{ "Colbur Berry", COLBUR_BERRY },
		{ "Cornn Berry", CORNN_BERRY },
		{ "Custap Berry", CUSTAP_BERRY },
		{ "Damp Rock", DAMP_ROCK },
		{ "DeepSeaScale", DEEPSEASCALE },
		{ "DeepSeaTooth", DEEPSEATOOTH },
		{ "Destiny Knot", DESTINY_KNOT },
		{ "Draco Plate", DRACO_PLATE },
		{ "Dragon Fang", DRAGON_FANG },
		{ "Dread Plate", DREAD_PLATE },
		{ "Durin Berry", DURIN_BERRY },
		{ "Earth Plate", EARTH_PLATE },
		{ "Enigma Berry", ENIGMA_BERRY },
		{ "Expert Belt", EXPERT_BELT },
		{ "Figy Berry", FIGY_BERRY },
		{ "Fist Plate", FIST_PLATE },
		{ "Flame Orb", FLAME_ORB },
		{ "Flame Plate", FLAME_PLATE },
		{ "Focus Band", FOCUS_BAND },
		{ "Focus Sash", FOCUS_SASH },
		{ "Ganlon Berry", GANLON_BERRY },
		{ "Grepa Berry", GREPA_BERRY },
		{ "Grip Claw", GRIP_CLAW },
		{ "Griseous Orb", GRISEOUS_ORB },
		{ "Haban Berry", HABAN_BERRY },
		{ "Hard Stone", HARD_STONE },
		{ "Heat Rock", HEAT_ROCK },
		{ "Hondew Berry", HONDEW_BERRY },
		{ "Iapapa Berry", IAPAPA_BERRY },
		{ "Icicle Plate", ICICLE_PLATE },
		{ "Icy Rock", ICY_ROCK },
		{ "Insect Plate", INSECT_PLATE },
		{ "Iron Ball", IRON_BALL },
		{ "Iron Plate", IRON_PLATE },
		{ "Jaboca Berry", JABOCA_BERRY },
		{ "Kasib Berry", KASIB_BERRY },
		{ "Kebia Berry", KEBIA_BERRY },
		{ "Kelpsy Berry", KELPSY_BERRY },
		{ "King's Rock", KINGS_ROCK },
		{ "Razor Fang", RAZOR_FANG },
		{ "Full Incense", FULL_INCENSE },
		{ "Lagging Tail", LAGGING_TAIL },
		{ "Lansat Berry", LANSAT_BERRY },
		{ "Lax Incense", LAX_INCENSE },
		{ "Leftovers", LEFTOVERS },
		{ "Leppa Berry", LEPPA_BERRY },
		{ "Liechi Berry", LIECHI_BERRY },
		{ "Life Orb", LIFE_ORB },
		{ "Light Ball", LIGHT_BALL },
		{ "Light Clay", LIGHT_CLAY },
		{ "Lucky Punch", LUCKY_PUNCH },
		{ "Lum Berry", LUM_BERRY },
		{ "Lustrous Orb", LUSTROUS_ORB },
		{ "Macho Brace", MACHO_BRACE },
		{ "Magnet", MAGNET },
		{ "Mago Berry", MAGO_BERRY },
		{ "Magost Berry", MAGOST_BERRY },
		{ "Air Mail", AIR_MAIL },
		{ "Bloom Mail", BLOOM_MAIL },
		{ "Brick Mail", BRICK_MAIL },
		{ "Bubble Mail", BUBBLE_MAIL },
		{ "Flame Mail", FLAME_MAIL },
		{ "Grass Mail", GRASS_MAIL },
		{ "Heart Mail", HEART_MAIL },
		{ "Mosaic Mail", MOSAIC_MAIL },
		{ "Snow Mail", SNOW_MAIL },
		{ "Space Mail", SPACE_MAIL },
		{ "Steel Mail", STEEL_MAIL },
		{ "Tunnel Mail", TUNNEL_MAIL },
		{ "Meadow Plate", MEADOW_PLATE },
		{ "Mental Herb", MENTAL_HERB },
		{ "Metal Coat", METAL_COAT },
		{ "Metal Powder", METAL_POWDER },
		{ "Metronome", METRONOME },
		{ "Micle Berry", MICLE_BERRY },
		{ "Mind Plate", MIND_PLATE },
		{ "Miracle Seed", MIRACLE_SEED },
		{ "Muscle Band", MUSCLE_BAND },
		{ "Mystic Water", MYSTIC_WATER },
		{ "Nanab Berry", NANAB_BERRY },
		{ "NeverMeltIce", NEVERMELTICE },
		{ "Nomel Berry", NOMEL_BERRY },
		{ "Occa Berry", OCCA_BERRY },
		{ "Odd Incense", ODD_INCENSE },
		{ "Oran Berry", ORAN_BERRY },
		{ "Pamtre Berry", PAMTRE_BERRY },
		{ "Passho Berry", PASSHO_BERRY },
		{ "Payapa Berry", PAYAPA_BERRY },
		{ "Pecha Berry", PECHA_BERRY },
		{ "Persim Berry", PERSIM_BERRY },
		{ "Petaya Berry", PETAYA_BERRY },
		{ "Pinap Berry", PINAP_BERRY },
		{ "Poison Barb", POISON_BARB },
		{ "Pomeg Berry", POMEG_BERRY },
		{ "Power Herb", POWER_HERB },
		{ "Power Anklet", POWER_ANKLET },
		{ "Power Band", POWER_BAND },
		{ "Power Belt", POWER_BELT },
		{ "Power Bracer", POWER_BRACER },
		{ "Power Lens", POWER_LENS },
		{ "Power Weight", POWER_WEIGHT },
		{ "Qualot Berry", QUALOT_BERRY },
		{ "Quick Claw", QUICK_CLAW },
		{ "Quick Powder", QUICK_POWDER },
		{ "Rabuta Berry", RABUTA_BERRY },
		{ "Rawst Berry", RAWST_BERRY },
		{ "Razor Claw", RAZOR_CLAW },
		{ "Razz Berry", RAZZ_BERRY },
		{ "Rindo Berry", RINDO_BERRY },
		{ "Rock Incense", ROCK_INCENSE },
		{ "Rose Incense", ROSE_INCENSE },
		{ "Rowap Berry", ROWAP_BERRY },
		{ "Salac Berry", SALAC_BERRY },
		{ "Scope Lens", SCOPE_LENS },
		{ "Sharp Beak", SHARP_BEAK },
		{ "Shed Shell", SHED_SHELL },
		{ "Shell Bell", SHELL_BELL },
		{ "Shuca Berry", SHUCA_BERRY },
		{ "Silk Scarf", SILK_SCARF },
		{ "SilverPowder", SILVERPOWDER },
		{ "Sitrus Berry", SITRUS_BERRY },
		{ "Sky Plate", SKY_PLATE },
		{ "Smooth Rock", SMOOTH_ROCK },
		{ "Soft Sand", SOFT_SAND },
		{ "Soul Dew", SOUL_DEW },
		{ "Spell Tag", SPELL_TAG },
		{ "Spelon Berry", SPELON_BERRY },
		{ "Splash Plate", SPLASH_PLATE },
		{ "Spooky Plate", SPOOKY_PLATE },
		{ "Starf Berry", STARF_BERRY },
		{ "Stick", STICK },
		{ "Sticky Barb", STICKY_BARB },
		{ "Stone Plate", STONE_PLATE },
		{ "Tamato Berry", TAMATO_BERRY },
		{ "Tanga Berry", TANGA_BERRY },
		{ "Thick Club", THICK_CLUB },
		{ "Toxic Orb", TOXIC_ORB },
		{ "Toxic Plate", TOXIC_PLATE },
		{ "TwistedSpoon", TWISTEDSPOON },
		{ "Wacan Berry", WACAN_BERRY },
		{ "Watmel Berry", WATMEL_BERRY },
		{ "Sea Incense", SEA_INCENSE },
		{ "Wave Incense", WAVE_INCENSE },
		{ "Wepear Berry", WEPEAR_BERRY },
		{ "White Herb", WHITE_HERB },
		{ "Wide Lens", WIDE_LENS },
		{ "Wiki Berry", WIKI_BERRY },
		{ "Wise Glasses", WISE_GLASSES },
		{ "Yache Berry", YACHE_BERRY },
		{ "Zap Plate", ZAP_PLATE },
		{ "Zoom Lens", ZOOM_LENS },
		{ "Cherish Ball", CHERISH_BALL },
		{ "Dive Ball", DIVE_BALL },
		{ "Dusk Ball", DUSK_BALL },
		{ "Great Ball", GREAT_BALL },
		{ "Heal Ball", HEAL_BALL },
		{ "Luxury Ball", LUXURY_BALL },
		{ "Master Ball", MASTER_BALL },
		{ "Nest Ball", NEST_BALL },
		{ "Net Ball", NET_BALL },
		{ "Poke Ball", POKE_BALL },
		{ "Premier Ball", PREMIER_BALL },
		{ "Quick Ball", QUICK_BALL },
		{ "Repeat Ball", REPEAT_BALL },
		{ "Timer Ball", TIMER_BALL },
		{ "TM01", TM01 },
		{ "TM02", TM02 },
		{ "TM03", TM03 },
		{ "TM04", TM04 },
		{ "TM05", TM05 },
		{ "TM06", TM06 },
		{ "TM07", TM07 },
		{ "TM08", TM08 },
		{ "TM09", TM09 },
		{ "TM10", TM10 },
		{ "TM11", TM11 },
		{ "TM12", TM12 },
		{ "TM13", TM13 },
		{ "TM14", TM14 },
		{ "TM15", TM15 },
		{ "TM16", TM16 },
		{ "TM17", TM17 },
		{ "TM18", TM18 },
		{ "TM19", TM19 },
		{ "TM20", TM20 },
		{ "TM21", TM21 },
		{ "TM22", TM22 },
		{ "TM23", TM23 },
		{ "TM24", TM24 },
		{ "TM25", TM25 },
		{ "TM26", TM26 },
		{ "TM27", TM27 },
		{ "TM28", TM28 },
		{ "TM29", TM29 },
		{ "TM30", TM30 },
		{ "TM31", TM31 },
		{ "TM32", TM32 },
		{ "TM33", TM33 },
		{ "TM34", TM34 },
		{ "TM35", TM35 },
		{ "TM36", TM36 },
		{ "TM37", TM37 },
		{ "TM38", TM38 },
		{ "TM39", TM39 },
		{ "TM40", TM40 },
		{ "TM41", TM41 },
		{ "TM42", TM42 },
		{ "TM43", TM43 },
		{ "TM44", TM44 },
		{ "TM45", TM45 },
		{ "TM46", TM46 },
		{ "TM47", TM47 },
		{ "TM48", TM48 },
		{ "TM49", TM49 },
		{ "TM50", TM50 },
		{ "TM51", TM51 },
		{ "TM52", TM52 },
		{ "TM53", TM53 },
		{ "TM54", TM54 },
		{ "TM55", TM55 },
		{ "TM56", TM56 },
		{ "TM57", TM57 },
		{ "TM58", TM58 },
		{ "TM59", TM59 },
		{ "TM60", TM60 },
		{ "TM61", TM61 },
		{ "TM62", TM62 },
		{ "TM63", TM63 },
		{ "TM64", TM64 },
		{ "TM65", TM65 },
		{ "TM66", TM66 },
		{ "TM67", TM67 },
		{ "TM68", TM68 },
		{ "TM69", TM69 },
		{ "TM70", TM70 },
		{ "TM71", TM71 },
		{ "TM72", TM72 },
		{ "TM73", TM73 },
		{ "TM74", TM74 },
		{ "TM75", TM75 },
		{ "TM76", TM76 },
		{ "TM77", TM77 },
		{ "TM78", TM78 },
		{ "TM79", TM79 },
		{ "TM80", TM80 },
		{ "TM81", TM81 },
		{ "TM82", TM82 },
		{ "TM83", TM83 },
		{ "TM84", TM84 },
		{ "TM85", TM85 },
		{ "TM86", TM86 },
		{ "TM87", TM87 },
		{ "TM88", TM88 },
		{ "TM89", TM89 },
		{ "TM90", TM90 },
		{ "TM91", TM91 },
		{ "TM92", TM92 },
		{ "Ultra Ball", ULTRA_BALL },
		{ "Blue Scarf", BLUE_SCARF },
		{ "Green Scarf", GREEN_SCARF },
		{ "Luck Incense", LUCK_INCENSE },
		{ "Pink Scarf", PINK_SCARF },
		{ "Pure Incense", PURE_INCENSE },
		{ "Reaper Cloth", REAPER_CLOTH },
		{ "Red Scarf", RED_SCARF },
		{ "Soothe Bell", SOOTHE_BELL },
		{ "Yellow Scarf", YELLOW_SCARF },
		{ "Amulet Coin", AMULET_COIN },
		{ "Antidote", ANTIDOTE },
		{ "Awakening", AWAKENING },
		{ "Big Mushroom", BIG_MUSHROOM },
		{ "Big Pearl", BIG_PEARL },
		{ "Black Flute", BLACK_FLUTE },
		{ "Blue Flute", BLUE_FLUTE },
		{ "Blue Shard", BLUE_SHARD },
		{ "Burn Heal", BURN_HEAL },
		{ "Calcium", CALCIUM },
		{ "Carbos", CARBOS },
		{ "Cleanse Tag", CLEANSE_TAG },
		{ "Damp Mulch", DAMP_MULCH },
		{ "Dire Hit", DIRE_HIT },
		{ "Dragon Scale", DRAGON_SCALE },
		{ "Elixir", ELIXIR },
		{ "Energy Root", ENERGY_ROOT },
		{ "EnergyPowder", ENERGYPOWDER },
		{ "Escape Rope", ESCAPE_ROPE },
		{ "Ether", ETHER },
		{ "Everstone", EVERSTONE },
		{ "Exp. Share", EXP_SHARE },
		{ "Fire Stone", FIRE_STONE },
		{ "Fluffy Tail", FLUFFY_TAIL },
		{ "Fresh Water", FRESH_WATER },
		{ "Full Heal", FULL_HEAL },
		{ "Full Restore", FULL_RESTORE },
		{ "Gooey Mulch", GOOEY_MULCH },
		{ "Green Shard", GREEN_SHARD },
		{ "Growth Mulch", GROWTH_MULCH },
		{ "Guard Spec.", GUARD_SPEC },
		{ "Heal Powder", HEAL_POWDER },
		{ "Heart Scale", HEART_SCALE },
		{ "Honey", HONEY },
		{ "HP Up", HP_UP },
		{ "Hyper Potion", HYPER_POTION },
		{ "Ice Heal", ICE_HEAL },
		{ "Iron", IRON },
		{ "Lava Cookie", LAVA_COOKIE },
		{ "Leaf Stone", LEAF_STONE },
		{ "Lemonade", LEMONADE },
		{ "Lucky Egg", LUCKY_EGG },
		{ "Max Elixir", MAX_ELIXIR },
		{ "Max Ether", MAX_ETHER },
		{ "Max Potion", MAX_POTION },
		{ "Max Repel", MAX_REPEL },
		{ "Max Revive", MAX_REVIVE },
		{ "MooMoo Milk", MOOMOO_MILK },
		{ "Moon Stone", MOON_STONE },
		{ "Nugget", NUGGET },
		{ "Old Gateau", OLD_GATEAU },
		{ "Parlyz Heal", PARLYZ_HEAL },
		{ "Pearl", PEARL },
		{ "Poke Doll", POKE_DOLL },
		{ "Potion", POTION },
		{ "PP Max", PP_MAX },
		{ "PP Up", PP_UP },
		{ "Protein", PROTEIN },
		{ "Rare Candy", RARE_CANDY },
		{ "Red Flute", RED_FLUTE },
		{ "Red Shard", RED_SHARD },
		{ "Repel", REPEL },
		{ "Revival Herb", REVIVAL_HERB },
		{ "Revive", REVIVE },
		{ "Sacred Ash", SACRED_ASH },
		{ "Shoal Salt", SHOAL_SALT },
		{ "Shoal Shell", SHOAL_SHELL },
		{ "Smoke Ball", SMOKE_BALL },
		{ "Soda Pop", SODA_POP },
		{ "Stable Mulch", STABLE_MULCH },
		{ "Star Piece", STAR_PIECE },
		{ "Stardust", STARDUST },
		{ "Sun Stone", SUN_STONE },
		{ "Super Potion", SUPER_POTION },
		{ "Super Repel", SUPER_REPEL },
		{ "Thunderstone", THUNDERSTONE },
		{ "TinyMushroom", TINYMUSHROOM },
		{ "Up-Grade", UP_GRADE },
		{ "Water Stone", WATER_STONE },
		{ "White Flute", WHITE_FLUTE },
		{ "X Accuracy", X_ACCURACY },
		{ "X Attack", X_ATTACK },
		{ "X Defend", X_DEFEND },
		{ "X Sp. Def", X_SP_DEF },
		{ "X Special", X_SPECIAL },
		{ "X Speed", X_SPEED },
		{ "Yellow Flute", YELLOW_FLUTE },
		{ "Yellow Shard", YELLOW_SHARD },
		{ "Zinc", ZINC },
		{ "Dubious Disc", DUBIOUS_DISC },
		{ "Dawn Stone", DAWN_STONE },
		{ "Dusk Stone", DUSK_STONE },
		{ "Electirizer", ELECTIRIZER },
		{ "Magmarizer", MAGMARIZER },
		{ "Odd Keystone", ODD_KEYSTONE },
		{ "Oval Stone", OVAL_STONE },
		{ "Protector", PROTECTOR },
		{ "Shiny Stone", SHINY_STONE },
		{ "Armor Fossil", ARMOR_FOSSIL },
		{ "Claw Fossil", CLAW_FOSSIL },
		{ "Dome Fossil", DOME_FOSSIL },
		{ "Helix Fossil", HELIX_FOSSIL },
		{ "Old Amber", OLD_AMBER },
		{ "Rare Bone", RARE_BONE },
		{ "Root Fossil", ROOT_FOSSIL },
		{ "Skull Fossil", SKULL_FOSSIL },
		{ "Bicycle", BICYCLE },
		{ "Coin Case", COIN_CASE },
		{ "Coupon 1", COUPON_1 },
		{ "Coupon 2", COUPON_2 },
		{ "Coupon 3", COUPON_3 },
		{ "Explorer Kit", EXPLORER_KIT },
		{ "Fashion Case", FASHION_CASE },
		{ "Galactic Key", GALACTIC_KEY },
		{ "Good Rod", GOOD_ROD },
		{ "HM01", HM01 },
		{ "HM02", HM02 },
		{ "HM03", HM03 },
		{ "HM04", HM04 },
		{ "HM05", HM05 },
		{ "HM06", HM06 },
		{ "HM07", HM07 },
		{ "HM08", HM08 },
		{ "Journal", JOURNAL },
		{ "Loot Sack", LOOT_SACK },
		{ "Lunar Wing", LUNAR_WING },
		{ "Member Card", MEMBER_CARD},
		{ "Oak's Letter", OAKS_LETTER },
		{ "Old Charm", OLD_CHARM },
		{ "Old Rod", OLD_ROD },
		{ "Pal Pad", PAL_PAD },
		{ "Parcel", PARCEL },
		{ "Poffin Case", POFFIN_CASE },
		{ "Point Card", POINT_CARD },
		{ "Poke Radar", POKE_RADAR },
		{ "Safari Ball", SAFARI_BALL },
		{ "Seal Bag", SEAL_BAG },
		{ "Seal Case", SEAL_CASE },
		{ "SecretPotion", SECRETPOTION },
		{ "Sprayduck", SPRAYDUCK },
		{ "Storage Key", STORAGE_KEY },
		{ "Suite Key", SUITE_KEY },
		{ "Super Rod", SUPER_ROD },
		{ "Town Map", TOWN_MAP },
		{ "VS Seeker", VS_SEEKER },
		{ "Works Key", WORKS_KEY }
	};
	return converter.find (str)->second;
}

void Item::set_name_from_string (std::string const & str) {
	name = from_string (str);
}
}
