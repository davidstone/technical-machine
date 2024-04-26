// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.team_predictor.ev_optimizer.defensive;

import tm.team_predictor.ev_optimizer.individual;
import tm.team_predictor.ev_optimizer.nature_effects;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.iv_and_ev;
import tm.stat.max_hp;
import tm.stat.nature_effect;
import tm.stat.stat_style;

import bounded;
export import containers;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

export struct DefensiveEVHP {
	BaseStats::HP base;
	PossibleOptimizedIVs ivs;
	MaxHP stat;
};
template<typename Base>
struct DefensiveEVStat {
	Base base;
	PossibleOptimizedIVs ivs;
	InitialStat<SpecialStyle::split> stat;
};

export using DefensiveEVDef = DefensiveEVStat<BaseStats::Def>;

export using DefensiveEVSpD = DefensiveEVStat<BaseStats::SpD>;

struct DataPoint {
	IVAndEV hp;
	Individual def;
	Individual spd;
	friend auto operator==(DataPoint, DataPoint) -> bool = default;
};

constexpr auto ev_sum(DataPoint const data) {
	return data.hp.ev.value() + data.def.ev.value() + data.spd.ev.value();
}

constexpr auto ev_range = containers::transform(containers::inclusive_integer_range(0_bi, EV::useful_max, 4_bi), bounded::construct<EV>);

constexpr auto to_index(IV const iv) {
	return bounded::assume_in_range(iv.value() - 30_bi, 0_bi, 1_bi);
}

export struct DefensiveEVs {
	constexpr DefensiveEVs(Level const level, DefensiveEVHP const original_hp, DefensiveEVDef const def, DefensiveEVSpD const spd) {
		bounded::bounded_integer auto const def_product = original_hp.stat * def.stat;
		bounded::bounded_integer auto const spd_product = original_hp.stat * spd.stat;

		auto defensive_product = [=](DataPoint const value) {
			auto const hp = HP(original_hp.base, level, value.hp.iv, value.hp.ev).max();
			auto single_product = [=](auto const base_stat, Individual const generated) {
				return hp * initial_stat<SpecialStyle::split>(base_stat, level, generated.nature_effect, generated.iv, generated.ev);
			};
			return
				single_product(def.base, value.def) *
				single_product(spd.base, value.spd);
		};

		auto is_better = [=](DataPoint const candidate, DataPoint const current) -> bool {
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

		constexpr auto is_expected = [](IV const iv) { return iv == IV(30_bi) or iv == IV(31_bi); };
		BOUNDED_ASSERT(containers::all(def.ivs, is_expected));
		BOUNDED_ASSERT(containers::all(spd.ivs, is_expected));
		for (auto const hp_iv : original_hp.ivs) {
			static_assert(max_possible_optimized_ivs == 2_bi);
			auto possibilities = containers::array<tv::optional<DataPoint>, containers::size(nature_effects), 2_bi, 2_bi>();
			for (auto const hp_ev : ev_range) {
				auto const hp = HP(original_hp.base, level, hp_iv, hp_ev).max();
				for (auto const nature_effect_index : containers::integer_range(containers::size(nature_effects))) {
					auto const nature_effect = nature_effects[nature_effect_index];
					auto const all_def = possible(
						level,
						def,
						nature_effect.physical,
						round_up_divide(def_product, hp)
					);
					auto const all_spd = possible(
						level,
						spd,
						nature_effect.special,
						round_up_divide(spd_product, hp)
					);
					auto & nature_possibilities = possibilities[nature_effect_index];
					for (auto const specific_def : all_def) {
						auto & def_possibilities = nature_possibilities[to_index(specific_def.iv)];
						for (auto const specific_spd : all_spd) {
							auto const candidate = DataPoint(
								{hp_iv, hp_ev},
								specific_def,
								specific_spd
							);
							if (ev_sum(candidate) > max_total_evs(SpecialStyle::split)) {
								continue;
							}
							auto & result = def_possibilities[to_index(specific_spd.iv)];
							if (!result or is_better(candidate, *result)) {
								insert(result, candidate);
							}
						}
					}
				}
			}
			for (auto const p0 : possibilities) {
				for (auto const p1 : p0) {
					for (auto const p2 : p1) {
						if (p2) {
							containers::push_back_into_capacity(m_container, *p2);
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

private:
	static constexpr auto capacity =
		containers::size(nature_effects) *
		bounded::pow(max_possible_optimized_ivs, 3_bi);
	containers::static_vector<DataPoint, capacity> m_container;
};

} // namespace technicalmachine
