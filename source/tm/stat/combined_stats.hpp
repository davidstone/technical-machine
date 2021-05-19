// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/generic_stats.hpp>
#include <tm/stat/nature.hpp>

namespace technicalmachine {

struct CombinedStats {
	Nature nature;
	IVs dvs_or_ivs;
	EVs evs;

	friend auto operator==(CombinedStats, CombinedStats) -> bool = default;
};

} // namespace technicalmachine
