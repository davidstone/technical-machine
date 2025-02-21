// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <string>

export module tm.stat.to_real_hp;

import tm.stat.allowed_hp;
import tm.stat.current_hp;
import tm.stat.max_hp;

import tm.visible_hp;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

export constexpr auto to_real_hp(MaxHP const max_hp, VisibleHP const visible_hp) -> AllowedHP {
	if (visible_hp.current == CurrentVisibleHP(0_bi)) {
		return AllowedHP(0_bi);
	}
	auto compute_value = [=](auto const visible_current) {
		return bounded::assume_in_range<CurrentHP>(max_hp * visible_current / visible_hp.max.value());
	};
	// TODO: Put in correct bounds on this for the foe Pokemon
	return AllowedHP(
		compute_value(visible_hp.current.value() - 1_bi),
		compute_value(visible_hp.current.value()),
		compute_value(bounded::min(visible_hp.current.value() + 1_bi, visible_hp.max.value()))
	);
}

} // namespace technicalmachine
