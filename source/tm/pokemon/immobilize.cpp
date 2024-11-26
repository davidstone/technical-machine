// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.immobilize;

import tm.compress;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

// Turns remaining on Bind, Clamp, Fire Spin, and Wrap in Generation 1
export struct Immobilize {
	constexpr auto advance_one_turn() & -> void {
		// TODO: Check this in the client and make this an assert
		if (m_counter == numeric_traits::max_value<Counter>) {
			throw std::runtime_error("Tried to advance immobilize move too far");
		}
		++m_counter;
	}

	friend auto operator==(Immobilize, Immobilize) -> bool = default;
	friend constexpr auto compress(Immobilize const value) {
		return compress(value.m_counter);
	}

private:
	// TODO:
	// 2: 37.5%
	// 3: 37.5%
	// 4: 12.5%
	// 5: 12.5%
	using Counter = bounded::integer<0, 5>;
	Counter m_counter = 0_bi;
};

} // namespace technicalmachine
