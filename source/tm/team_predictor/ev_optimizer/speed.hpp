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

#pragma once

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {

struct SpeedEVs {
	SpeedEVs(BaseStats, Level, IV, InitialStat target);
	friend auto begin(SpeedEVs const & evs) {
		return begin(evs.m_container);
	}
	friend auto end(SpeedEVs const & evs) {
		return end(evs.m_container);
	}
private:
	struct Mapped {
		Nature nature;
		IVAndEV stat;
	};
	containers::static_vector<Mapped, static_cast<int>(bounded::max_value<Nature>) + 1> m_container;
};

}	// namespace technicalmachine
