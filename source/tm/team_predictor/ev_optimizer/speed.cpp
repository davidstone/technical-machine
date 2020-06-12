// Optimize Speed EVs and nature to remove waste
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

#include <tm/team_predictor/ev_optimizer/speed.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/emplace_back.hpp>

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
	BOUNDED_ASSERT(!empty(m_container));
}

}	// namespace technicalmachine
