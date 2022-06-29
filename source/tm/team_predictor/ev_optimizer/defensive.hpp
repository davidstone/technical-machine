// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/team_predictor/ev_optimizer/defensive_data_point.hpp>
#include <tm/team_predictor/ev_optimizer/possible_optimized_ivs.hpp>

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/begin_end.hpp>
#include <containers/integer_range.hpp>
#include <containers/is_empty.hpp>
#include <containers/push_back.hpp>
#include <containers/size.hpp>
#include <containers/static_vector.hpp>

namespace technicalmachine {

struct DefensiveEVHP {
	BaseStats::HP base;
	PossibleOptimizedIVs ivs;
	HP::max_type stat;
};
template<Generation generation, typename Base> requires (generation >= Generation::three)
struct DefensiveEVStat {
	Base base;
	PossibleOptimizedIVs ivs;
	InitialStat<generation> stat;
};

template<Generation generation>
using DefensiveEVDef = DefensiveEVStat<generation, BaseStats::Def>;

template<Generation generation>
using DefensiveEVSpD = DefensiveEVStat<generation, BaseStats::SpD>;

constexpr auto ev_sum(DataPoint const data) {
	return data.hp.ev.value() + data.defense.ev.value() + data.special_defense.ev.value();
}

constexpr auto ev_range() {
	return containers::transform(containers::inclusive_integer_range(0_bi, EV::useful_max, 4_bi), bounded::construct<EV>);
}

struct DefensiveEVs {
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
						auto best_per_nature_and_iv = bounded::optional<DataPoint>();
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
