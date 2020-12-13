// Optimize Speed EVs and nature to remove waste
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/ev_optimizer/speed.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/emplace_back.hpp>
#include <containers/is_empty.hpp>

#include <algorithm>

namespace technicalmachine {
using namespace bounded::literal;

SpeedEVs::SpeedEVs(BaseStats const base, Level const level, IV const iv, InitialStat const target) {
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const ev = stat_to_ev(target, nature, RegularStat::spe, base.spe(), iv, level);
		if (ev) {
			containers::emplace_back(m_container, nature, IVAndEV{iv, *ev});
		}
	}
	BOUNDED_ASSERT(!containers::is_empty(m_container));
}

}	// namespace technicalmachine
