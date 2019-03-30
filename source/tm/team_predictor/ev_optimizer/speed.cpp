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

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/legacy_iterator.hpp>

#include <algorithm>

namespace technicalmachine {
using namespace bounded::literal;

SpeedEVs::SpeedEVs(Nature const initial_nature, Stat const initial_speed_stat, Level const level) {
	auto const speed = initial_stat(StatNames::SPE, initial_speed_stat, level, initial_nature);
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const evs = ev_range();
		auto const it = std::partition_point(containers::legacy_iterator(begin(evs)), containers::legacy_iterator(end(evs)), [=](auto const ev) {
			auto const stat = Stat(initial_speed_stat, ev);
			return initial_stat(StatNames::SPE, stat, level, nature) < speed;
		});
		if (it.base() != end(evs)) {
			m_container.emplace_back(nature, *it);
		}
	}
	BOUNDED_ASSERT(!empty(m_container));
}

}	// namespace technicalmachine
