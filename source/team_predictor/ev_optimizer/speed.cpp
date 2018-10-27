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

#include "speed.hpp"

#include "../../stat/calculate.hpp"
#include "../../stat/stat_names.hpp"

#include <containers/integer_range.hpp>

#include <algorithm>
#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

SpeedEVs::SpeedEVs(Nature const initial_nature, Stat const initial_speed_stat, Level const level) {
	auto const speed = initial_stat(StatNames::SPE, initial_speed_stat, level, initial_nature);
	for (auto const nature : containers::enum_range<Nature>()) {
		auto const evs = containers::integer_range(0_bi, EV::max + 4_bi, 4_bi);
		auto const it = std::partition_point(begin(evs), end(evs), [=](auto const ev) {
			auto const stat = Stat(initial_speed_stat, EV(ev));
			return initial_stat(StatNames::SPE, stat, level, nature) < speed;
		});
		if (it != end(evs)) {
			m_container.emplace_back(nature, EV(*it));
		}
	}
	assert(!empty(m_container));
}

}	// namespace technicalmachine
