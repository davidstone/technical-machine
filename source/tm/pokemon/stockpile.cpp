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
import tv;

namespace technicalmachine {
using namespace bounded::literal;

using StockpileCount = bounded::integer<0, 3>;

export template<Generation generation>
struct Stockpile {
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
	[[no_unique_address]] IntegerIf<StockpileCount, exists> m_level = 0_bi;
};

using Healing = rational<
	bounded::constant_t<1>,
	bounded::integer<1, 4>
>;
export constexpr auto swallow_healing(StockpileCount const stockpiles) -> tv::optional<Healing> {
	switch (stockpiles.value()) {
		case 0: return tv::none;
		case 1: return Healing(1_bi, 4_bi);
		case 2: return Healing(1_bi, 2_bi);
		case 3: return Healing(1_bi, 1_bi);
		default: std::unreachable();
	}
}

} // namespace technicalmachine
