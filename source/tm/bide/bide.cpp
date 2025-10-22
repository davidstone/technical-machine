// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.bide.bide;

import tm.bide.damage;
import tm.bide.duration;

import tm.stat.current_hp;

import tm.compress;
import tm.generation;
import tm.probability;

import tv;

namespace technicalmachine {

export template<Generation generation>
struct Bide {
	constexpr auto add_damage(CurrentHP const damage) & {
		m_damage.add(damage);
	}

	constexpr auto advance_one_turn() & -> void {
		m_duration.advance_one_turn();
	}

	constexpr auto released_damage() const -> CurrentHP {
		return m_damage.released();
	}

	constexpr auto action_end_probability() const -> Probability {
		return m_duration.action_end_probability();
	}

	friend auto operator==(Bide, Bide) -> bool = default;
	friend constexpr auto compress(Bide const value) {
		return compress_combine(value.m_damage, value.m_duration);
	}

private:
	BideDamage m_damage;
	BideDuration<generation> m_duration;
};

} // namespace technicalmachine
