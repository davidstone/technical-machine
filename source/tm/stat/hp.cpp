// Copyright (C) 2016 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/stat/hp.hpp>

#include <tm/pokemon/level.hpp>
#include <tm/stat/base_stats.hpp>

namespace technicalmachine {
namespace {

auto initial_hp(Generation const generation, Species const species, EV const ev, IV const iv, Level const level) {
	auto const base = BaseStats(generation, species).hp();
	auto const value = BOUNDED_CONDITIONAL((base > 1_bi),
		(2_bi * base + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 10_bi + level(),
		1_bi
	);
	static_assert(bounded::min_value<decltype(value)> == bounded::min_value<HP::max_type>, "Incorrect HP min.");
	static_assert(bounded::max_value<decltype(value)> == bounded::max_value<HP::max_type>, "Incorrect HP max.");
	return value;
}

}	// namespace

HP::HP(Generation const generation, Species const species, Level const level, EV const ev_, IV const iv_) :
	m_ev(ev_),
	m_iv(iv_),
	m_max(initial_hp(generation, species, m_ev, m_iv, level)),
	m_current(m_max)
	{
}

}	// namespace technicalmachine
