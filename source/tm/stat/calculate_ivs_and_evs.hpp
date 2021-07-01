// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/base_stats.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/possible_dvs_or_ivs.hpp>
#include <tm/stat/stat_names.hpp>
#include <tm/stat/stat_to_ev.hpp>
#include <tm/stat/stats.hpp>

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/string_conversions/generation.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/type.hpp>

#include <bounded/optional.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/integer_range.hpp>
#include <containers/is_empty.hpp>
#include <containers/take.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

template<Generation generation>
auto calculate_ivs_and_evs(
	Species const species,
	Level const level,
	Stats<generation> const stats,
	bounded::optional<HiddenPower<generation>> const hidden_power,
	decltype(containers::enum_range<Nature>()) const nature_range
) -> CombinedStats<generation> {
	BOUNDED_ASSERT(!containers::is_empty(nature_range));
	auto const base = BaseStats(generation, species);
	auto const dvs_or_ivs = possible_dvs_or_ivs(hidden_power);
	if constexpr (generation <= Generation::two) {
		constexpr auto nature = Nature::Hardy;
		auto compute_ev = [=](SplitSpecialRegularStat const stat_name, DV const dv) {
			return stat_to_ev<generation>(stats[stat_name], stat_name, base[stat_name], level, nature, IV(dv));
		};
		for (auto const atk_dv : containers::reversed(dvs_or_ivs.atk())) {
			auto const atk_ev = compute_ev(SplitSpecialRegularStat::atk, atk_dv);
			if (!atk_ev) {
				continue;
			}
			for (auto const def_dv : containers::reversed(dvs_or_ivs.def())) {
				auto const def_ev = compute_ev(SplitSpecialRegularStat::def, def_dv);
				if (!def_ev) {
					continue;
				}
				for (auto const spe_dv : containers::reversed(dvs_or_ivs.spe())) {
					auto const spe_ev = compute_ev(SplitSpecialRegularStat::spe, spe_dv);
					if (!spe_ev) {
						continue;
					}
					for (auto const spc_dv : containers::reversed(dvs_or_ivs.spc())) {
						auto const spa_ev = compute_ev(SplitSpecialRegularStat::spa, spc_dv);
						if (!spa_ev) {
							continue;
						}
						auto const spd_ev = compute_ev(SplitSpecialRegularStat::spd, spc_dv);
						if (!spd_ev) {
							continue;
						}
						// With Pokemon under level 100, sometimes `n` might
						// give enough to put the Pokemon at enough SpA, but be
						// one short for SpD. `n + 4` might not change SpA but
						// increase SpD to the correct value.
						auto const spc_ev = bounded::max(*spa_ev, *spd_ev);

						auto const dvs = DVs(atk_dv, def_dv, spc_dv, spe_dv);
						auto const hp_ev = hp_to_ev(base.hp(), level, stats.hp().max(), IV(dvs.hp()));
						if (!hp_ev) {
							continue;
						}

						if constexpr (generation != Generation::one) {
							if (hidden_power and HiddenPower<generation>(dvs) != *hidden_power) {
								continue;
							}
						}

						auto const evs = OldGenEVs(
							*hp_ev,
							*atk_ev,
							*def_ev,
							*spe_ev,
							spc_ev
						);
						return CombinedStats<generation>{nature, dvs, evs};
					}
				}
			}
		}
	} else {
		auto partial_ev_sum_is_valid = [](auto... evs) {
			return (... + evs.value()) <= max_total_evs(generation);
		};
		for (auto const hp_iv : containers::reversed(dvs_or_ivs.hp())) {
			auto const hp_ev = hp_to_ev(base.hp(), level, stats.hp().max(), hp_iv);
			if (!hp_ev) {
				continue;
			}
			for (auto const nature : nature_range) {
				auto compute_ev = [=](SplitSpecialRegularStat const stat_name, IV const iv) {
					return stat_to_ev<generation>(stats[stat_name], stat_name, base[stat_name], level, nature, iv);
				};
				for (auto const atk_iv : containers::reversed(dvs_or_ivs.atk())) {
					auto const atk_ev = compute_ev(SplitSpecialRegularStat::atk, atk_iv);
					if (!atk_ev) {
						continue;
					}
					for (auto const def_iv : containers::reversed(dvs_or_ivs.def())) {
						auto const def_ev = compute_ev(SplitSpecialRegularStat::def, def_iv);
						if (!def_ev) {
							continue;
						}
						if (!partial_ev_sum_is_valid(*hp_ev, *atk_ev, *def_ev)) {
							continue;
						}
						for (auto const spa_iv : containers::reversed(dvs_or_ivs.spa())) {
							auto spa_ev = compute_ev(SplitSpecialRegularStat::spa, spa_iv);
							if (!spa_ev) {
								continue;
							}
							if (!partial_ev_sum_is_valid(*hp_ev, *atk_ev, *def_ev, *spa_ev)) {
								continue;
							}
							for (auto const spd_iv : containers::reversed(dvs_or_ivs.spd())) {
								auto spd_ev = compute_ev(SplitSpecialRegularStat::spd, spd_iv);
								if (!spd_ev) {
									continue;
								}
								if (!partial_ev_sum_is_valid(*hp_ev, *atk_ev, *def_ev, *spa_ev, *spd_ev)) {
									continue;
								}
								for (auto const spe_iv : containers::reversed(dvs_or_ivs.spe())) {
									auto const spe_ev = compute_ev(SplitSpecialRegularStat::spe, spe_iv);
									if (!spe_ev) {
										continue;
									}
									if (!partial_ev_sum_is_valid(*hp_ev, *atk_ev, *def_ev, *spa_ev, *spd_ev, *spe_ev)) {
										continue;
									}
									auto const ivs = IVs(hp_iv, atk_iv, def_iv, spa_iv, spd_iv, spe_iv);
									if (hidden_power and HiddenPower<generation>(ivs) != *hidden_power) {
										continue;
									}

									auto const evs = EVs(
										*hp_ev,
										*atk_ev,
										*def_ev,
										*spa_ev,
										*spd_ev,
										*spe_ev
									);
									if (ev_sum(evs) > max_total_evs(generation)) {
										continue;
									}
									return CombinedStats<generation>{nature, ivs, evs};
								}
							}
						}
					}
				}
			}
		}
	}
	auto const nature_string = [=] {
		auto result = containers::string("{"sv);
		for (auto const nature : containers::take(nature_range, bounded::increase_min<0>(containers::size(nature_range) - 1_bi))) {
			result = containers::concatenate<containers::string>(std::move(result), to_string(nature), ", "sv);
		}
		return containers::concatenate<containers::string>(std::move(result), to_string(containers::back(nature_range)), "}"sv);
	}();
	auto const hidden_power_string = hidden_power ?
		containers::concatenate<containers::string>(
			"Type: "sv, to_string(hidden_power->type()),
			", Power: "sv, bounded::to_string(hidden_power->power())
		) :
		containers::string("none");
	throw std::runtime_error(containers::concatenate<std::string>(
		"No Nature, IV, and EV combination gives the received stats in generation "sv,
		to_string(generation),
		": Species: "sv, to_string(species),
		", Level: "sv, bounded::to_string(level()),
		", Possible Natures: "sv, nature_string,
		", HP: "sv, bounded::to_string(stats.hp().max()),
		", Attack: "sv, bounded::to_string(stats.atk()),
		", Defense: "sv, bounded::to_string(stats.def()),
		", Special Attack: "sv, bounded::to_string(stats.spa()),
		", Special Defense: "sv, bounded::to_string(stats.spd()),
		", Speed: "sv, bounded::to_string(stats.spe()),
		", Hidden Power: "sv, hidden_power_string
	));
}

template<Generation generation>
auto calculate_ivs_and_evs(
	Species const species,
	Level const level,
	Stats<generation> const stats,
	bounded::optional<HiddenPower<generation>> const hidden_power
) {
	constexpr auto nature_range = generation <= Generation::two ? 
		containers::enum_range(Nature::Hardy, Nature::Hardy) :
		containers::enum_range<Nature>();

	return calculate_ivs_and_evs(
		species,
		level,
		stats,
		hidden_power,
		nature_range
	);
}

} // namespace technicalmachine
