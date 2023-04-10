// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.test.status;

import tm.status.status;
import tm.status.status_name;

import tm.ability;
import tm.generation;

namespace technicalmachine {

// Awakening probability
static_assert([]{
	constexpr auto ability = Ability::Pressure;
	auto status = Status(StatusName::sleep);
	for (auto const expected : {0.0, 1.0 / 4.0, 1.0 / 3.0, 1.0 / 2.0, 1.0 / 1.0}) {
		auto const calculated = status.probability_of_clearing(Generation::four, ability);
		BOUNDED_ASSERT(expected == calculated);
		constexpr auto clear_status = false;
		status.advance_from_move(ability, clear_status);
	}
	return true;
}());

} // namespace technicalmachine
