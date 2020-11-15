// Optimize defensive EVs and nature to remove waste
// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/team_predictor/ev_optimizer/defensive.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/calculate.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/transform.hpp>
#include <containers/integer_range.hpp>
#include <containers/push_back.hpp>

namespace technicalmachine {
namespace {

constexpr auto ev_sum(DataPoint const data) {
	return data.hp.ev.value() + data.defense.ev.value() + data.special_defense.ev.value();
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
	BOUNDED_ASSERT(!empty(m_container));
}

}	// namespace technicalmachine
