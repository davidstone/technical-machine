// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.stat.calculate_ivs_and_evs;

import tm.pokemon.any_pokemon;
import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.iv_and_ev;
import tm.stat.nature;
import tm.stat.nature_effect;
import tm.stat.possible_dvs_or_ivs;
import tm.stat.stat_names;
import tm.stat.stat_style;
import tm.stat.stat_to_ev;
import tm.stat.stats;

import tm.string_conversions.generation;
import tm.string_conversions.nature;
import tm.string_conversions.species;
import tm.string_conversions.type;

import tm.generation;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;
using namespace std::string_view_literals;

template<bool combine>
constexpr auto fix_if(SplitSpecialRegularStat const stat_name) {
	if constexpr (combine) {
		return to_combined(stat_name);
	} else {
		return stat_name;
	}
}

constexpr auto to_string(auto const stats) -> containers::string {
	auto const hp_str = containers::to_string(stats.hp.max());
	auto const atk_str = containers::to_string(stats.atk);
	auto const def_str = containers::to_string(stats.def);
	auto const spe_str = containers::to_string(stats.spe);
	if constexpr (requires { stats.spc; }) {
		return containers::concatenate<containers::string>(
			"HP: "sv, hp_str,
			", Attack: "sv, atk_str,
			", Defense: "sv, def_str,
			", Speed: "sv, spe_str,
			", Special: "sv, containers::to_string(stats.spc)
		);
	} else {
		return containers::concatenate<containers::string>(
			"HP: "sv, hp_str,
			", Attack: "sv, atk_str,
			", Defense: "sv, def_str,
			", Special Attack: "sv, containers::to_string(stats.spa),
			", Special Defense: "sv, containers::to_string(stats.spd),
			", Speed: "sv, spe_str
		);
	}
}

export template<Generation generation>
constexpr auto calculate_ivs_and_evs(
	Species const species,
	Level const level,
	Stats<stat_style_for(generation)> const stats,
	tv::optional<HiddenPower<generation>> const hidden_power,
	decltype(containers::enum_range<Nature>()) const nature_range
) -> CombinedStatsFor<generation> {
	BOUNDED_ASSERT(!containers::is_empty(nature_range));
	auto const base = BaseStats(generation, species);
	auto const dvs_or_ivs = possible_dvs_or_ivs(hidden_power);
	auto compute_ev = [=](SplitSpecialRegularStat const stat_name, Nature const nature, auto const dv_or_iv) {
		return stat_to_ev(
			stats[fix_if<generation == Generation::one>(stat_name)],
			base[stat_name],
			level,
			to_nature_effect(nature, stat_name),
			IV(dv_or_iv)
		);
	};
	auto const dv_or_iv_ev_range = [=]<typename DVOrIV>(auto const possible, bounded::type_t<DVOrIV>, auto const to_ev) {
		using WithEV = std::conditional_t<std::same_as<DVOrIV, DV>, DVAndEV, IVAndEV>;
		return containers::make_static_vector(
			containers::remove_none(
				containers::transform(
					containers::reversed(possible),
					[=](DVOrIV const dv_or_iv) -> tv::optional<WithEV> {
						auto const ev = to_ev(dv_or_iv);
						if (!ev) {
							return tv::none;
						}
						return WithEV(dv_or_iv, *ev);
					}
				)
			)
		);
	};
	if constexpr (generation <= Generation::two) {
		constexpr auto nature = Nature::Hardy;
		auto const dv_ev_range = [=](SplitSpecialRegularStat const stat_name) {
			return dv_or_iv_ev_range(
				dvs_or_ivs[fix_if<true>(stat_name)],
				bounded::type<DV>,
				[=](DV const dv) { return compute_ev(stat_name, nature, dv); }
			);
		};
		auto const atk_range = dv_ev_range(SplitSpecialRegularStat::atk);
		auto const def_range = dv_ev_range(SplitSpecialRegularStat::def);
		auto const spe_range = dv_ev_range(SplitSpecialRegularStat::spe);
		for (auto const atk : atk_range) {
			for (auto const def : def_range) {
				for (auto const spe : spe_range) {
					for (auto const spc_dv : containers::reversed(dvs_or_ivs.spc)) {
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

						auto const dvs = DVs(atk.dv, def.dv, spe.dv, spc_dv);
						auto const hp_ev = hp_to_ev(stats.hp.max(), base.hp(), level, IV(get_hp(dvs)));
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
						return CombinedStatsFor<generation>{nature, dvs, evs};
					}
				}
			}
		}
	} else {
		auto partial_ev_sum_is_valid = [](auto... evs) {
			return (... + evs.value()) <= max_total_evs(special_input_style_for(generation));
		};
		auto const hp_range = dv_or_iv_ev_range(
			dvs_or_ivs.hp,
			bounded::type<IV>,
			[=](IV const iv) { return hp_to_ev(stats.hp.max(), base.hp(), level, iv); }
		);
		for (auto const hp : hp_range) {
			for (auto const nature : nature_range) {
				auto const iv_ev_range = [=](SplitSpecialRegularStat const stat_name) {
					return dv_or_iv_ev_range(
						dvs_or_ivs[stat_name],
						bounded::type<IV>,
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
									return CombinedStatsFor<generation>{nature, ivs, evs};
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
			", Power: "sv, containers::to_string(hidden_power->power())
		) :
		containers::string("none");
	throw std::runtime_error(containers::concatenate<std::string>(
		"No Nature, IV, and EV combination gives the received stats in generation "sv,
		to_string(generation),
		": Species: "sv, to_string(species),
		", Level: "sv, containers::to_string(level()),
		", Possible Natures: "sv, nature_string,
		", "sv, to_string(stats),
		", Hidden Power: "sv, hidden_power_string
	));
}

export template<Generation generation>
constexpr auto calculate_ivs_and_evs(
	Species const species,
	Level const level,
	Stats<stat_style_for(generation)> const stats,
	tv::optional<HiddenPower<generation>> const hidden_power
) -> CombinedStatsFor<generation> {
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

export template<any_pokemon PokemonType>
constexpr auto calculate_ivs_and_evs(PokemonType const & pokemon) {
	auto const nature = pokemon.nature();
	return calculate_ivs_and_evs(
		pokemon.species(),
		pokemon.level(),
		pokemon.stats(),
		pokemon.hidden_power(),
		containers::enum_range(nature, nature)
	);
}

} // namespace technicalmachine
