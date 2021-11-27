// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/string.hpp>

#include <stdexcept>

namespace technicalmachine {

auto to_real_hp(HP::max_type const max_hp, VisibleHP const visible_hp) -> AllowedHP {
	if (visible_hp.current == 0_bi) {
		return AllowedHP(0_bi);
	}
	constexpr auto expected_max_visible_hp = 100_bi;
	if (expected_max_visible_hp != visible_hp.max) {
		throw std::runtime_error("Received an invalid max HP. Expected: " + bounded::to_string(expected_max_visible_hp) + " but got " + bounded::to_string(visible_hp.max));
	}
	auto compute_value = [=](auto const visible_current) {
		return ::bounded::assume_in_range<HP::current_type>(max_hp * visible_current / visible_hp.max);
	};
	// TODO: Put in correct bounds on this for the foe Pokemon
	return AllowedHP(
		compute_value(visible_hp.current - 1_bi),
		compute_value(visible_hp.current),
		compute_value(visible_hp.current + 1_bi)
	);
}

} // namespace technicalmachine
