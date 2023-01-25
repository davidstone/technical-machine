// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.team_predictor.ev_optimizer.speed;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.stat_to_ev;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.team_predictor.ev_optimizer.possible_optimized_ivs;

import tm.generation;

import bounded;
import containers;
import numeric_traits;

namespace technicalmachine {
using namespace bounded::literal;

export struct SpeedEVs {
	template<Generation generation> requires (generation >= Generation::three)
	struct Input {
		InitialStat<generation> stat;
	};
	template<Generation generation> requires (generation >= Generation::three)
	constexpr SpeedEVs(BaseStats::Spe const base, Level const level, PossibleOptimizedIVs const ivs, Input<generation> const target) {
		for (auto const nature : containers::enum_range<Nature>()) {
			for (auto const iv : ivs) {
				auto const ev = stat_to_ev_at_least(target.stat, SplitSpecialRegularStat::spe, base, level, nature, iv);
				if (ev) {
					containers::emplace_back(m_container, nature, iv, *ev);
				}
			}
		}
		BOUNDED_ASSERT(!containers::is_empty(m_container));
	}

	constexpr auto begin() const {
		return containers::begin(m_container);
	}
	constexpr auto end() const {
		return containers::end(m_container);
	}

private:
	struct Mapped {
		Nature nature;
		IV iv;
		EV ev;
	};
	static constexpr auto maximum_natures = bounded::integer(numeric_traits::max_value<Nature>) + 1_bi;
	containers::static_vector<Mapped, maximum_natures * max_possible_optimized_ivs> m_container;
};

} // namespace technicalmachine
