export module tm.stat.stats;

import tm.stat.generic_stats;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.stat_style;

import std_module;

namespace technicalmachine {

export template<StatStyle stat_style>
using Stats = std::conditional_t<
	stat_style == StatStyle::gen1,
	GenericCombinedStats<HP, InitialStat<to_input_style(stat_style)>>,
	GenericStats<HP, InitialStat<to_input_style(stat_style)>>
>;

} // namespace technicalmachine
