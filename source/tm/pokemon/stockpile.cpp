// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.stockpile;

import tm.compress;
import tm.exists_if;
import tm.generation;
import tm.rational;
import tm.saturating_add;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
struct Stockpile {
	static constexpr auto max = 3;
	// Returns whether Stockpile was able to increment (true) or if it is
	// already maxed out (false)
	constexpr auto increment() & {
		if constexpr (exists) {
			auto const initial = m_level;
			saturating_increment(m_level);
			return m_level == initial;
		} else {
			return false;
		}
	}

	// Returns the amount of Stockpile lost. This allows correct subtraction
	// for stat boosts.
	constexpr auto release() & {
		return std::exchange(m_level, 0_bi);
	}

	constexpr auto spit_up_power() const {
		return m_level * 100_bi;
	}

	friend auto operator==(Stockpile, Stockpile) -> bool = default;
	friend constexpr auto compress(Stockpile const value) {
		return compress(value.m_level);
	}

private:
	static constexpr auto exists = generation >= Generation::three;
	[[no_unique_address]] IntegerIf<bounded::integer<0, max>, exists> m_level = 0_bi;
};

export constexpr auto swallow_healing(bounded::bounded_integer auto const stockpiles) {
	using result = rational<bounded::integer<1, 1>, bounded::integer<1, 4>>;
	switch (stockpiles.value()) {
		case 1: return result{1_bi, 4_bi};
		case 2: return result{1_bi, 2_bi};
		case 3: return result{1_bi, 1_bi};
		default: std::unreachable();
	}
}

} // namespace technicalmachine
