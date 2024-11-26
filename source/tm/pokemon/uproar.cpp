// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.uproar;

import tm.compress;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct UproarCounter {
	constexpr auto advance_one_turn() & -> void {
		if (m_turns_active == numeric_traits::max_value<Counter>) {
			// TODO: Check this in the client and make this an assert
			throw std::runtime_error("Tried to advance Uproar too much");
		}
		++m_turns_active;
	}

	friend auto operator==(UproarCounter, UproarCounter) -> bool = default;
	friend constexpr auto compress(UproarCounter const value) {
		return compress(value.m_turns_active);
	}
private:
	// TODO: Change range in different generations
	// Gen 3,4: 2-5
	// Gen 5+: 3
	using Counter = bounded::integer<0, 5>;
	Counter m_turns_active;
};

} // namespace technicalmachine
