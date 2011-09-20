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

#include <map>
#include <string>
#include "item.h"

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
		case POWER_ITEMS:
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

std::string Item::get_name () const {
	static std::string const item_name [] = { "No Item", "Adamant Orb ", "Aguav Berry", "Apicot Berry", "Aspear Berry", "Babiri Berry", "Belue Berry", "Berry Juice", "Big Root", "Black Belt", "Black Sludge", "BlackGlasses", "Bluk Berry", "BrightPowder", "Charcoal", "Charti Berry", "Cheri Berry", "Chesto Berry", "Chilan Berry", "Choice Band", "Choice Scarf", "Choice Specs", "Chople Berry", "Coba Berry", "Colbur Berry", "Cornn Berry", "Custap Berry", "Damp Rock", "DeepSeaScale", "DeepSeaTooth", "Destiny Knot", "Draco Plate", "Dragon Fang", "Dread Plate", "Durin Berry", "Earth Plate", "Enigma Berry", "Expert Belt", "Figy Berry", "Fist Plate", "Flame Orb", "Flame Plate", "Focus Band", "Focus Sash", "Ganlon Berry", "Grepa Berry", "Grip Claw", "Griseous Orb", "Haban Berry", "Hard Stone", "Heat Rock", "Hondew Berry", "Iapapa Berry", "Icicle Plate", "Icy Rock", "Insect Plate", "Iron Ball", "Iron Plate", "Jaboca Berry", "Kasib Berry", "Kebia Berry", "Kelpsy Berry", "King's Rock", "Lagging Tail", "Lansat Berry", "Lax Incense", "Leftovers", "Leppa Berry", "Liechi Berry", "Life Orb", "Light Ball", "Light Clay", "Lucky Punch", "Lum Berry", "Lustrous Orb", "Macho Brace", "Magnet", "Mago Berry", "Magost Berry", "Mail", "Meadow Plate", "Mental Herb", "Metal Coat", "Metal Powder", "Metronome", "Micle Berry", "Mind Plate", "Miracle Seed", "Muscle Band", "Mystic Water", "Nanab Berry", "NeverMeltIce", "Nomel Berry", "Occa Berry", "Odd Incense", "Oran Berry", "Pamtre Berry", "Passho Berry", "Payapa Berry", "Pecha Berry", "Persim Berry", "Petaya Berry", "Pinap Berry", "Poison Barb", "Pomeg Berry", "Power Herb", "Power Item", "Qualot Berry", "Quick Claw", "Quick Powder", "Rabuta Berry", "Rawst Berry", "Razor Claw", "Razz Berry", "Rindo Berry", "Rock Incense", "Rose Incense", "Rowap Berry", "Salac Berry", "Scope Lens", "Sharp Beak", "Shed Shell", "Shell Bell", "Shuca Berry", "Silk Scarf", "SilverPowder", "Sitrus Berry", "Sky Plate", "Smooth Rock", "Soft Sand", "Soul Dew", "Spell Tag", "Spelon Berry", "Splash Plate", "Spooky Plate", "Starf Berry", "Stick", "Sticky Barb", "Stone Plate", "Tamato Berry", "Tanga Berry", "Thick Club", "Toxic Orb", "Toxic Plate", "TwistedSpoon", "Wacan Berry", "Watmel Berry", "Wave Incense", "Wepear Berry", "White Herb", "Wide Lens", "Wiki Berry", "Wise Glasses", "Yache Berry", "Zap Plate", "Zoom Lens", "Other", "Other10", "Other30", "Other50", "Other80", "Other100", "End Item" };
	return item_name [name];
}

