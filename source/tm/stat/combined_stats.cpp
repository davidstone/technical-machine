// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.combined_stats;

import tm.stat.default_evs;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_style;

import tm.generation;

import std_module;

namespace technicalmachine {

export template<SpecialInputStyle stat_style>
struct CombinedStats {
	Nature nature = Nature::Hardy;
	DVsOrIVs<stat_style> dvs_or_ivs = max_dvs_or_ivs<stat_style>;
	std::conditional_t<stat_style == SpecialInputStyle::combined, OldGenEVs, EVs> evs =  default_evs<stat_style>;
	friend auto operator==(CombinedStats, CombinedStats) -> bool = default;
};

export template<Generation generation>
using CombinedStatsFor = CombinedStats<special_input_style_for(generation)>;

} // namespace technicalmachine
