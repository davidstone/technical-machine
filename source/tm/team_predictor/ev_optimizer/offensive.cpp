// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.team_predictor.ev_optimizer.offensive;

import tm.team_predictor.ev_optimizer.individual;
import tm.team_predictor.ev_optimizer.nature_effects;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.iv_and_ev;
import tm.stat.nature;
import tm.stat.nature_effect;
import tm.stat.stat_names;
import tm.stat.stat_style;

import bounded;
import containers;
import numeric_traits;
import std_module;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

template<typename Base>
struct Inputs {
	Base base;
	PossibleOptimizedIVs ivs;
	InitialStat<SpecialStyle::split> stat;
	bool useful;
};

export using OffensiveEVAtk = Inputs<BaseStats::Atk>;
export using OffensiveEVSpA = Inputs<BaseStats::SpA>;

struct OffensiveStats {
	Individual atk;
	Individual spa;
};

// If I don't have a physical move, prefer to lower that because it lowers
// confusion damage. If I do have a physical move but no special move, prefer
// to lower Special Attack because it is the only remaining stat guaranteed to
// be unused. This allows me to maximize Speed and the defensive stats.

constexpr auto physical_only_nature = containers::array({
	NatureEffects(NatureEffect::positive, NatureEffect::negative),
	NatureEffects(NatureEffect::neutral, NatureEffect::negative),
});
constexpr auto special_only_nature = containers::array({
	NatureEffects(NatureEffect::negative, NatureEffect::positive),
	NatureEffects(NatureEffect::negative, NatureEffect::neutral),
});
constexpr auto none_nature = containers::array({
	NatureEffects(NatureEffect::negative, NatureEffect::neutral),
});

using UsefulNatures = std::span<NatureEffects const>;
constexpr auto useful_nature_effects(bool const is_physical, bool const is_special) -> UsefulNatures {
	return
		is_physical and is_special ? UsefulNatures(nature_effects) :
		is_physical and !is_special ? UsefulNatures(physical_only_nature) :
		!is_physical and is_special ? UsefulNatures(special_only_nature) :
		UsefulNatures(none_nature);
}

export struct OffensiveEVs {
	constexpr OffensiveEVs(Level const level, OffensiveEVAtk const atk, OffensiveEVSpA const spa) {
		auto const atk_target = atk.useful ?
			atk.stat :
			numeric_traits::min_value<InitialStat<SpecialStyle::split>>;
		auto const spa_target = spa.useful ?
			spa.stat :
			numeric_traits::min_value<InitialStat<SpecialStyle::split>>;
		for (auto const natures : useful_nature_effects(atk.useful, spa.useful)) {
			for (auto const atk_value : possible(level, atk, natures.physical, atk_target)) {
				for (auto const spa_value : possible(level, spa, natures.special, spa_target)) {
					containers::push_back_into_capacity(
						m_container,
						OffensiveStats(atk_value, spa_value)
					);
				}
			}
		}
	}

	constexpr auto begin() const {
		return containers::begin(m_container);
	}
	constexpr auto end() const {
		return containers::end(m_container);
	}

private:
	static constexpr auto capacity =
		containers::size(nature_effects) *
		bounded::pow(max_possible_optimized_ivs, 2_bi);
	containers::static_vector<OffensiveStats, capacity> m_container;
};

} // namespace technicalmachine
