// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.stat.hp;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.current_hp;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.max_hp;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto initial_hp(BaseStats::HP const base, Level const level, IV const iv, EV const ev) -> MaxHP {
	std::same_as<MaxHP> auto const value = BOUNDED_CONDITIONAL(
		base > 1_bi,
		(2_bi * base + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 10_bi + level(),
		1_bi
	);
	return value;
}

export struct HP {
	constexpr explicit HP(MaxHP max_):
		m_max(max_),
		m_current(max_)
	{
	}

	constexpr HP(BaseStats::HP const base, Level const level, IV const iv, EV const ev):
		m_max(initial_hp(base, level, iv, ev)),
		m_current(m_max)
	{
	}

	constexpr auto operator=(auto const & value) & -> HP & {
		m_current = bounded::clamp(value, 0_bi, m_max);
		return *this;
	}
	constexpr auto current() const -> CurrentHP {
		return m_current;
	}

	constexpr auto max() const -> MaxHP {
		return m_max;
	}

	friend auto operator==(HP, HP) -> bool = default;

private:
	MaxHP m_max;
	CurrentHP m_current;

	constexpr explicit HP(bounded::tombstone_tag, auto const make):
		m_max(make()),
		m_current()
	{
	}
	friend bounded::tombstone_traits<HP>;
	friend bounded::tombstone_traits_composer<&HP::m_max>;
};

static_assert(HP(100_bi).max() == 100_bi);
static_assert(HP(100_bi).current() == 100_bi);
static_assert(HP(60_bi, Level(1_bi), IV(31_bi), EV(0_bi)).max() == 12_bi);
static_assert(HP(250_bi, Level(100_bi), IV(30_bi), EV(252_bi)).max() == 703_bi);

} // namespace technicalmachine

template<>
struct bounded::tombstone_traits<technicalmachine::HP> : bounded::tombstone_traits_composer<&technicalmachine::HP::m_max> {
};
