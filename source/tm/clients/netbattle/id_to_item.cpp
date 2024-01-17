// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.nb.id_to_item;

import tm.item;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine::nb {
using namespace bounded::literal;

export using ItemID = bounded::integer<0, 127>;
export constexpr auto id_to_item(ItemID const id, bool const prefer_gen_4_item) -> Item {
	switch (id.value()) {
		case 0: return !prefer_gen_4_item ? Item::None : Item::Micle_Berry;

		// Generation 2 mostly
		case 1: return !prefer_gen_4_item ? Item::Berry : Item::Enigma_Berry;
		case 2: return !prefer_gen_4_item ? Item::Berry_Juice : Item::Rowap_Berry;
		case 3: return !prefer_gen_4_item ? Item::Bitter_Berry : Item::Jaboca_Berry;
		case 4: return !prefer_gen_4_item ? Item::Burnt_Berry : Item::Babiri_Berry;
		case 5: return !prefer_gen_4_item ? Item::Gold_Berry : Item::Charti_Berry;
		case 6: return !prefer_gen_4_item ? Item::Ice_Berry : Item::Chilan_Berry;
		case 7: return !prefer_gen_4_item ? Item::Mint_Berry : Item::Chople_Berry;
		case 8: return !prefer_gen_4_item ? Item::MiracleBerry : Item::Coba_Berry;
		case 9: return !prefer_gen_4_item ? Item::MysteryBerry : Item::Colbur_Berry;
		case 10: return !prefer_gen_4_item ? Item::PRZCureBerry : Item::Haban_Berry;
		case 11: return !prefer_gen_4_item ? Item::PSNCureBerry : Item::Kasib_Berry;
		case 12: return !prefer_gen_4_item ? Item::Berserk_Gene : Item::Kebia_Berry;
		case 13: return !prefer_gen_4_item ? Item::Black_Belt : Item::Occa_Berry;
		case 14: return !prefer_gen_4_item ? Item::Black_Glasses : Item::Passho_Berry;
		case 15: return !prefer_gen_4_item ? Item::Bright_Powder : Item::Payapa_Berry;
		case 16: return !prefer_gen_4_item ? Item::Charcoal : Item::Rindo_Berry;
		case 17: return !prefer_gen_4_item ? Item::Dragon_Fang : Item::Shuca_Berry;
		case 18: return !prefer_gen_4_item ? Item::Focus_Band : Item::Tanga_Berry;
		case 19: return !prefer_gen_4_item ? Item::Hard_Stone : Item::Wacan_Berry;
		case 20: return !prefer_gen_4_item ? Item::Kings_Rock : Item::Yache_Berry;
		case 21: return !prefer_gen_4_item ? Item::Leftovers : Item::Flame_Plate;
		case 22: return !prefer_gen_4_item ? Item::Light_Ball : Item::Griseous_Orb;
		case 23: return Item::Lucky_Punch;
		case 24: return Item::Magnet;
		case 25: return Item::Metal_Coat;
		case 26: return Item::Metal_Powder;
		case 27: return Item::Miracle_Seed;
		case 28: return Item::Mystic_Water;
		case 29: return Item::Never_Melt_Ice;
		case 30: return Item::Pink_Bow;
		case 31: return Item::Poison_Barb;
		case 32: return Item::Polkadot_Bow;
		case 33: return Item::Quick_Claw;
		case 34: return Item::Scope_Lens;
		case 35: return Item::Sharp_Beak;
		case 36: return Item::Silver_Powder;
		case 37: return Item::Soft_Sand;
		case 38: return Item::Spell_Tag;
		case 39: return Item::Stick;
		case 40: return Item::Thick_Club;
		case 41: return Item::Twisted_Spoon;

		// Generation 3
		case 42: return Item::Cheri_Berry;
		case 43: return Item::Chesto_Berry;
		case 44: return Item::Pecha_Berry;
		case 45: return Item::Rawst_Berry;
		case 46: return Item::Aspear_Berry;
		case 47: return Item::Leppa_Berry;
		case 48: return Item::Oran_Berry;
		case 49: return Item::Persim_Berry;
		case 50: return Item::Lum_Berry;
		case 51: return Item::Sitrus_Berry;
		case 52: return Item::Figy_Berry;
		case 53: return Item::Iapapa_Berry;
		case 54: return Item::Mago_Berry;
		case 55: return Item::Wiki_Berry;
		case 56: return Item::Aguav_Berry;
		case 57: return Item::Liechi_Berry;
		case 58: return Item::Ganlon_Berry;
		case 59: return Item::Salac_Berry;
		case 60: return Item::Petaya_Berry;
		case 61: return Item::Apicot_Berry;
		case 62: return Item::Lansat_Berry;
		case 63: return Item::Starf_Berry;
		case 64: return Item::Choice_Band;
		case 65: return Item::Deep_Sea_Scale;
		case 66: return Item::Deep_Sea_Tooth;
		case 67: return Item::Lax_Incense;
		case 68: return Item::Macho_Brace;
		case 69: return Item::Mental_Herb;
		case 70: return Item::Sea_Incense;
		case 71: return Item::Shell_Bell;
		case 72: return Item::Silk_Scarf;
		case 73: return Item::Soul_Dew;
		case 74: return Item::White_Herb;

		// Generation 4 mostly
		case 75: return Item::Adamant_Orb;
		case 76: return Item::Big_Root;
		case 77: return Item::Choice_Scarf;
		case 78: return Item::Choice_Specs;
		case 79: return Item::Damp_Rock;
		case 80: return Item::Destiny_Knot;
		case 81: return Item::Draco_Plate;
		case 82: return Item::Dread_Plate;
		case 83: return Item::Earth_Plate;
		case 84: return Item::Expert_Belt;
		case 85: return Item::Fist_Plate;
		case 86: return Item::Flame_Orb;
		case 87: return Item::Focus_Sash;
		case 88: return Item::Full_Incense;
		case 89: return Item::Grip_Claw;
		case 90: return Item::Heat_Rock;
		case 91: return Item::Icicle_Plate;
		case 92: return Item::Icy_Rock;
		case 93: return Item::Insect_Plate;
		case 94: return Item::Iron_Ball;
		case 95: return Item::Iron_Plate;
		case 96: return Item::Lagging_Tail;
		case 97: return Item::Life_Orb;
		case 98: return Item::Light_Clay;
		case 99: return Item::Lustrous_Orb;
		case 100: return Item::Meadow_Plate;
		case 101: return Item::Metronome;
		case 102: return Item::Mind_Plate;
		case 103: return Item::Muscle_Band;
		case 104: return Item::Odd_Incense;
		case 105: return Item::Power_Herb;
		case 106: return Item::Quick_Powder;
		case 107: return Item::Razor_Claw;
		case 108: return Item::Razor_Fang;
		case 109: return Item::Rock_Incense;
		case 110: return Item::Rose_Incense;
		case 111: return Item::Shed_Shell;
		case 112: return Item::Shoal_Salt;
		case 113: return Item::Sky_Plate;
		case 114: return Item::Smooth_Rock;
		case 115: return Item::Splash_Plate;
		case 116: return Item::Spooky_Plate;
		case 117: return Item::Sticky_Barb;
		case 118: return Item::Stone_Plate;
		case 119: return Item::Toxic_Orb;
		case 120: return Item::Toxic_Plate;
		case 121: return Item::Wave_Incense;
		case 122: return Item::Wide_Lens;
		case 123: return Item::Wise_Glasses;
		case 124: return Item::Zap_Plate;
		case 125: return Item::Zoom_Lens;
		case 126: return Item::Black_Sludge;
		case 127: return Item::Custap_Berry;

		default:
			static_assert(numeric_traits::max_value<decltype(id)> == 127_bi);
			std::unreachable();
	}
}

} // namespace technicalmachine::nb