class Map {
	public:
		std::map <std::string, Item::Items> item;
		Map () {
			item ["No Item"] = Item::NO_ITEM;
			item ["Adamant Orb "] = Item::ADAMANT_ORB;
			item ["Aguav Berry"] = Item::AGUAV_BERRY;
			item ["Apicot Berry"] = Item::APICOT_BERRY;
			item ["Aspear Berry"] = Item::ASPEAR_BERRY;
			item ["Babiri Berry"] = Item::BABIRI_BERRY;
			item ["Belue Berry"] = Item::BELUE_BERRY;
			item ["Berry Juice"] = Item::BERRY_JUICE;
			item ["Big Root"] = Item::BIG_ROOT;
			item ["Black Belt"] = Item::BLACK_BELT;
			item ["Black Sludge"] = Item::BLACK_SLUDGE;
			item ["BlackGlasses"] = Item::BLACKGLASSES;
			item ["Bluk Berry"] = Item::BLUK_BERRY;
			item ["BrightPowder"] = Item::BRIGHTPOWDER;
			item ["Charcoal"] = Item::CHARCOAL;
			item ["Charti Berry"] = Item::CHARTI_BERRY;
			item ["Cheri Berry"] = Item::CHERI_BERRY;
			item ["Chesto Berry"] = Item::CHESTO_BERRY;
			item ["Chilan Berry"] = Item::CHILAN_BERRY;
			item ["Choice Band"] = Item::CHOICE_BAND;
			item ["Choice Scarf"] = Item::CHOICE_SCARF;
			item ["Choice Specs"] = Item::CHOICE_SPECS;
			item ["Chople Berry"] = Item::CHOPLE_BERRY;
			item ["Coba Berry"] = Item::COBA_BERRY;
			item ["Colbur Berry"] = Item::COLBUR_BERRY;
			item ["Cornn Berry"] = Item::CORNN_BERRY;
			item ["Custap Berry"] = Item::CUSTAP_BERRY;
			item ["Damp Rock"] = Item::DAMP_ROCK;
			item ["DeepSeaScale"] = Item::DEEPSEASCALE;
			item ["DeepSeaTooth"] = Item::DEEPSEATOOTH;
			item ["Destiny Knot"] = Item::DESTINY_KNOT;
			item ["Draco Plate"] = Item::DRACO_PLATE;
			item ["Dragon Fang"] = Item::DRAGON_FANG;
			item ["Dread Plate"] = Item::DREAD_PLATE;
			item ["Durin Berry"] = Item::DURIN_BERRY;
			item ["Earth Plate"] = Item::EARTH_PLATE;
			item ["Enigma Berry"] = Item::ENIGMA_BERRY;
			item ["Expert Belt"] = Item::EXPERT_BELT;
			item ["Figy Berry"] = Item::FIGY_BERRY;
			item ["Fist Plate"] = Item::FIST_PLATE;
			item ["Flame Orb"] = Item::FLAME_ORB;
			item ["Flame Plate"] = Item::FLAME_PLATE;
			item ["Focus Band"] = Item::FOCUS_BAND;
			item ["Focus Sash"] = Item::FOCUS_SASH;
			item ["Ganlon Berry"] = Item::GANLON_BERRY;
			item ["Grepa Berry"] = Item::GREPA_BERRY;
			item ["Grip Claw"] = Item::GRIP_CLAW;
			item ["Griseous Orb"] = Item::GRISEOUS_ORB;
			item ["Haban Berry"] = Item::HABAN_BERRY;
			item ["Hard Stone"] = Item::HARD_STONE;
			item ["Heat Rock"] = Item::HEAT_ROCK;
			item ["Hondew Berry"] = Item::HONDEW_BERRY;
			item ["Iapapa Berry"] = Item::IAPAPA_BERRY;
			item ["Icicle Plate"] = Item::ICICLE_PLATE;
			item ["Icy Rock"] = Item::ICY_ROCK;
			item ["Insect Plate"] = Item::INSECT_PLATE;
			item ["Iron Ball"] = Item::IRON_BALL;
			item ["Iron Plate"] = Item::IRON_PLATE;
			item ["Jaboca Berry"] = Item::JABOCA_BERRY;
			item ["Kasib Berry"] = Item::KASIB_BERRY;
			item ["Kebia Berry"] = Item::KEBIA_BERRY;
			item ["Kelpsy Berry"] = Item::KELPSY_BERRY;
			item ["King's Rock"] = Item::KINGS_ROCK;
			item ["Razor Fang"] = Item::KINGS_ROCK;
			item ["Full Incense"] = Item::LAGGING_TAIL;
			item ["Lagging Tail"] = Item::LAGGING_TAIL;
			item ["Lansat Berry"] = Item::LANSAT_BERRY;
			item ["Lax Incense"] = Item::LAX_INCENSE;
			item ["Leftovers"] = Item::LEFTOVERS;
			item ["Leppa Berry"] = Item::LEPPA_BERRY;
			item ["Liechi Berry"] = Item::LIECHI_BERRY;
			item ["Life Orb"] = Item::LIFE_ORB;
			item ["Light Ball"] = Item::LIGHT_BALL;
			item ["Light Clay"] = Item::LIGHT_CLAY;
			item ["Lucky Punch"] = Item::LUCKY_PUNCH;
			item ["Lum Berry"] = Item::LUM_BERRY;
			item ["Lustrous Orb"] = Item::LUSTROUS_ORB;
			item ["Macho Brace"] = Item::MACHO_BRACE;
			item ["Magnet"] = Item::MAGNET;
			item ["Mago Berry"] = Item::MAGO_BERRY;
			item ["Magost Berry"] = Item::MAGOST_BERRY;
			item ["Mail"] = Item::MAIL;
			item ["Air Mail"] = Item::MAIL;
			item ["Bloom Mail"] = Item::MAIL;
			item ["Brick Mail"] = Item::MAIL;
			item ["Bubble Mail"] = Item::MAIL;
			item ["Flame Mail"] = Item::MAIL;
			item ["Grass Mail"] = Item::MAIL;
			item ["Heart Mail"] = Item::MAIL;
			item ["Mosaic Mail"] = Item::MAIL;
			item ["Snow Mail"] = Item::MAIL;
			item ["Space Mail"] = Item::MAIL;
			item ["Steel Mail"] = Item::MAIL;
			item ["Tunnel Mail"] = Item::MAIL;
			item ["Meadow Plate"] = Item::MEADOW_PLATE;
			item ["Mental Herb"] = Item::MENTAL_HERB;
			item ["Metal Coat"] = Item::METAL_COAT;
			item ["Metal Powder"] = Item::METAL_POWDER;
			item ["Metronome"] = Item::METRONOME;
			item ["Micle Berry"] = Item::MICLE_BERRY;
			item ["Mind Plate"] = Item::MIND_PLATE;
			item ["Miracle Seed"] = Item::MIRACLE_SEED;
			item ["Muscle Band"] = Item::MUSCLE_BAND;
			item ["Mystic Water"] = Item::MYSTIC_WATER;
			item ["Nanab Berry"] = Item::NANAB_BERRY;
			item ["NeverMeltIce"] = Item::NEVERMELTICE;
			item ["Nomel Berry"] = Item::NOMEL_BERRY;
			item ["Occa Berry"] = Item::OCCA_BERRY;
			item ["Odd Incense"] = Item::ODD_INCENSE;
			item ["Oran Berry"] = Item::ORAN_BERRY;
			item ["Pamtre Berry"] = Item::PAMTRE_BERRY;
			item ["Passho Berry"] = Item::PASSHO_BERRY;
			item ["Payapa Berry"] = Item::PAYAPA_BERRY;
			item ["Pecha Berry"] = Item::PECHA_BERRY;
			item ["Persim Berry"] = Item::PERSIM_BERRY;
			item ["Petaya Berry"] = Item::PETAYA_BERRY;
			item ["Pinap Berry"] = Item::PINAP_BERRY;
			item ["Poison Barb"] = Item::POISON_BARB;
			item ["Pomeg Berry"] = Item::POMEG_BERRY;
			item ["Power Herb"] = Item::POWER_HERB;
			item ["Power Item"] = Item::POWER_ITEMS;
			item ["Power Anklet"] = Item::POWER_ITEMS;
			item ["Power Band"] = Item::POWER_ITEMS;
			item ["Power Belt"] = Item::POWER_ITEMS;
			item ["Power Bracer"] = Item::POWER_ITEMS;
			item ["Power Lens"] = Item::POWER_ITEMS;
			item ["Power Weight"] = Item::POWER_ITEMS;
			item ["Qualot Berry"] = Item::QUALOT_BERRY;
			item ["Quick Claw"] = Item::QUICK_CLAW;
			item ["Quick Powder"] = Item::QUICK_POWDER;
			item ["Rabuta Berry"] = Item::RABUTA_BERRY;
			item ["Rawst Berry"] = Item::RAWST_BERRY;
			item ["Razor Claw"] = Item::RAZOR_CLAW;
			item ["Razz Berry"] = Item::RAZZ_BERRY;
			item ["Rindo Berry"] = Item::RINDO_BERRY;
			item ["Rock Incense"] = Item::ROCK_INCENSE;
			item ["Rose Incense"] = Item::ROSE_INCENSE;
			item ["Rowap Berry"] = Item::ROWAP_BERRY;
			item ["Salac Berry"] = Item::SALAC_BERRY;
			item ["Scope Lens"] = Item::SCOPE_LENS;
			item ["Sharp Beak"] = Item::SHARP_BEAK;
			item ["Shed Shell"] = Item::SHED_SHELL;
			item ["Shell Bell"] = Item::SHELL_BELL;
			item ["Shuca Berry"] = Item::SHUCA_BERRY;
			item ["Silk Scarf"] = Item::SILK_SCARF;
			item ["SilverPowder"] = Item::SILVERPOWDER;
			item ["Sitrus Berry"] = Item::SITRUS_BERRY;
			item ["Sky Plate"] = Item::SKY_PLATE;
			item ["Smooth Rock"] = Item::SMOOTH_ROCK;
			item ["Soft Sand"] = Item::SOFT_SAND;
			item ["Soul Dew"] = Item::SOUL_DEW;
			item ["Spell Tag"] = Item::SPELL_TAG;
			item ["Spelon Berry"] = Item::SPELON_BERRY;
			item ["Splash Plate"] = Item::SPLASH_PLATE;
			item ["Spooky Plate"] = Item::SPOOKY_PLATE;
			item ["Starf Berry"] = Item::STARF_BERRY;
			item ["Stick"] = Item::STICK;
			item ["Sticky Barb"] = Item::STICKY_BARB;
			item ["Stone Plate"] = Item::STONE_PLATE;
			item ["Tamato Berry"] = Item::TAMATO_BERRY;
			item ["Tanga Berry"] = Item::TANGA_BERRY;
			item ["Thick Club"] = Item::THICK_CLUB;
			item ["Toxic Orb"] = Item::TOXIC_ORB;
			item ["Toxic Plate"] = Item::TOXIC_PLATE;
			item ["TwistedSpoon"] = Item::TWISTEDSPOON;
			item ["Wacan Berry"] = Item::WACAN_BERRY;
			item ["Watmel Berry"] = Item::WATMEL_BERRY;
			item ["Sea Incense"] = Item::WAVE_INCENSE;
			item ["Wave Incense"] = Item::WAVE_INCENSE;
			item ["Wepear Berry"] = Item::WEPEAR_BERRY;
			item ["White Herb"] = Item::WHITE_HERB;
			item ["Wide Lens"] = Item::WIDE_LENS;
			item ["Wiki Berry"] = Item::WIKI_BERRY;
			item ["Wise Glasses"] = Item::WISE_GLASSES;
			item ["Yache Berry"] = Item::YACHE_BERRY;
			item ["Zap Plate"] = Item::ZAP_PLATE;
			item ["Zoom Lens"] = Item::ZOOM_LENS;
			item ["Other"] = Item::OTHER;
			item ["Cherish Ball"] = Item::OTHER;
			item ["Dive Ball"] = Item::OTHER;
			item ["Dusk Ball"] = Item::OTHER;
			item ["Great Ball"] = Item::OTHER;
			item ["Heal Ball"] = Item::OTHER;
			item ["Luxury Ball"] = Item::OTHER;
			item ["Master Ball"] = Item::OTHER;
			item ["Nest Ball"] = Item::OTHER;
			item ["Net Ball"] = Item::OTHER;
			item ["Poke Ball"] = Item::OTHER;
			item ["Premier Ball"] = Item::OTHER;
			item ["Quick Ball"] = Item::OTHER;
			item ["Repeat Ball"] = Item::OTHER;
			item ["Timer Ball"] = Item::OTHER;
			item ["TM01"] = Item::OTHER;
			item ["TM02"] = Item::OTHER;
			item ["TM03"] = Item::OTHER;
			item ["TM04"] = Item::OTHER;
			item ["TM05"] = Item::OTHER;
			item ["TM06"] = Item::OTHER;
			item ["TM07"] = Item::OTHER;
			item ["TM08"] = Item::OTHER;
			item ["TM09"] = Item::OTHER;
			item ["TM10"] = Item::OTHER;
			item ["TM11"] = Item::OTHER;
			item ["TM12"] = Item::OTHER;
			item ["TM13"] = Item::OTHER;
			item ["TM14"] = Item::OTHER;
			item ["TM15"] = Item::OTHER;
			item ["TM16"] = Item::OTHER;
			item ["TM17"] = Item::OTHER;
			item ["TM18"] = Item::OTHER;
			item ["TM19"] = Item::OTHER;
			item ["TM20"] = Item::OTHER;
			item ["TM21"] = Item::OTHER;
			item ["TM22"] = Item::OTHER;
			item ["TM23"] = Item::OTHER;
			item ["TM24"] = Item::OTHER;
			item ["TM25"] = Item::OTHER;
			item ["TM26"] = Item::OTHER;
			item ["TM27"] = Item::OTHER;
			item ["TM28"] = Item::OTHER;
			item ["TM29"] = Item::OTHER;
			item ["TM30"] = Item::OTHER;
			item ["TM31"] = Item::OTHER;
			item ["TM32"] = Item::OTHER;
			item ["TM33"] = Item::OTHER;
			item ["TM34"] = Item::OTHER;
			item ["TM35"] = Item::OTHER;
			item ["TM36"] = Item::OTHER;
			item ["TM37"] = Item::OTHER;
			item ["TM38"] = Item::OTHER;
			item ["TM39"] = Item::OTHER;
			item ["TM40"] = Item::OTHER;
			item ["TM41"] = Item::OTHER;
			item ["TM42"] = Item::OTHER;
			item ["TM43"] = Item::OTHER;
			item ["TM44"] = Item::OTHER;
			item ["TM45"] = Item::OTHER;
			item ["TM46"] = Item::OTHER;
			item ["TM47"] = Item::OTHER;
			item ["TM48"] = Item::OTHER;
			item ["TM49"] = Item::OTHER;
			item ["TM50"] = Item::OTHER;
			item ["TM51"] = Item::OTHER;
			item ["TM52"] = Item::OTHER;
			item ["TM53"] = Item::OTHER;
			item ["TM54"] = Item::OTHER;
			item ["TM55"] = Item::OTHER;
			item ["TM56"] = Item::OTHER;
			item ["TM57"] = Item::OTHER;
			item ["TM58"] = Item::OTHER;
			item ["TM59"] = Item::OTHER;
			item ["TM60"] = Item::OTHER;
			item ["TM61"] = Item::OTHER;
			item ["TM62"] = Item::OTHER;
			item ["TM63"] = Item::OTHER;
			item ["TM64"] = Item::OTHER;
			item ["TM65"] = Item::OTHER;
			item ["TM66"] = Item::OTHER;
			item ["TM67"] = Item::OTHER;
			item ["TM68"] = Item::OTHER;
			item ["TM69"] = Item::OTHER;
			item ["TM70"] = Item::OTHER;
			item ["TM71"] = Item::OTHER;
			item ["TM72"] = Item::OTHER;
			item ["TM73"] = Item::OTHER;
			item ["TM74"] = Item::OTHER;
			item ["TM75"] = Item::OTHER;
			item ["TM76"] = Item::OTHER;
			item ["TM77"] = Item::OTHER;
			item ["TM78"] = Item::OTHER;
			item ["TM79"] = Item::OTHER;
			item ["TM80"] = Item::OTHER;
			item ["TM81"] = Item::OTHER;
			item ["TM82"] = Item::OTHER;
			item ["TM83"] = Item::OTHER;
			item ["TM84"] = Item::OTHER;
			item ["TM85"] = Item::OTHER;
			item ["TM86"] = Item::OTHER;
			item ["TM87"] = Item::OTHER;
			item ["TM88"] = Item::OTHER;
			item ["TM89"] = Item::OTHER;
			item ["TM90"] = Item::OTHER;
			item ["TM91"] = Item::OTHER;
			item ["TM92"] = Item::OTHER;
			item ["Ultra Ball"] = Item::OTHER;
			item ["Other10"] = Item::OTHER10;
			item ["Blue Scarf"] = Item::OTHER10;
			item ["Green Scarf"] = Item::OTHER10;
			item ["Luck Incense"] = Item::OTHER10;
			item ["Pink Scarf"] = Item::OTHER10;
			item ["Pure Incense"] = Item::OTHER10;
			item ["Reaper Cloth"] = Item::OTHER10;
			item ["Red Scarf"] = Item::OTHER10;
			item ["Soothe Bell"] = Item::OTHER10;
			item ["Yellow Scarf"] = Item::OTHER10;
			item ["Other30"] = Item::OTHER30;
			item ["Amulet Coin"] = Item::OTHER30;
			item ["Antidote"] = Item::OTHER30;
			item ["Awakening"] = Item::OTHER30;
			item ["Big Mushroom"] = Item::OTHER30;
			item ["Big Pearl"] = Item::OTHER30;
			item ["Black Flute"] = Item::OTHER30;
			item ["Blue Flute"] = Item::OTHER30;
			item ["Blue Shard"] = Item::OTHER30;
			item ["Burn Heal"] = Item::OTHER30;
			item ["Calcium"] = Item::OTHER30;
			item ["Carbos"] = Item::OTHER30;
			item ["Cleanse Tag"] = Item::OTHER30;
			item ["Damp Mulch"] = Item::OTHER30;
			item ["Dire Hit"] = Item::OTHER30;
			item ["Dragon Scale"] = Item::OTHER30;
			item ["Elixir"] = Item::OTHER30;
			item ["Energy Root"] = Item::OTHER30;
			item ["EnergyPowder"] = Item::OTHER30;
			item ["Escape Rope"] = Item::OTHER30;
			item ["Ether"] = Item::OTHER30;
			item ["Everstone"] = Item::OTHER30;
			item ["Exp. Share"] = Item::OTHER30;
			item ["Fire Stone"] = Item::OTHER30;
			item ["Fluffy Tail"] = Item::OTHER30;
			item ["Fresh Water"] = Item::OTHER30;
			item ["Full Heal"] = Item::OTHER30;
			item ["Full Restore"] = Item::OTHER30;
			item ["Gooey Mulch"] = Item::OTHER30;
			item ["Green Shard"] = Item::OTHER30;
			item ["Growth Mulch"] = Item::OTHER30;
			item ["Guard Spec."] = Item::OTHER30;
			item ["Heal Powder"] = Item::OTHER30;
			item ["Heart Scale"] = Item::OTHER30;
			item ["Honey"] = Item::OTHER30;
			item ["HP Up"] = Item::OTHER30;
			item ["Hyper Potion"] = Item::OTHER30;
			item ["Ice Heal"] = Item::OTHER30;
			item ["Iron"] = Item::OTHER30;
			item ["Lava Cookie"] = Item::OTHER30;
			item ["Leaf Stone"] = Item::OTHER30;
			item ["Lemonade"] = Item::OTHER30;
			item ["Lucky Egg"] = Item::OTHER30;
			item ["Max Elixir"] = Item::OTHER30;
			item ["Max Ether"] = Item::OTHER30;
			item ["Max Potion"] = Item::OTHER30;
			item ["Max Repel"] = Item::OTHER30;
			item ["Max Revive"] = Item::OTHER30;
			item ["MooMoo Milk"] = Item::OTHER30;
			item ["Moon Stone"] = Item::OTHER30;
			item ["Nugget"] = Item::OTHER30;
			item ["Old Gateau"] = Item::OTHER30;
			item ["Parlyz Heal"] = Item::OTHER30;
			item ["Pearl"] = Item::OTHER30;
			item ["Poke Doll"] = Item::OTHER30;
			item ["Potion"] = Item::OTHER30;
			item ["PP Max"] = Item::OTHER30;
			item ["PP Up"] = Item::OTHER30;
			item ["Protein"] = Item::OTHER30;
			item ["Rare Candy"] = Item::OTHER30;
			item ["Red Flute"] = Item::OTHER30;
			item ["Red Shard"] = Item::OTHER30;
			item ["Repel"] = Item::OTHER30;
			item ["Revival Herb"] = Item::OTHER30;
			item ["Revive"] = Item::OTHER30;
			item ["Sacred Ash"] = Item::OTHER30;
			item ["Shoal Salt"] = Item::OTHER30;
			item ["Shoal Shell"] = Item::OTHER30;
			item ["Smoke Ball"] = Item::OTHER30;
			item ["Soda Pop"] = Item::OTHER30;
			item ["Stable Mulch"] = Item::OTHER30;
			item ["Star Piece"] = Item::OTHER30;
			item ["Stardust"] = Item::OTHER30;
			item ["Sun Stone"] = Item::OTHER30;
			item ["Super Potion"] = Item::OTHER30;
			item ["Super Repel"] = Item::OTHER30;
			item ["Thunderstone"] = Item::OTHER30;
			item ["TinyMushroom"] = Item::OTHER30;
			item ["Up-Grade"] = Item::OTHER30;
			item ["Water Stone"] = Item::OTHER30;
			item ["White Flute"] = Item::OTHER30;
			item ["X Accuracy"] = Item::OTHER30;
			item ["X Attack"] = Item::OTHER30;
			item ["X Defend"] = Item::OTHER30;
			item ["X Sp. Def"] = Item::OTHER30;
			item ["X Special"] = Item::OTHER30;
			item ["X Speed"] = Item::OTHER30;
			item ["Yellow Flute"] = Item::OTHER30;
			item ["Yellow Shard"] = Item::OTHER30;
			item ["Zinc"] = Item::OTHER30;
			item ["Other50"] = Item::OTHER50;
			item ["Dubious Disc"] = Item::OTHER50;
			item ["Other80"] = Item::OTHER80;
			item ["Dawn Stone"] = Item::OTHER80;
			item ["Dusk Stone"] = Item::OTHER80;
			item ["Electirizer"] = Item::OTHER80;
			item ["Magmarizer"] = Item::OTHER80;
			item ["Odd Keystone"] = Item::OTHER80;
			item ["Oval Stone"] = Item::OTHER80;
			item ["Protector"] = Item::OTHER80;
			item ["Shiny Stone"] = Item::OTHER80;
			item ["Other100"] = Item::OTHER100;
			item ["Armor Fossil"] = Item::OTHER100;
			item ["Claw Fossil"] = Item::OTHER100;
			item ["Dome Fossil"] = Item::OTHER100;
			item ["Helix Fossil"] = Item::OTHER100;
			item ["Old Amber"] = Item::OTHER100;
			item ["Rare Bone"] = Item::OTHER100;
			item ["Root Fossil"] = Item::OTHER100;
			item ["Skull Fossil"] = Item::OTHER100;
		}
		Item::Items find (std::string const & str) const {
			return item.find (str)->second;
		}
};

Item::Items Item::name_from_string (std::string const & str) {
	static Map const map;
	return map.find (str);
}

void Item::set_name_from_string (std::string const & str) {
	name = name_from_string (str);
}
}
