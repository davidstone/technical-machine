export module tm.clients.ps.parsed_stats;

import tm.stat.generic_stats;
import tm.stat.initial_stat;
import tm.stat.stat_style;

import tm.visible_hp;

namespace technicalmachine::ps {

// PS always sends split-special stats
export using ParsedStat = InitialStat<SpecialInputStyle::split>;
export using ParsedStats = GenericStats<VisibleHP, ParsedStat>;

} // namespace techniclalmachine::ps
