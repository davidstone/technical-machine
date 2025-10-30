// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.immobilize;

import tm.compress;
import tm.probability;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

// Turns spent on Bind, Clamp, Fire Spin, and Wrap in Generation 1
//
// We don't know whether immobilize released when we are reading a live log
// until after we try to do something else. So we unfortunately have to have two
// code paths: one of them for analysis (where we deal in probability) and the
// other for live battles / log replays (where we deal in knowns and unknowns).
// `advance_one_turn` is used for only the analysis path.
export struct Immobilize {
	constexpr auto advance_one_turn() & -> void {
		// TODO: Check this in the client and make this an assert
		if (m_counter == numeric_traits::max_value<Counter>) {
			throw std::runtime_error("Tried to advance immobilize move too far");
		}
		++m_counter;
	}

	constexpr auto end_of_turn_end_probability() const -> Probability {
		// 2: 37.5%
		// 3: 37.5%
		// 4: 12.5%
		// 5: 12.5%
		switch (m_counter.value()) {
			case 0: return Probability(0.0);
			case 1: return Probability(0.0);
			case 2: return Probability(0.375);
			case 3: return Probability(0.6);
			case 4: return Probability(0.5);
			case 5: return Probability(1.0);
			default: std::unreachable();
		}
	}

	friend auto operator==(Immobilize, Immobilize) -> bool = default;
	friend constexpr auto compress(Immobilize const value) {
		return compress(value.m_counter);
	}

private:
	using Counter = bounded::integer<0, 5>;
	Counter m_counter = 0_bi;
};

} // namespace technicalmachine
