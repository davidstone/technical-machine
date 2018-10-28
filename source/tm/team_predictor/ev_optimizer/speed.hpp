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

#pragma once

#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>
#include <tm/pokemon/level.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {

struct SpeedEVs {
private:
public:
	explicit SpeedEVs(Nature nature, Stat stat, Level level);
	friend auto begin(SpeedEVs const & evs) {
		return begin(evs.m_container);
	}
	friend auto end(SpeedEVs const & evs) {
		return end(evs.m_container);
	}
private:
	struct Mapped {
		Nature nature;
		EV ev;
	};
	static constexpr auto max_size = static_cast<std::intmax_t>(std::numeric_limits<Nature>::max() + 1_bi);
	containers::static_vector<Mapped, max_size> m_container;
};

}	// namespace technicalmachine
