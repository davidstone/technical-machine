// Item header
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "item.h"

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
	int power;
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
			power = 60;
			break;
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
			power = 70;
			break;
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
			power = 80;
			break;
		default:
			power = 0;
			break;
	}
	return power;
}

int Item::get_fling_power () const {
	int power;
	switch (name) {
		case IRON_BALL:
			power = 130;
			break;
		case HARD_STONE:
		case OTHER100:
			power = 100;
			break;
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
			power = 90;
			break;
		case QUICK_CLAW:
		case RAZOR_CLAW:
		case STICKY_BARB:
		case OTHER80:
			power = 80;
			break;
		case DRAGON_FANG:
		case POISON_BARB:
		case POWER_ANKLET:
		case POWER_BAND:
		case POWER_BELT:
		case POWER_BRACER:
		case POWER_LENS:
		case POWER_WEIGHT:
			power = 70;
			break;
		case ADAMANT_ORB:
		case DAMP_ROCK:
		case HEAT_ROCK:
		case LUSTROUS_ORB:
		case MACHO_BRACE:
		case STICK:
			power = 60;
			break;
		case SHARP_BEAK:
		case OTHER50:
			power = 50;
			break;
		case ICY_ROCK:
		case LUCKY_PUNCH:
			power = 40;
			break;
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
		case OTHER30:
			power = 30;
			break;
		default:
			power = 10;
			break;
	}
	return power;
}

