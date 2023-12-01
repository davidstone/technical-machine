// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.status.check_ability_can_status;

import tm.status.status_name;

import tm.string_conversions.ability;
import tm.string_conversions.status_name;

import tm.ability;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto ability_statuses(Ability const ability) -> containers::static_vector<StatusName, 4_bi> {
	switch (ability) {
		case Ability::Effect_Spore: return {{StatusName::paralysis, StatusName::poison, StatusName::sleep}};
		case Ability::Flame_Body: return {{StatusName::burn}};
		case Ability::Poison_Point: return {{StatusName::poison}};
		case Ability::Static: return {{StatusName::paralysis}};
		case Ability::Synchronize: return {{StatusName::burn, StatusName::freeze, StatusName::paralysis, StatusName::poison}};
		default: return {};
	}
}

export constexpr auto check_ability_can_status(Ability const ability, StatusName const status) -> void {
	if (!containers::any_equal(ability_statuses(ability), status)) {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Tried to apply "sv,
			to_string(status),
			" as a result of a contact move against "sv,
			to_string(ability)
		));
	}
}

} // namespace technicalmachine