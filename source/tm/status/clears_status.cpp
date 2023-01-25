// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.status.clears_status;

import tm.status.status_name;

import tm.ability;
import tm.item;

namespace technicalmachine {

export constexpr auto clears_status(Item const item, StatusName const status) -> bool {
	switch (item) {
		case Item::Ice_Berry:
		case Item::Rawst_Berry:
			return status == StatusName::burn;
		case Item::Aspear_Berry:
		case Item::Burnt_Berry:
			return status == StatusName::freeze;
		case Item::Cheri_Berry:
		case Item::PRZCureBerry:
			return status == StatusName::paralysis;
		case Item::Chesto_Berry:
		case Item::Mint_Berry:
			return status == StatusName::rest or status == StatusName::sleep;
		case Item::Pecha_Berry:
		case Item::PSNCureBerry:
			return status == StatusName::poison or status == StatusName::toxic;
		case Item::Lum_Berry:
		case Item::MiracleBerry:
			return true;
		default:
			return false;
	}
}

export constexpr bool can_clear_status(Ability const ability, StatusName const status) {
	return ability == Ability::Shed_Skin and status != StatusName::clear;
}

} // namespace technicalmachine
