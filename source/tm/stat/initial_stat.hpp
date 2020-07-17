// Copyright (C) 2020 David Stone
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

#pragma once

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

constexpr auto initial_stat(RegularStat const stat_name, auto const base, Nature const nature, IV const iv, EV const ev, Level const level) {
	auto const pre_nature = (2_bi * base + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 5_bi;
	return pre_nature * boost(nature, stat_name);
}

using InitialStat = decltype(initial_stat(
	std::declval<RegularStat>(),
	std::declval<BaseStats::regular_value_type>(),
	std::declval<Nature>(),
	std::declval<IV>(),
	std::declval<EV>(),
	std::declval<Level>()
));

inline auto initial_stat(RegularStat const stat_name, BaseStats const base_stats, CombinedStats<IVAndEV> inputs, Level const level) {
	auto const iv_and_ev = inputs[PermanentStat(stat_name)];
	return initial_stat(stat_name, base_stats[stat_name], inputs.nature, iv_and_ev.iv, iv_and_ev.ev, level);
}

} // namespace technicalmachine
