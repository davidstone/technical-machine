// Optimize defensive EVs and nature to remove waste
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/ev_optimizer/defensive.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/integer_range.hpp>
#include <containers/is_empty.hpp>
#include <containers/push_back.hpp>

namespace technicalmachine {
namespace {

constexpr auto ev_sum(DataPoint const data) {
	return data.hp.ev.value() + data.defense.ev.value() + data.special_defense.ev.value();
}

constexpr auto ev_range() {
	return containers::transform(containers::inclusive_integer_range(0_bi, EV::useful_max, 4_bi), bounded::construct_return<EV>);
}

}	// namespace

DefensiveEVs::DefensiveEVs(BaseStats const base_stats, Level const level, InputHP const original_hp, InputStat const def, InputStat const spd) {
	bounded::bounded_integer auto const def_product = original_hp.stat * def.stat;
	bounded::bounded_integer auto const spd_product = original_hp.stat * spd.stat;

	auto defensive_product = [=](DataPoint const value) {
		auto const hp = HP(base_stats, level, value.hp.iv, value.hp.ev).max();
		auto single_product = [=](RegularStat const name, BaseStats::regular_value_type const base_stat, IVAndEV const generated) {
			return hp * initial_stat(name, base_stat, value.nature, generated.iv, generated.ev, level);
		};

		return single_product(RegularStat::def, base_stats.def(), value.defense) * single_product(RegularStat::spd, base_stats.spd(), value.special_defense);
	};

	for (auto const nature : containers::enum_range<Nature>()) {
		auto best_per_nature = bounded::optional<DataPoint>{};
		for (auto const hp_ev : ev_range()) {
			auto const hp = HP(base_stats, level, original_hp.iv, hp_ev);
			auto find_minimum_matching = [=](RegularStat const stat_name, auto const base, IV const iv, bounded::bounded_integer auto const original_product) {
				auto const target_stat = round_up_divide(original_product, hp.max());
				return stat_to_ev(target_stat, nature, stat_name, base, iv, level);
			};

			auto const defense_ev = find_minimum_matching(RegularStat::def, base_stats.def(), def.iv, def_product);
			if (!defense_ev) {
				continue;
			}
			auto const special_defense_ev = find_minimum_matching(RegularStat::spd, base_stats.spd(), spd.iv, spd_product);
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
			auto const candidate = DataPoint{nature, {original_hp.iv, hp_ev}, {def.iv, *defense_ev}, {spd.iv, *special_defense_ev}};
			if (!best_per_nature or is_better(candidate, *best_per_nature)) {
				insert(best_per_nature, candidate);
			}
		}
		if (best_per_nature) {
			containers::push_back(m_container, *best_per_nature);
		}
	}
	BOUNDED_ASSERT(!containers::is_empty(m_container));
}

}	// namespace technicalmachine
