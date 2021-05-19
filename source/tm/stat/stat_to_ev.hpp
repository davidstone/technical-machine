// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/has_physical_or_special_move.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto round_up_divide(auto const lhs, auto const rhs) {
	return lhs / rhs + BOUNDED_CONDITIONAL(lhs % rhs == 0_bi, 0_bi, 1_bi);
}

inline auto hp_to_ev(BaseStats const base, Level const level, HP::max_type const stat, IV const iv) -> EV {
	if (base.hp() == 1_bi) {
		return EV(0_bi);
	}
	auto const computed = (round_up_divide((stat - level() - 10_bi) * 100_bi, level()) - iv.value() - 2_bi * base.hp()) * 4_bi;
	return EV(EV::value_type(bounded::max(0_bi, computed)));
}

// `target` is not just InitialStat because this function is also used in the EV
// optimizer, where values outside the legal range are regularly encountered as
// part of speculative computation.
constexpr auto stat_to_ev(bounded::bounded_integer auto const target, Nature const nature, RegularStat const stat_name, BaseStats::regular_value_type const base, IV const iv, Level const level) -> bounded::optional<EV> {
	auto const computed = (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi;
	if (computed > EV::max) {
		return bounded::none;
	}
	return EV(EV::value_type(bounded::max(0_bi, computed), bounded::non_check));
}

auto calculate_ivs_and_evs(
	Generation const generation,
	Species const species,
	Level const level,
	GenericStats<HP::max_type, InitialStat> const stats,
	bounded::optional<Type> const hidden_power_type,
	bool has_physical_move,
	decltype(containers::enum_range<Nature>()) const nature_range
) -> CombinedStats;

inline auto calculate_ivs_and_evs(
	Generation const generation,
	Species const species,
	Level const level,
	GenericStats<HP::max_type, InitialStat> const stats,
	bounded::optional<Type> const hidden_power_type,
	bool const has_physical_move
) -> CombinedStats {
	auto const nature_range = generation <= Generation::two ? 
		containers::enum_range(Nature::Hardy, Nature::Hardy) :
		containers::enum_range<Nature>();

	return calculate_ivs_and_evs(
		generation,
		species,
		level,
		stats,
		hidden_power_type,
		has_physical_move,
		nature_range
	);
}

template<Generation generation>
auto calculate_ivs_and_evs(Pokemon<generation> const pokemon) -> CombinedStats {
	auto const nature = pokemon.nature();
	auto const stats = GenericStats<HP::max_type, InitialStat>{
		pokemon.hp().max(),
		pokemon.stat(RegularStat::atk),
		pokemon.stat(RegularStat::def),
		pokemon.stat(RegularStat::spa),
		pokemon.stat(RegularStat::spd),
		pokemon.stat(RegularStat::spe)
	};
	return calculate_ivs_and_evs(
		generation,
		pokemon.species(),
		pokemon.level(),
		stats,
		get_hidden_power_type(pokemon),
		has_physical_move(pokemon),
		containers::enum_range(nature, nature)
	);
}

} // namespace technicalmachine
