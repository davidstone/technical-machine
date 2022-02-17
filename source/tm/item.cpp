// Item functions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/item.hpp>

namespace technicalmachine {
using namespace bounded::literal;

bounded::integer<0, 80> berry_power(Item const item) {
	switch (item) {
		case Item::Aguav_Berry:
		case Item::Aspear_Berry:
		case Item::Babiri_Berry:
		case Item::Charti_Berry:
		case Item::Cheri_Berry:
		case Item::Chesto_Berry:
		case Item::Chilan_Berry:
		case Item::Chople_Berry:
		case Item::Coba_Berry:
		case Item::Colbur_Berry:
		case Item::Figy_Berry:
		case Item::Haban_Berry:
		case Item::Iapapa_Berry:
		case Item::Kasib_Berry:
		case Item::Kebia_Berry:
		case Item::Leppa_Berry:
		case Item::Lum_Berry:
		case Item::Mago_Berry:
		case Item::Occa_Berry:
		case Item::Oran_Berry:
		case Item::Passho_Berry:
		case Item::Payapa_Berry:
		case Item::Pecha_Berry:
		case Item::Persim_Berry:
		case Item::Rawst_Berry:
		case Item::Razz_Berry:
		case Item::Rindo_Berry:
		case Item::Shuca_Berry:
		case Item::Sitrus_Berry:
		case Item::Tanga_Berry:
		case Item::Wacan_Berry:
		case Item::Wiki_Berry:
		case Item::Yache_Berry:
			return 60_bi;
		case Item::Bluk_Berry:
		case Item::Cornn_Berry:
		case Item::Grepa_Berry:
		case Item::Hondew_Berry:
		case Item::Kelpsy_Berry:
		case Item::Magost_Berry:
		case Item::Nanab_Berry:
		case Item::Nomel_Berry:
		case Item::Pamtre_Berry:
		case Item::Pinap_Berry:
		case Item::Pomeg_Berry:
		case Item::Qualot_Berry:
		case Item::Rabuta_Berry:
		case Item::Spelon_Berry:
		case Item::Tamato_Berry:
		case Item::Wepear_Berry:
			return 70_bi;
		case Item::Apicot_Berry:
		case Item::Belue_Berry:
		case Item::Custap_Berry:
		case Item::Durin_Berry:
		case Item::Enigma_Berry:
		case Item::Ganlon_Berry:
		case Item::Jaboca_Berry:
		case Item::Lansat_Berry:
		case Item::Liechi_Berry:
		case Item::Micle_Berry:
		case Item::Petaya_Berry:
		case Item::Rowap_Berry:
		case Item::Salac_Berry:
		case Item::Starf_Berry:
		case Item::Watmel_Berry:
			return 80_bi;
		default:
			return 0_bi;
	}
}

} // namespace technicalmachine
