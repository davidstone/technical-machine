// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.team_predictor.ev_optimizer.defensive;

import tm.team_predictor.ev_optimizer.data_point;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.hp;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.initial_stat;
import tm.stat.iv_and_ev;
import tm.stat.max_hp;
import tm.stat.stat_names;
import tm.stat.stat_to_ev;

import tm.generation;
import tm.round_up_divide;

import bounded;
import containers;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

export struct DefensiveEVHP {
	BaseStats::HP base;
	PossibleOptimizedIVs ivs;
	MaxHP stat;
};
template<Generation generation, typename Base> requires (generation >= Generation::three)
struct DefensiveEVStat {
	Base base;
	PossibleOptimizedIVs ivs;
	InitialStat<generation> stat;
};

export template<Generation generation>
using DefensiveEVDef = DefensiveEVStat<generation, BaseStats::Def>;

export template<Generation generation>
using DefensiveEVSpD = DefensiveEVStat<generation, BaseStats::SpD>;

constexpr auto ev_sum(DataPoint const data) {
	return data.hp.ev.value() + data.defense.ev.value() + data.special_defense.ev.value();
}

constexpr auto ev_range() {
	return containers::transform(containers::inclusive_integer_range(0_bi, EV::useful_max, 4_bi), bounded::construct<EV>);
}

export struct DefensiveEVs {
	template<Generation generation> requires (generation >= Generation::three)
	constexpr DefensiveEVs(Level const level, DefensiveEVHP const original_hp, DefensiveEVDef<generation> const def, DefensiveEVSpD<generation> const spd) {
		bounded::bounded_integer auto const def_product = original_hp.stat * def.stat;
		bounded::bounded_integer auto const spd_product = original_hp.stat * spd.stat;

		auto defensive_product = [=](DataPoint const value) {
			auto const hp = HP(original_hp.base, level, value.hp.iv, value.hp.ev).max();
			auto single_product = [=](SplitSpecialRegularStat const name, auto const base_stat, IVAndEV const generated) {
				return hp * initial_stat<generation>(name, base_stat, level, value.nature, generated.iv, generated.ev);
			};

			return single_product(SplitSpecialRegularStat::def, def.base, value.defense) * single_product(SplitSpecialRegularStat::spd, spd.base, value.special_defense);
		};

		for (auto const nature : containers::enum_range<Nature>()) {
			for (auto const hp_iv : original_hp.ivs) {
				for (auto const def_iv : def.ivs) {
					for (auto const spd_iv : spd.ivs) {
						auto best_per_nature_and_iv = tv::optional<DataPoint>();
						for (auto const hp_ev : ev_range()) {
							auto const hp = HP(original_hp.base, level, hp_iv, hp_ev);
							auto find_minimum_matching = [=](SplitSpecialRegularStat const stat_name, auto const base, IV const iv, bounded::bounded_integer auto const original_product) {
								auto const target_stat = round_up_divide(original_product, hp.max());
								return stat_to_ev_at_least(target_stat, stat_name, base, level, nature, iv);
							};

							auto const defense_ev = find_minimum_matching(SplitSpecialRegularStat::def, def.base, def_iv, def_product);
							if (!defense_ev) {
								continue;
							}
							auto const special_defense_ev = find_minimum_matching(SplitSpecialRegularStat::spd, spd.base, spd_iv, spd_product);
							if (!special_defense_ev) {
								continue;
							}
							auto is_better = [=](DataPoint const candidate, DataPoint const current) {
								auto const cmp = ev_sum(candidate) <=> ev_sum(current);
								if (cmp < 0) {
									return true;
								} else if (cmp > 0) {
									return false;
								}
								auto const candidate_product = defensive_product(candidate);
								auto const current_product = defensive_product(current);
								return candidate_product > current_product;
							};
							auto const candidate = DataPoint{nature, {hp_iv, hp_ev}, {def_iv, *defense_ev}, {spd_iv, *special_defense_ev}};
							if (!best_per_nature_and_iv or is_better(candidate, *best_per_nature_and_iv)) {
								insert(best_per_nature_and_iv, candidate);
							}
						}
						if (best_per_nature_and_iv) {
							containers::push_back(m_container, *best_per_nature_and_iv);
						}
					}
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
	constexpr auto find(Nature const nature) const {
		return containers::find_if(m_container, [=](auto const value) { return value.nature == nature; });
	}

private:
	static constexpr auto maximum_natures = containers::size(containers::enum_range<Nature>());
	containers::static_vector<DataPoint, maximum_natures * bounded::pow(max_possible_optimized_ivs, 3_bi)> m_container;
};

} // namespace technicalmachine
