// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/status.hpp>

#include <cstdint>

namespace technicalmachine {

enum class ContactAbilityEffect : std::uint8_t {
	nothing,
	burn,
	infatuation,
	paralysis,
	poison,
	sleep
};

constexpr auto status_to_contact_ability_effect(StatusName const status) {
	switch (status) {
		case StatusName::burn: return ContactAbilityEffect::burn;
		case StatusName::paralysis: return ContactAbilityEffect::paralysis;
		case StatusName::poison: return ContactAbilityEffect::poison;
		case StatusName::sleep: return ContactAbilityEffect::sleep;
		default: std::unreachable();
	}
}

} // namespace technicalmachine
