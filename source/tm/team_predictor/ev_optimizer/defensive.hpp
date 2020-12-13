// Optimize defensive EVs and nature to remove waste
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/team_predictor/ev_optimizer/defensive_data_point.hpp>

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/initial_stat.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/static_vector/static_vector.hpp>
#include <containers/begin_end.hpp>
#include <containers/integer_range.hpp>
#include <containers/size.hpp>

namespace technicalmachine {

struct DefensiveEVs {
	struct InputHP {
		IV iv;
		HP::max_type stat;
	};
	struct InputStat {
		IV iv;
		InitialStat stat;
	};
	DefensiveEVs(BaseStats, Level, InputHP, InputStat defense, InputStat special_defense);
	auto begin() const {
		return containers::begin(m_container);
	}
	auto end() const {
		return containers::end(m_container);
	}
	auto find(Nature const nature) const {
		return containers::find_if(m_container, [=](auto const value) { return value.nature == nature; });
	}
private:
	static constexpr auto number_of_natures = containers::size(containers::enum_range<Nature>());
	containers::static_vector<DataPoint, number_of_natures.value()> m_container;
};

}	// namespace technicalmachine
