// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/hp.hpp>

namespace technicalmachine {
namespace {

auto initial_hp(BaseStats const base, Level const level, IV const iv, EV const ev) {
	auto const value = BOUNDED_CONDITIONAL((base.hp() > 1_bi),
		(2_bi * base.hp() + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 10_bi + level(),
		1_bi
	);
	static_assert(bounded::min_value<decltype(value)> == bounded::min_value<HP::max_type>);
	static_assert(bounded::max_value<decltype(value)> == bounded::max_value<HP::max_type>);
	return value;
}

} // namespace

HP::HP(BaseStats const base, Level const level, IV const iv, EV const ev):
	m_max(initial_hp(base, level, iv, ev)),
	m_current(m_max)
{
}

} // namespace technicalmachine
