// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hidden_power_ivs.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/has_physical_or_special_move.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/string_conversions/generation.hpp>
#include <tm/string_conversions/species.hpp>

#include <bounded/optional.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/integer_range.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

using namespace std::string_view_literals;
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
constexpr auto stat_to_ev(bounded::bounded_integer auto const target, RegularStat const stat_name, BaseStats::regular_value_type const base, Level const level, Nature const nature, IV const iv) -> bounded::optional<EV> {
	auto const computed = (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi;
	if (computed > EV::max) {
		return bounded::none;
	}
	return EV(EV::value_type(bounded::max(0_bi, computed), bounded::non_check));
}

template<Generation generation>
auto calculate_ivs_and_evs(
	Species const species,
	Level const level,
	GenericStats<HP::max_type, InitialStat<generation>> const stats,
	bounded::optional<Type> const hidden_power_type,
	bool has_physical_move,
	decltype(containers::enum_range<Nature>()) const nature_range
) -> CombinedStats<generation> {
	// TODO: Use Hidden Power power to determine IVs, not just the type
	auto const base = BaseStats(generation, species);
	
	while (true) {
		auto const ivs = hidden_power_ivs(generation, hidden_power_type, has_physical_move);
		auto const hp_ev = hp_to_ev(base, level, stats.hp(), ivs.hp());

		for (auto const nature : nature_range) {
			auto compute_ev = [=](RegularStat const stat_name, auto const base_stat) {
				return stat_to_ev(stats[stat_name], stat_name, base_stat, level, nature, ivs[stat_name]);
			};
			auto const attack_ev = compute_ev(RegularStat::atk, base.atk());
			if (!attack_ev) {
				continue;
			}
			auto const defense_ev = compute_ev(RegularStat::def, base.def());
			if (!defense_ev) {
				continue;
			}
			auto const special_attack_ev = compute_ev(RegularStat::spa, base.spa());
			if (!special_attack_ev) {
				continue;
			}
			auto const special_defense_ev = compute_ev(RegularStat::spd, base.spd());
			if (!special_defense_ev) {
				continue;
			}
			auto const speed_ev = compute_ev(RegularStat::spe, base.spe());
			if (!speed_ev) {
				continue;
			}

			auto const evs = EVs(
				hp_ev,
				*attack_ev,
				*defense_ev,
				*special_attack_ev,
				*special_defense_ev,
				*speed_ev
			);
			if (ev_sum(evs) > max_total_evs(generation)) {
				continue;
			}
			
			return CombinedStats<generation>{
				nature,
				ivs,
				evs
			};
		}
		if (has_physical_move) {
			break;
		}
		has_physical_move = true;
	}
	throw std::runtime_error(containers::concatenate<std::string>(
		"No Nature + EV combination combines to give the received stats in generation "sv,
		to_string(generation),
		": Species: "sv, to_string(species),
		", Level: "sv, bounded::to_string(level()),
		", HP: "sv, bounded::to_string(stats.hp()),
		", Attack: "sv, bounded::to_string(stats.atk()),
		", Defense: "sv, bounded::to_string(stats.def()),
		", Special Attack: "sv, bounded::to_string(stats.spa()),
		", Special Defense: "sv, bounded::to_string(stats.spd()),
		", Speed: "sv, bounded::to_string(stats.spe())
	));
}

template<Generation generation>
auto calculate_ivs_and_evs(
	Species const species,
	Level const level,
	GenericStats<HP::max_type, InitialStat<generation>> const stats,
	bounded::optional<Type> const hidden_power_type,
	bool const has_physical_move
) {
	constexpr auto nature_range = generation <= Generation::two ? 
		containers::enum_range(Nature::Hardy, Nature::Hardy) :
		containers::enum_range<Nature>();

	return calculate_ivs_and_evs<generation>(
		species,
		level,
		stats,
		hidden_power_type,
		has_physical_move,
		nature_range
	);
}

template<Generation generation>
auto calculate_ivs_and_evs(Pokemon<generation> const pokemon) {
	auto const nature = pokemon.nature();
	auto const stats = GenericStats<HP::max_type, InitialStat<generation>>{
		pokemon.hp().max(),
		pokemon.stat(RegularStat::atk),
		pokemon.stat(RegularStat::def),
		pokemon.stat(RegularStat::spa),
		pokemon.stat(RegularStat::spd),
		pokemon.stat(RegularStat::spe)
	};
	return calculate_ivs_and_evs<generation>(
		pokemon.species(),
		pokemon.level(),
		stats,
		get_hidden_power_type(pokemon),
		has_physical_move(pokemon),
		containers::enum_range(nature, nature)
	);
}

} // namespace technicalmachine
