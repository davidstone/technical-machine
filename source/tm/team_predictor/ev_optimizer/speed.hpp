// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/stat_to_ev.hpp>
#include <tm/stat/nature.hpp>

#include <bounded/assert.hpp>

#include <containers/static_vector/static_vector.hpp>
#include <containers/begin_end.hpp>
#include <containers/emplace_back.hpp>
#include <containers/is_empty.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

struct SpeedEVs {
	template<Generation generation> requires (generation >= Generation::three)
	struct Input {
		InitialStat<generation> stat;
	};
	template<Generation generation> requires (generation >= Generation::three)
	SpeedEVs(BaseStats const base, Level const level, IV const iv, Input<generation> const target) {
		for (auto const nature : containers::enum_range<Nature>()) {
			auto const ev = stat_to_ev(target.stat, SplitSpecialRegularStat::spe, base.spe(), level, nature, iv);
			if (ev) {
				containers::emplace_back(m_container, nature, iv, *ev);
			}
		}
		BOUNDED_ASSERT(!containers::is_empty(m_container));
	}

	auto begin() const {
		return containers::begin(m_container);
	}
	auto end() const {
		return containers::end(m_container);
	}
private:
	struct Mapped {
		Nature nature;
		IV iv;
		EV ev;
	};
	containers::static_vector<Mapped, static_cast<int>(numeric_traits::max_value<Nature>) + 1> m_container;
};

} // namespace technicalmachine
