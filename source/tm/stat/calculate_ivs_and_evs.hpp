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
#include <containers/static_vector.hpp>
#include <containers/take.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

using namespace std::string_view_literals;

template<Generation generation>
constexpr auto calculate_ivs_and_evs(
	Species const species,
	Level const level,
	Stats<generation> const stats,
	bounded::optional<HiddenPower<generation>> const hidden_power,
	decltype(containers::enum_range<Nature>()) const nature_range
) -> CombinedStats<generation> {
	BOUNDED_ASSERT(!containers::is_empty(nature_range));
	auto const base = BaseStats(generation, species);
	auto const dvs_or_ivs = possible_dvs_or_ivs(hidden_power);
	auto compute_ev = [=](SplitSpecialRegularStat const stat_name, Nature const nature, auto const dv_or_iv) {
		return stat_to_ev<generation>(stats[stat_name], stat_name, base[stat_name], level, nature, IV(dv_or_iv));
	};
	auto const dv_or_iv_ev_range = [=]<typename DVOrIV>(auto const possible, bounded::detail::types<DVOrIV>, auto const to_ev) {
		struct WithOptionalEV {
			DVOrIV dv_or_iv;
			bounded::optional<EV> ev;
		};
		using WithEV = std::conditional_t<std::is_same_v<DVOrIV, DV>, DVAndEV, IVAndEV>;
		return containers::make_static_vector(
			containers::transform(
				containers::filter(
					containers::transform(
						containers::reversed(possible),
						[=](DVOrIV const dv_or_iv) {
							return WithOptionalEV{
								dv_or_iv,
								to_ev(dv_or_iv)
							};
						}
					),
					[](WithOptionalEV const value) { return static_cast<bool>(value.ev); }
				),
				[](WithOptionalEV const value) { return WithEV{value.dv_or_iv, *value.ev}; }
			)
		);
	};
	if constexpr (generation <= Generation::two) {
		constexpr auto nature = Nature::Hardy;
		auto const dv_ev_range = [=](SplitSpecialRegularStat const stat_name) {
			return dv_or_iv_ev_range(
				dvs_or_ivs[stat_name],
				bounded::detail::types<DV>(),
				[=](DV const dv) { return compute_ev(stat_name, nature, dv); }
			);
		};
		auto const atk_range = dv_ev_range(SplitSpecialRegularStat::atk);
		auto const def_range = dv_ev_range(SplitSpecialRegularStat::def);
		auto const spe_range = dv_ev_range(SplitSpecialRegularStat::spe);
		for (auto const atk : atk_range) {
			for (auto const def : def_range) {
				for (auto const spe : spe_range) {
					for (auto const spc_dv : containers::reversed(dvs_or_ivs.spc())) {
						auto const spa_ev = compute_ev(SplitSpecialRegularStat::spa, nature, spc_dv);
						if (!spa_ev) {
							continue;
						}
						auto const spd_ev = compute_ev(SplitSpecialRegularStat::spd, nature, spc_dv);
						if (!spd_ev) {
							continue;
						}
						// With Pokemon under level 100, sometimes `n` might
						// give enough to put the Pokemon at enough SpA, but be
						// one short for SpD. `n + 4` might not change SpA but
						// increase SpD to the correct value.
						auto const spc_ev = bounded::max(*spa_ev, *spd_ev);

						auto const dvs = DVs(atk.dv, def.dv, spc_dv, spe.dv);
						auto const hp_ev = hp_to_ev(stats.hp().max(), base.hp(), level, IV(dvs.hp()));
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
							atk.ev,
							def.ev,
							spe.ev,
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
		auto const hp_range = dv_or_iv_ev_range(
			dvs_or_ivs.hp(),
			bounded::detail::types<IV>(),
			[=](IV const iv) { return hp_to_ev(stats.hp().max(), base.hp(), level, iv); }
		);
		for (auto const hp : hp_range) {
			for (auto const nature : nature_range) {
				auto const iv_ev_range = [=](SplitSpecialRegularStat const stat_name) {
					return dv_or_iv_ev_range(
						dvs_or_ivs[stat_name],
						bounded::detail::types<IV>(),
						[=](IV const iv) { return compute_ev(stat_name, nature, iv); }
					);
				};
				auto const atk_range = iv_ev_range(SplitSpecialRegularStat::atk);
				auto const def_range = iv_ev_range(SplitSpecialRegularStat::def);
				auto const spa_range = iv_ev_range(SplitSpecialRegularStat::spa);
				auto const spd_range = iv_ev_range(SplitSpecialRegularStat::spd);
				auto const spe_range = iv_ev_range(SplitSpecialRegularStat::spe);
				for (auto const atk : atk_range) {
					for (auto const def : def_range) {
						if (!partial_ev_sum_is_valid(hp.ev, atk.ev, def.ev)) {
							break;
						}
						for (auto const spa : spa_range) {
							if (!partial_ev_sum_is_valid(hp.ev, atk.ev, def.ev, spa.ev)) {
								break;
							}
							for (auto const spd : spd_range) {
								if (!partial_ev_sum_is_valid(hp.ev, atk.ev, def.ev, spa.ev, spd.ev)) {
									break;
								}
								for (auto const spe : spe_range) {
									if (!partial_ev_sum_is_valid(hp.ev, atk.ev, def.ev, spa.ev, spd.ev, spe.ev)) {
										break;
									}
									auto const ivs = IVs(hp.iv, atk.iv, def.iv, spa.iv, spd.iv, spe.iv);
									if (hidden_power and HiddenPower<generation>(ivs) != *hidden_power) {
										continue;
									}

									auto const evs = EVs(
										hp.ev,
										atk.ev,
										def.ev,
										spa.ev,
										spd.ev,
										spe.ev
									);
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
constexpr auto calculate_ivs_and_evs(
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
