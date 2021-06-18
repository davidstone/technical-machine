// Copyright David Stone 2021.
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
#include <tm/stat/stat_to_ev.hpp>

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

template<Generation generation>
auto calculate_ivs_and_evs(
	Species const species,
	Level const level,
	Stats<generation> const stats,
	bounded::optional<Type> const hidden_power_type,
	bool has_physical_move,
	decltype(containers::enum_range<Nature>()) const nature_range
) -> CombinedStats<generation> {
	using namespace std::string_view_literals;
	// TODO: Use Hidden Power power to determine IVs, not just the type
	auto const base = BaseStats(generation, species);
	while (true) {
		auto const ivs = hidden_power_ivs<generation>(hidden_power_type, has_physical_move);
		auto const hp_ev = hp_to_ev(base.hp(), level, stats.hp().max(), IV(ivs.hp()));

		for (auto const nature : nature_range) {
			auto compute_ev = [=](SplitSpecialRegularStat const stat_name) {
				return stat_to_ev(stats[stat_name], stat_name, base[stat_name], level, nature, IV(ivs[stat_name]));
			};
			auto const atk_ev = compute_ev(SplitSpecialRegularStat::atk);
			if (!atk_ev) {
				continue;
			}
			auto const def_ev = compute_ev(SplitSpecialRegularStat::def);
			if (!def_ev) {
				continue;
			}
			auto spa_ev = compute_ev(SplitSpecialRegularStat::spa);
			if (!spa_ev) {
				continue;
			}
			auto spd_ev = compute_ev(SplitSpecialRegularStat::spd);
			if (!spd_ev) {
				continue;
			}
			if constexpr (generation <= Generation::two) {
				auto const spc_ev = bounded::max(*spa_ev, *spd_ev);
				*spa_ev = spc_ev;
				*spd_ev = spc_ev;
			}
			auto const spe_ev = compute_ev(SplitSpecialRegularStat::spe);
			if (!spe_ev) {
				continue;
			}

			auto const evs = [=] {
				if constexpr (generation <= Generation::two) {
					return OldGenEVs(
						hp_ev,
						*atk_ev,
						*def_ev,
						*spe_ev,
						*spa_ev
					);
				} else {
					return EVs(
						hp_ev,
						*atk_ev,
						*def_ev,
						*spa_ev,
						*spd_ev,
						*spe_ev
					);
				}
			}();
			if (ev_sum(evs) > max_total_evs(generation)) {
				continue;
			}
			return CombinedStats<generation>{nature, ivs, evs};
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
		", HP: "sv, bounded::to_string(stats.hp().max()),
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
	Stats<generation> const stats,
	bounded::optional<Type> const hidden_power_type,
	bool const has_physical_move
) {
	constexpr auto nature_range = generation <= Generation::two ? 
		containers::enum_range(Nature::Hardy, Nature::Hardy) :
		containers::enum_range<Nature>();

	return calculate_ivs_and_evs(
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
	auto const stats = Stats<generation>{
		pokemon.hp(),
		pokemon.stat(SplitSpecialRegularStat::atk),
		pokemon.stat(SplitSpecialRegularStat::def),
		pokemon.stat(SplitSpecialRegularStat::spa),
		pokemon.stat(SplitSpecialRegularStat::spd),
		pokemon.stat(SplitSpecialRegularStat::spe)
	};
	return calculate_ivs_and_evs(
		pokemon.species(),
		pokemon.level(),
		stats,
		get_hidden_power_type(pokemon),
		has_physical_move(pokemon),
		containers::enum_range(nature, nature)
	);
}

} // namespace technicalmachine
