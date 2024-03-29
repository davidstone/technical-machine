// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.team_predictor.ev_optimizer.offensive;

import tm.team_predictor.ev_optimizer.possible_optimized_ivs;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.iv_and_ev;
import tm.stat.nature;
import tm.stat.stat_names;
import tm.stat.stat_style;
import tm.stat.stat_to_ev;

import bounded;
import containers;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

template<typename Base>
struct OffensiveEVInputs {
	Base base;
	PossibleOptimizedIVs ivs;
	InitialStat<SpecialStyle::split> stat;
	bool include_evs;
};

export using OffensiveEVAtk = OffensiveEVInputs<BaseStats::Atk>;

export using OffensiveEVSpA = OffensiveEVInputs<BaseStats::SpA>;

struct OffensiveStats {
	Nature nature;
	IVAndEV attack;
	IVAndEV special_attack;
};

// If I don't have a physical move, prefer to lower that because it lowers
// confusion damage. If I do have a physical move but no special move, prefer
// to lower Special Attack because it is the only remaining stat guaranteed to
// be unused. This allows me to maximize Speed and the defensive stats.
constexpr auto useful_natures(bool const is_physical, bool const is_special) {
	return containers::filter(
		containers::enum_range<Nature>(),
		[=](Nature const nature) {
			if (!is_physical) {
				auto const lowers = lowers_stat(nature, SplitSpecialRegularStat::atk);
				return !is_special ? lowers and !boosts_stat(nature, SplitSpecialRegularStat::spa) : lowers;
			}
			if (!is_special) {
				return lowers_stat(nature, SplitSpecialRegularStat::spa);
			}
			return true;
		}
	);
}

template<typename Base>
struct OffensiveEVInputsIV {
	Base base;
	IV iv;
	InitialStat<SpecialStyle::split> stat;
	bool include_evs;
};

using OffensiveEVAtkIV = OffensiveEVInputsIV<BaseStats::Atk>;

using OffensiveEVSpAIV = OffensiveEVInputsIV<BaseStats::SpA>;

template<typename Base>
constexpr auto target_stat(SplitSpecialRegularStat const stat_name, Level const level, OffensiveEVInputsIV<Base> const input, Nature const harmful_nature) {
	return input.include_evs ? input.stat : initial_stat<SpecialStyle::split>(stat_name, input.base, level, harmful_nature, input.iv, EV(0_bi));
}

auto evs_for_nature(Level const level, Nature const nature, OffensiveEVAtkIV const atk, OffensiveEVSpAIV const spa) {
	auto const target_atk = target_stat(SplitSpecialRegularStat::atk, level, atk, Nature::Modest);
	auto const target_spa = target_stat(SplitSpecialRegularStat::spa, level, spa, Nature::Adamant);
	auto const atk_ev = stat_to_ev_at_least(target_atk, SplitSpecialRegularStat::atk, atk.base, level, nature, atk.iv);
	auto const spa_ev = stat_to_ev_at_least(target_spa, SplitSpecialRegularStat::spa, spa.base, level, nature, spa.iv);
	return BOUNDED_CONDITIONAL(atk_ev and spa_ev, (OffensiveStats{nature, {atk.iv, *atk_ev}, {spa.iv, *spa_ev}}), tv::none);
}

export struct OffensiveEVs {
	constexpr OffensiveEVs(Level const level, OffensiveEVAtk const atk, OffensiveEVSpA const spa) {
		for (auto const nature : useful_natures(atk.include_evs, spa.include_evs)) {
			for (auto const atk_iv : atk.ivs) {
				for (auto const spa_iv : spa.ivs) {
					auto const result = evs_for_nature(
						level,
						nature,
						OffensiveEVAtkIV{atk.base, atk_iv, atk.stat, atk.include_evs},
						OffensiveEVSpAIV{spa.base, spa_iv, spa.stat, spa.include_evs}
					);
					if (result) {
						containers::push_back_into_capacity(m_container, *result);
					}
				}
			}
		}
	}

	constexpr auto find(Nature const nature) const -> OffensiveStats const * {
		return containers::maybe_find_if(m_container, [=](auto const value) { return value.nature == nature; });
	}

private:
	static constexpr auto maximum_natures = containers::size(containers::enum_range<Nature>());
	containers::static_vector<OffensiveStats, maximum_natures * bounded::pow(max_possible_optimized_ivs, 2_bi)> m_container;
};

} // namespace technicalmachine
