// Optimize EVs and nature to remove waste
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/ev_optimizer/ev_optimizer.hpp>

#include <tm/team_predictor/ev_optimizer/defensive.hpp>
#include <tm/team_predictor/ev_optimizer/offensive.hpp>
#include <tm/team_predictor/ev_optimizer/speed.hpp>

#include <tm/move/category.hpp>

#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hidden_power_ivs.hpp>

#include <bounded/assert.hpp>

#include <containers/begin_end.hpp>

#include <random>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto combine(Generation const generation, OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & speed_container) -> CombinedStats<IVAndEV> {
	auto best = bounded::optional<CombinedStats<IVAndEV>>{};
	for (auto const & speed : speed_container) {
		auto const offensive = o.find(speed.nature);
		if (!offensive) {
			continue;
		}
		auto const defensive = d.find(speed.nature);
		if (defensive == containers::end(d)) {
			continue;
		}
		auto candidate = CombinedStats<IVAndEV>{
			speed.nature,
			defensive->hp,
			offensive->attack,
			defensive->defense,
			offensive->special_attack,
			defensive->special_defense,
			speed.stat
		};
		if (!best or ev_sum(candidate) < ev_sum(*best)) {
			insert(best, candidate);
		}
	}
	BOUNDED_ASSERT(best);
	BOUNDED_ASSERT(ev_sum(*best) <= max_total_evs(generation));
	return *best;
}

} // namespace

auto optimize_evs(
	Generation const generation,
	CombinedStats<IVAndEV> combined,
	Species const species,
	Level const level,
	bounded::optional<Type> const hidden_power_type,
	bool const include_attack,
	bool const include_special_attack,
	std::mt19937 & random_engine
) -> CombinedStats<IVAndEV> {
	auto const base_stats = BaseStats(generation, species);
	while (true) {
		auto const previous = combined;
		combined = pad_random_evs(generation, combined, include_attack, include_special_attack, random_engine);
		auto const stats = initial_stats(base_stats, level, combined);
		combined = compute_minimal_spread(generation, base_stats, stats, level, hidden_power_type, include_attack, include_special_attack);
		// Technically this isn't correct based on how I pad: I could have some
		// leftover EVs that could have done some good somewhere else, but were
		// not enough to increase the stat they were randomly assigned to.
		if (previous == combined) {
			return combined;
		}
	}
}

auto compute_minimal_spread(
	Generation const generation,
	BaseStats const base_stats,
	GenericStats<HP::max_type, InitialStat> stats,
	Level const level,
	bounded::optional<Type> const hidden_power_type,
	bool const include_attack,
	bool const include_special_attack
) -> CombinedStats<IVAndEV> {
	auto const ivs = hidden_power_ivs(generation, hidden_power_type, include_attack);

	if (generation <= Generation::two) {
		return {
			Nature::Hardy,
			{ivs.hp, EV(EV::useful_max)},
			{ivs.atk, include_attack ? EV(EV::useful_max) : EV(0_bi)},
			{ivs.def, EV(EV::useful_max)},
			{ivs.spa, EV(EV::useful_max)},
			{ivs.spd, EV(EV::useful_max)},
			{ivs.spe, EV(EV::useful_max)},
		};
	}

	return combine(
		generation,
		OffensiveEVs(
			base_stats,
			level,
			OffensiveEVs::Input{ivs.atk, stats.atk, include_attack},
			OffensiveEVs::Input{ivs.spa, stats.spa, include_special_attack}
		),
		DefensiveEVs(
			base_stats,
			level,
			DefensiveEVs::InputHP{ivs.hp, stats.hp},
			DefensiveEVs::InputStat{ivs.def, stats.def},
			DefensiveEVs::InputStat{ivs.spd, stats.spd}
		),
		SpeedEVs(base_stats, level, ivs.spe, stats.spe)
	);
}

auto pad_random_evs(Generation const generation, CombinedStats<IVAndEV> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<IVAndEV> {
	if (generation <= Generation::two) {
		for (auto const stat_name : containers::enum_range<PermanentStat>()) {
			auto const minimize_stat = stat_name == PermanentStat::atk and !include_attack;
			combined[stat_name].ev = minimize_stat ? EV(0_bi) : EV(EV::useful_max);
		}
		return combined;
	}
	auto distribution = std::discrete_distribution{};
	constexpr auto minimal_increment = 4_bi;
	while (ev_sum(combined) + minimal_increment <= max_total_evs(generation)) {
		distribution.param({
			combined.hp.ev == EV::useful_max ? 0.0 : 1.0,
			(!include_attack or combined.atk.ev == EV::useful_max) ? 0.0 : 1.0,
			combined.def.ev == EV::useful_max ? 0.0 : 1.0,
			(!include_special_attack or combined.spa.ev == EV::useful_max) ? 0.0 : 1.0,
			combined.spd.ev == EV::useful_max ? 0.0 : 1.0,
			combined.spe.ev == EV::useful_max ? 0.0 : 1.0,
		});
		auto const index = distribution(random_engine);
		auto & ev = combined[PermanentStat(index - 1)].ev;
		ev = EV(EV::value_type(ev.value() + minimal_increment));
	}
	return combined;
}

}	// namespace technicalmachine
