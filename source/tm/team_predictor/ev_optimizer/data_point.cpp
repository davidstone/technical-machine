// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.ev_optimizer.data_point;

import tm.stat.iv_and_ev;
import tm.stat.nature;

namespace technicalmachine {

export struct DataPoint {
	Nature nature;
	IVAndEV hp;
	IVAndEV defense;
	IVAndEV special_defense;
};

} // namespace technicalmachine
