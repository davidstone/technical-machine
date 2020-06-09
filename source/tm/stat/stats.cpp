// All 'normal' stats that a Pokemon has
// Copyright (C) 2018 David Stone
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

#include <tm/stat/stats.hpp>

namespace technicalmachine {
namespace {

constexpr auto default_ev = EV(0_bi);

} // namespace

Stats::Stats(Generation const generation, BaseStats const base, Level const level):
	m_hp(base, level, default_iv(generation), default_ev),
	m_stats{
		Stat(base.atk(), default_iv(generation), default_ev),
		Stat(base.def(), default_iv(generation), default_ev),
		Stat(base.spa(), default_iv(generation), default_ev),
		Stat(base.spd(), default_iv(generation), default_ev),
		Stat(base.spe(), default_iv(generation), default_ev)
	}
{
}

auto Stats::hp() const -> HP const & {
	return m_hp;
}
auto Stats::hp() -> HP & {
	return m_hp;
}

auto Stats::operator[](RegularStat const stat) const -> Stat const & {
	return containers::at(m_stats, stat);
}
auto Stats::operator[](RegularStat const stat) -> Stat & {
	return containers::at(m_stats, stat);
}


}	// namespace technicalmachine
