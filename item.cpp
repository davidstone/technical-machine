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

}
