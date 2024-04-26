// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.team_predictor.ev_optimizer.speed;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.initial_stat;
import tm.stat.nature_effect;
import tm.stat.stat_style;

import tm.team_predictor.ev_optimizer.individual;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;

import bounded;
import containers;
import numeric_traits;

namespace technicalmachine {
using namespace bounded::literal;

export struct SpeedEVs {
	struct Input {
		BaseStats::Spe base;
		PossibleOptimizedIVs ivs;
		InitialStat<SpecialStyle::split> target;
	};
	constexpr SpeedEVs(Level const level, Input const input) {
		for (auto const nature_effect : containers::enum_range<NatureEffect>()) {
			containers::append_into_capacity(
				m_container,
				possible(level, input, nature_effect, input.target)
			);
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
	containers::static_vector<Individual, bounded::number_of<NatureEffect> * max_possible_optimized_ivs> m_container;
};

} // namespace technicalmachine
