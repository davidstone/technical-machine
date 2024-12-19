// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.bide.duration;

import tm.compress;
import tm.generation;
import tm.probability;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
struct BideDuration {
	constexpr auto advance_one_turn() -> void {
		if (m_turns_active == numeric_traits::max_value<Counter>) {
			throw std::runtime_error("Bide should have activated by now");
		}
		++m_turns_active;
	}

	constexpr auto action_end_probability() const -> Probability {
		if constexpr (generation <= Generation::two) {
			switch (m_turns_active.value()) {
				case 0: return Probability(0.0);
				case 1: return Probability(0.5);
				case 2: return Probability(1.0);
				default: std::unreachable();
			}
		} else {
			switch (m_turns_active.value()) {
				case 0: return Probability(0.0);
				case 1: return Probability(1.0);
				default: std::unreachable();
			}
		}
	}

	friend auto operator==(BideDuration, BideDuration) -> bool = default;
	friend constexpr auto compress(BideDuration const value) {
		return compress(value.m_turns_active);
	}

private:
	using Counter = bounded::integer<0, generation <= Generation::two ? 2 : 1>;
	Counter m_turns_active = 0_bi;
};

} // namespace technicalmachine
