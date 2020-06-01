// Copyright (C) 2020 David Stone
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

#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/string.hpp>

#include <stdexcept>

namespace technicalmachine {

auto to_real_hp(bool const is_ai, HP const actual_hp, VisibleHP const visible_hp) -> AllowedHP {
	if (visible_hp.current == 0_bi) {
		return AllowedHP{0_bi, 0_bi, 0_bi};
	}
	constexpr auto max_visible_foe_hp = 100_bi;
	auto const max_hp = actual_hp.max();
	auto const expected_max_visible_hp = BOUNDED_CONDITIONAL(is_ai, max_hp, max_visible_foe_hp);
	if (expected_max_visible_hp != visible_hp.max) {
		throw std::runtime_error("Received an invalid max HP. Expected: " + bounded::to_string(expected_max_visible_hp) + " but got " + bounded::to_string(visible_hp.max));
	}
	auto compute_value = [=](auto const visible_current) {
		return HP::current_type(bounded::max(1_bi, max_hp * visible_current / visible_hp.max));
	};
	if (is_ai) {
		auto const value = compute_value(visible_hp.current);
		return AllowedHP{value, value, value};
	} else {
		// TODO: Put in correct bounds on this for the foe Pokemon
		return AllowedHP{
			compute_value(visible_hp.current - 1_bi),
			compute_value(visible_hp.current),
			compute_value(visible_hp.current + 1_bi)
		};
	}
}

} // namespace technicalmachine