bool Item::blocks_trick () const {
	switch	 (name) {
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

std::string Item::get_name () const {
	static std::string const item_name [] = { "No Item", "Adamant Orb ", "Aguav Berry", "Air Mail", "Apicot Berry", "Aspear Berry", "Babiri Berry", "Belue Berry", "Berry Juice", "Big Root", "Black Belt", "Black Sludge", "BlackGlasses", "Bloom Mail", "Bluk Berry", "Brick Mail", "BrightPowder", "Bubble Mail", "Charcoal", "Charti Berry", "Cheri Berry", "Chesto Berry", "Chilan Berry", "Choice Band", "Choice Scarf", "Choice Specs", "Chople Berry", "Coba Berry", "Colbur Berry", "Cornn Berry", "Custap Berry", "Damp Rock", "DeepSeaScale", "DeepSeaTooth", "Destiny Knot", "Draco Plate", "Dragon Fang", "Dread Plate", "Durin Berry", "Earth Plate", "Enigma Berry", "Expert Belt", "Figy Berry", "Fist Plate", "Flame Mail", "Flame Orb", "Flame Plate", "Focus Band", "Focus Sash", "Full Incense", "Ganlon Berry", "Grass Mail", "Grepa Berry", "Grip Claw", "Griseous Orb", "Haban Berry", "Hard Stone", "Heart Mail", "Heat Rock", "Hondew Berry", "Iapapa Berry", "Icicle Plate", "Icy Rock", "Insect Plate", "Iron Ball", "Iron Plate", "Jaboca Berry", "Kasib Berry", "Kebia Berry", "Kelpsy Berry", "King's Rock", "Lagging Tail", "Lansat Berry", "Lax Incense", "Leftovers", "Leppa Berry", "Liechi Berry", "Life Orb", "Light Ball", "Light Clay", "Lucky Punch", "Lum Berry", "Lustrous Orb", "Macho Brace", "Magnet", "Mago Berry", "Magost Berry", "Meadow Plate", "Mental Herb", "Metal Coat", "Metal Powder", "Metronome", "Micle Berry", "Mind Plate", "Miracle Seed", "Mosaic Mail", "Muscle Band", "Mystic Water", "Nanab Berry", "NeverMeltIce", "Nomel Berry", "Occa Berry", "Odd Incense", "Oran Berry", "Pamtre Berry", "Passho Berry", "Payapa Berry", "Pecha Berry", "Persim Berry", "Petaya Berry", "Pinap Berry", "Poison Barb", "Pomeg Berry", "Power Anklet", "Power Band", "Power Belt", "Power Bracer", "Power Herb", "Power Lens", "Power Weight", "Qualot Berry", "Quick Claw", "Quick Powder", "Rabuta Berry", "Rawst Berry", "Razor Claw", "Razor Fang", "Razz Berry", "Rindo Berry", "Rock Incense", "Rose Incense", "Rowap Berry", "Salac Berry", "Scope Lens", "Sea Incense", "Sharp Beak", "Shed Shell", "Shell Bell", "Shuca Berry", "Silk Scarf", "SilverPowder", "Sitrus Berry", "Sky Plate", "Smooth Rock", "Snow Mail", "Soft Sand", "Soul Dew", "Space Mail", "Spell Tag", "Spelon Berry", "Splash Plate", "Spooky Plate", "Starf Berry", "Steel Mail", "Stick", "Sticky Barb", "Stone Plate", "Tamato Berry", "Tanga Berry", "Thick Club", "Toxic Orb", "Toxic Plate", "Tunnel Mail", "TwistedSpoon", "Wacan Berry", "Watmel Berry", "Wave Incense", "Wepear Berry", "White Herb", "Wide Lens", "Wiki Berry", "Wise Glasses", "Yache Berry", "Zap Plate", "Zoom Lens", "Other", "Other10", "Other30", "Other50", "Other80", "Other100", "End Item" };
	return item_name [name];
}

class Item_From_String {
	public:
		std::map <std::string, Item::Items> item;
		Item_From_String ():
			item {
				{ "No Item", Item::NO_ITEM },
				{ "Adamant Orb ", Item::ADAMANT_ORB },
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
				{ "DeepSeaTooth", Item::DEEPSEATOOTH },
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
				{ "Other", Item::OTHER },
				{ "Cherish Ball", Item::OTHER },
				{ "Dive Ball", Item::OTHER },
				{ "Dusk Ball", Item::OTHER },
				{ "Great Ball", Item::OTHER },
				{ "Heal Ball", Item::OTHER },
				{ "Luxury Ball", Item::OTHER },
				{ "Master Ball", Item::OTHER },
				{ "Nest Ball", Item::OTHER },
				{ "Net Ball", Item::OTHER },
				{ "Poke Ball", Item::OTHER },
				{ "Premier Ball", Item::OTHER },
				{ "Quick Ball", Item::OTHER },
				{ "Repeat Ball", Item::OTHER },
				{ "Timer Ball", Item::OTHER },
				{ "TM01", Item::OTHER },
				{ "TM02", Item::OTHER },
				{ "TM03", Item::OTHER },
				{ "TM04", Item::OTHER },
				{ "TM05", Item::OTHER },
				{ "TM06", Item::OTHER },
				{ "TM07", Item::OTHER },
				{ "TM08", Item::OTHER },
				{ "TM09", Item::OTHER },
				{ "TM10", Item::OTHER },
				{ "TM11", Item::OTHER },
				{ "TM12", Item::OTHER },
				{ "TM13", Item::OTHER },
				{ "TM14", Item::OTHER },
				{ "TM15", Item::OTHER },
				{ "TM16", Item::OTHER },
				{ "TM17", Item::OTHER },
				{ "TM18", Item::OTHER },
				{ "TM19", Item::OTHER },
				{ "TM20", Item::OTHER },
				{ "TM21", Item::OTHER },
				{ "TM22", Item::OTHER },
				{ "TM23", Item::OTHER },
				{ "TM24", Item::OTHER },
				{ "TM25", Item::OTHER },
				{ "TM26", Item::OTHER },
				{ "TM27", Item::OTHER },
				{ "TM28", Item::OTHER },
				{ "TM29", Item::OTHER },
				{ "TM30", Item::OTHER },
				{ "TM31", Item::OTHER },
				{ "TM32", Item::OTHER },
				{ "TM33", Item::OTHER },
				{ "TM34", Item::OTHER },
				{ "TM35", Item::OTHER },
				{ "TM36", Item::OTHER },
				{ "TM37", Item::OTHER },
				{ "TM38", Item::OTHER },
				{ "TM39", Item::OTHER },
				{ "TM40", Item::OTHER },
				{ "TM41", Item::OTHER },
				{ "TM42", Item::OTHER },
				{ "TM43", Item::OTHER },
				{ "TM44", Item::OTHER },
				{ "TM45", Item::OTHER },
				{ "TM46", Item::OTHER },
				{ "TM47", Item::OTHER },
				{ "TM48", Item::OTHER },
				{ "TM49", Item::OTHER },
				{ "TM50", Item::OTHER },
				{ "TM51", Item::OTHER },
				{ "TM52", Item::OTHER },
				{ "TM53", Item::OTHER },
				{ "TM54", Item::OTHER },
				{ "TM55", Item::OTHER },
				{ "TM56", Item::OTHER },
				{ "TM57", Item::OTHER },
				{ "TM58", Item::OTHER },
				{ "TM59", Item::OTHER },
				{ "TM60", Item::OTHER },
				{ "TM61", Item::OTHER },
				{ "TM62", Item::OTHER },
				{ "TM63", Item::OTHER },
				{ "TM64", Item::OTHER },
				{ "TM65", Item::OTHER },
				{ "TM66", Item::OTHER },
				{ "TM67", Item::OTHER },
				{ "TM68", Item::OTHER },
				{ "TM69", Item::OTHER },
				{ "TM70", Item::OTHER },
				{ "TM71", Item::OTHER },
				{ "TM72", Item::OTHER },
				{ "TM73", Item::OTHER },
				{ "TM74", Item::OTHER },
				{ "TM75", Item::OTHER },
				{ "TM76", Item::OTHER },
				{ "TM77", Item::OTHER },
				{ "TM78", Item::OTHER },
				{ "TM79", Item::OTHER },
				{ "TM80", Item::OTHER },
				{ "TM81", Item::OTHER },
				{ "TM82", Item::OTHER },
				{ "TM83", Item::OTHER },
				{ "TM84", Item::OTHER },
				{ "TM85", Item::OTHER },
				{ "TM86", Item::OTHER },
				{ "TM87", Item::OTHER },
				{ "TM88", Item::OTHER },
				{ "TM89", Item::OTHER },
				{ "TM90", Item::OTHER },
				{ "TM91", Item::OTHER },
				{ "TM92", Item::OTHER },
				{ "Ultra Ball", Item::OTHER },
				{ "Other10", Item::OTHER10 },
				{ "Blue Scarf", Item::OTHER10 },
				{ "Green Scarf", Item::OTHER10 },
				{ "Luck Incense", Item::OTHER10 },
				{ "Pink Scarf", Item::OTHER10 },
				{ "Pure Incense", Item::OTHER10 },
				{ "Reaper Cloth", Item::OTHER10 },
				{ "Red Scarf", Item::OTHER10 },
				{ "Soothe Bell", Item::OTHER10 },
				{ "Yellow Scarf", Item::OTHER10 },
				{ "Other30", Item::OTHER30 },
				{ "Amulet Coin", Item::OTHER30 },
				{ "Antidote", Item::OTHER30 },
				{ "Awakening", Item::OTHER30 },
				{ "Big Mushroom", Item::OTHER30 },
				{ "Big Pearl", Item::OTHER30 },
				{ "Black Flute", Item::OTHER30 },
				{ "Blue Flute", Item::OTHER30 },
				{ "Blue Shard", Item::OTHER30 },
				{ "Burn Heal", Item::OTHER30 },
				{ "Calcium", Item::OTHER30 },
				{ "Carbos", Item::OTHER30 },
				{ "Cleanse Tag", Item::OTHER30 },
				{ "Damp Mulch", Item::OTHER30 },
				{ "Dire Hit", Item::OTHER30 },
				{ "Dragon Scale", Item::OTHER30 },
				{ "Elixir", Item::OTHER30 },
				{ "Energy Root", Item::OTHER30 },
				{ "EnergyPowder", Item::OTHER30 },
				{ "Escape Rope", Item::OTHER30 },
				{ "Ether", Item::OTHER30 },
				{ "Everstone", Item::OTHER30 },
				{ "Exp. Share", Item::OTHER30 },
				{ "Fire Stone", Item::OTHER30 },
				{ "Fluffy Tail", Item::OTHER30 },
				{ "Fresh Water", Item::OTHER30 },
				{ "Full Heal", Item::OTHER30 },
				{ "Full Restore", Item::OTHER30 },
				{ "Gooey Mulch", Item::OTHER30 },
				{ "Green Shard", Item::OTHER30 },
				{ "Growth Mulch", Item::OTHER30 },
				{ "Guard Spec.", Item::OTHER30 },
				{ "Heal Powder", Item::OTHER30 },
				{ "Heart Scale", Item::OTHER30 },
				{ "Honey", Item::OTHER30 },
				{ "HP Up", Item::OTHER30 },
				{ "Hyper Potion", Item::OTHER30 },
				{ "Ice Heal", Item::OTHER30 },
				{ "Iron", Item::OTHER30 },
				{ "Lava Cookie", Item::OTHER30 },
				{ "Leaf Stone", Item::OTHER30 },
				{ "Lemonade", Item::OTHER30 },
				{ "Lucky Egg", Item::OTHER30 },
				{ "Max Elixir", Item::OTHER30 },
				{ "Max Ether", Item::OTHER30 },
				{ "Max Potion", Item::OTHER30 },
				{ "Max Repel", Item::OTHER30 },
				{ "Max Revive", Item::OTHER30 },
				{ "MooMoo Milk", Item::OTHER30 },
				{ "Moon Stone", Item::OTHER30 },
				{ "Nugget", Item::OTHER30 },
				{ "Old Gateau", Item::OTHER30 },
				{ "Parlyz Heal", Item::OTHER30 },
				{ "Pearl", Item::OTHER30 },
				{ "Poke Doll", Item::OTHER30 },
				{ "Potion", Item::OTHER30 },
				{ "PP Max", Item::OTHER30 },
				{ "PP Up", Item::OTHER30 },
				{ "Protein", Item::OTHER30 },
				{ "Rare Candy", Item::OTHER30 },
				{ "Red Flute", Item::OTHER30 },
				{ "Red Shard", Item::OTHER30 },
				{ "Repel", Item::OTHER30 },
				{ "Revival Herb", Item::OTHER30 },
				{ "Revive", Item::OTHER30 },
				{ "Sacred Ash", Item::OTHER30 },
				{ "Shoal Salt", Item::OTHER30 },
				{ "Shoal Shell", Item::OTHER30 },
				{ "Smoke Ball", Item::OTHER30 },
				{ "Soda Pop", Item::OTHER30 },
				{ "Stable Mulch", Item::OTHER30 },
				{ "Star Piece", Item::OTHER30 },
				{ "Stardust", Item::OTHER30 },
				{ "Sun Stone", Item::OTHER30 },
				{ "Super Potion", Item::OTHER30 },
				{ "Super Repel", Item::OTHER30 },
				{ "Thunderstone", Item::OTHER30 },
				{ "TinyMushroom", Item::OTHER30 },
				{ "Up-Grade", Item::OTHER30 },
				{ "Water Stone", Item::OTHER30 },
				{ "White Flute", Item::OTHER30 },
				{ "X Accuracy", Item::OTHER30 },
				{ "X Attack", Item::OTHER30 },
				{ "X Defend", Item::OTHER30 },
				{ "X Sp. Def", Item::OTHER30 },
				{ "X Special", Item::OTHER30 },
				{ "X Speed", Item::OTHER30 },
				{ "Yellow Flute", Item::OTHER30 },
				{ "Yellow Shard", Item::OTHER30 },
				{ "Zinc", Item::OTHER30 },
				{ "Other50", Item::OTHER50 },
				{ "Dubious Disc", Item::OTHER50 },
				{ "Other80", Item::OTHER80 },
				{ "Dawn Stone", Item::OTHER80 },
				{ "Dusk Stone", Item::OTHER80 },
				{ "Electirizer", Item::OTHER80 },
				{ "Magmarizer", Item::OTHER80 },
				{ "Odd Keystone", Item::OTHER80 },
				{ "Oval Stone", Item::OTHER80 },
				{ "Protector", Item::OTHER80 },
				{ "Shiny Stone", Item::OTHER80 },
				{ "Other100", Item::OTHER100 },
				{ "Armor Fossil", Item::OTHER100 },
				{ "Claw Fossil", Item::OTHER100 },
				{ "Dome Fossil", Item::OTHER100 },
				{ "Helix Fossil", Item::OTHER100 },
				{ "Old Amber", Item::OTHER100 },
				{ "Rare Bone", Item::OTHER100 },
				{ "Root Fossil", Item::OTHER100 },
				{ "Skull Fossil", Item::OTHER100 }
			} {
		}
		Item::Items find (std::string const & str) const {
			return item.find (str)->second;
		}
};

Item::Items Item::name_from_string (std::string const & str) {
	static Item_From_String const converter;
	return converter.find (str);
}

void Item::set_name_from_string (std::string const & str) {
	name = name_from_string (str);
}
}
