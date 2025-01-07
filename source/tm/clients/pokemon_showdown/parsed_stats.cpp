// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.parsed_stats;

import tm.stat.generic_stats;
import tm.stat.initial_stat;
import tm.stat.stat_style;

import tm.visible_hp;

import bounded;

namespace technicalmachine::ps {

// PS always sends split-special stats
export using ParsedStat = InitialStat<SpecialInputStyle::split>;
export using ParsedStats = GenericStats<VisibleHP, ParsedStat>;

} // namespace techniclalmachine::ps
