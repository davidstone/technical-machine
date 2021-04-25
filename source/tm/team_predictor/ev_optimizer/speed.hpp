// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>

#include <containers/static_vector/static_vector.hpp>
#include <containers/begin_end.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

struct SpeedEVs {
	SpeedEVs(BaseStats, Level, IV, InitialStat target);
	auto begin() const {
		return containers::begin(m_container);
	}
	auto end() const {
		return containers::end(m_container);
	}
private:
	struct Mapped {
		Nature nature;
		IVAndEV stat;
	};
	containers::static_vector<Mapped, static_cast<int>(numeric_traits::max_value<Nature>) + 1> m_container;
};

}	// namespace technicalmachine
