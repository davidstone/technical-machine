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

#ifndef ITEM_H_
#define ITEM_H_

#include <string>

namespace technicalmachine {

// OTHER represents those items that can be held, but otherwise have no competitive effect. These items cannot be used with Fling. The OTHER items followed by a number represent those items that can be held and used with Fling (with the number representing Fling's power used with that item), but otherwise have no competitive effect. Replace with the real thing later.

class Item {
	public:
		enum Items { NO_ITEM, ADAMANT_ORB, AGUAV_BERRY, AIR_MAIL, APICOT_BERRY, ASPEAR_BERRY, BABIRI_BERRY, BELUE_BERRY, BERRY_JUICE, BIG_ROOT, BLACK_BELT, BLACK_SLUDGE, BLACKGLASSES, BLOOM_MAIL, BLUK_BERRY, BRICK_MAIL, BRIGHTPOWDER, BUBBLE_MAIL, CHARCOAL, CHARTI_BERRY, CHERI_BERRY, CHESTO_BERRY, CHILAN_BERRY, CHOICE_BAND, CHOICE_SCARF, CHOICE_SPECS, CHOPLE_BERRY, COBA_BERRY, COLBUR_BERRY, CORNN_BERRY, CUSTAP_BERRY, DAMP_ROCK, DEEPSEASCALE, DEEPSEATOOTH, DESTINY_KNOT, DRACO_PLATE, DRAGON_FANG, DREAD_PLATE, DURIN_BERRY, EARTH_PLATE, ENIGMA_BERRY, EXPERT_BELT, FIGY_BERRY, FIST_PLATE, FLAME_MAIL, FLAME_ORB, FLAME_PLATE, FOCUS_BAND, FOCUS_SASH, FULL_INCENSE, GANLON_BERRY, GRASS_MAIL, GREPA_BERRY, GRIP_CLAW, GRISEOUS_ORB, HABAN_BERRY, HARD_STONE, HEART_MAIL, HEAT_ROCK, HONDEW_BERRY, IAPAPA_BERRY, ICICLE_PLATE, ICY_ROCK, INSECT_PLATE, IRON_BALL, IRON_PLATE, JABOCA_BERRY, KASIB_BERRY, KEBIA_BERRY, KELPSY_BERRY, KINGS_ROCK, LAGGING_TAIL, LANSAT_BERRY, LAX_INCENSE, LEFTOVERS, LEPPA_BERRY, LIECHI_BERRY, LIFE_ORB, LIGHT_BALL, LIGHT_CLAY, LUCKY_PUNCH, LUM_BERRY, LUSTROUS_ORB, MACHO_BRACE, MAGNET, MAGO_BERRY, MAGOST_BERRY, MEADOW_PLATE, MENTAL_HERB, METAL_COAT, METAL_POWDER, METRONOME, MICLE_BERRY, MIND_PLATE, MIRACLE_SEED, MOSAIC_MAIL, MUSCLE_BAND, MYSTIC_WATER, NANAB_BERRY, NEVERMELTICE, NOMEL_BERRY, OCCA_BERRY, ODD_INCENSE, ORAN_BERRY, PAMTRE_BERRY, PASSHO_BERRY, PAYAPA_BERRY, PECHA_BERRY, PERSIM_BERRY, PETAYA_BERRY, PINAP_BERRY, POISON_BARB, POMEG_BERRY, POWER_ANKLET, POWER_BAND, POWER_BELT, POWER_BRACER, POWER_HERB, POWER_LENS, POWER_WEIGHT, QUALOT_BERRY, QUICK_CLAW, QUICK_POWDER, RABUTA_BERRY, RAWST_BERRY, RAZOR_CLAW, RAZOR_FANG, RAZZ_BERRY, RINDO_BERRY, ROCK_INCENSE, ROSE_INCENSE, ROWAP_BERRY, SALAC_BERRY, SCOPE_LENS, SEA_INCENSE, SHARP_BEAK, SHED_SHELL, SHELL_BELL, SHUCA_BERRY, SILK_SCARF, SILVERPOWDER, SITRUS_BERRY, SKY_PLATE, SMOOTH_ROCK, SNOW_MAIL, SOFT_SAND, SOUL_DEW, SPACE_MAIL, SPELL_TAG, SPELON_BERRY, SPLASH_PLATE, SPOOKY_PLATE, STARF_BERRY, STEEL_MAIL, STICK, STICKY_BARB, STONE_PLATE, TAMATO_BERRY, TANGA_BERRY, THICK_CLUB, TOXIC_ORB, TOXIC_PLATE, TUNNEL_MAIL, TWISTEDSPOON, WACAN_BERRY, WATMEL_BERRY, WAVE_INCENSE, WEPEAR_BERRY, WHITE_HERB, WIDE_LENS, WIKI_BERRY, WISE_GLASSES, YACHE_BERRY, ZAP_PLATE, ZOOM_LENS, OTHER, OTHER10, OTHER30, OTHER50, OTHER80, OTHER100, END_ITEM };
	
		Items name;
	
		Item ();
		Item (Items item);
		bool is_set () const;
		bool is_choice_item () const;
		int get_berry_power () const;		// Returns 0 for non-berries
		int get_fling_power () const;
		bool blocks_trick () const;
		std::string get_name () const;
		static Items name_from_string (std::string const & str);
		void set_name_from_string (std::string const & str);
};

}
#endif		// ITEM_H_
