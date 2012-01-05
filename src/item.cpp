// Item functions
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

namespace technicalmachine {

Item::Item ():
	name (END) {
}

Item::Item (Items item):
	name (item) {
}

bool Item::is_set () const {
	return name != END;
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

}	// namespace technicalmachine
