// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.rampage;

import tm.compress;
import tm.generation;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

// Outrage, Petal Dance, and Thrash
// TODO: Ends if it fails to execute (FP, flinch), which only confuses in Gen 5+
export template<Generation generation>
struct Rampage {
	constexpr auto advance_one_turn() & -> void {
		if (m_turns_active == numeric_traits::max_value<Counter>) {
			// TODO: Check this in the client and make this an assert
			throw std::runtime_error("Tried to advance Rampage too much");
		}
		++m_turns_active;
	}

	friend auto operator==(Rampage, Rampage) -> bool = default;
	friend constexpr auto compress(Rampage const value) {
		return compress(value.m_turns_active);
	}
private:
	using Counter = bounded::integer<0, generation == Generation::one ? 4 : 3>;
	Counter m_turns_active;
};

} // namespace technicalmachine
