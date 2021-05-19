// Optimize offensive EVs and nature to remove waste
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>

#include <bounded/integer.hpp>

#include <containers/static_vector/static_vector.hpp>
#include <containers/size.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct OffensiveStats {
	Nature nature;
	IVAndEV attack;
	IVAndEV special_attack;
};

struct OffensiveEVs {
	struct Input {
		IV iv;
		InitialStat stat;
		bool include_evs;
	};
	OffensiveEVs(BaseStats, Level, Input attack, Input special_attack);

	auto find(Nature const nature) const -> OffensiveStats const *;
private:
	containers::static_vector<OffensiveStats, containers::size(containers::enum_range<Nature>()).value()> m_container;
};

}	// namespace technicalmachine
