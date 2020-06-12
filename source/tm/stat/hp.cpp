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
