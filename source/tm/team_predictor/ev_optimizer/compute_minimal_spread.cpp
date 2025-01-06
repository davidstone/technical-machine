// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.team_predictor.ev_optimizer.compute_minimal_spread;

import tm.team_predictor.ev_optimizer.defensive;
import tm.team_predictor.ev_optimizer.offensive;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;
import tm.team_predictor.ev_optimizer.speed;

import tm.pokemon.hidden_power;
import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.nature_effect;
import tm.stat.possible_dvs_or_ivs;
import tm.stat.stat_style;
import tm.stat.stats;

import tm.generation;

import bounded;
import containers;
import tv;

namespace technicalmachine {
using namespace bounded::literal;

using enum NatureEffect;

constexpr auto to_nature(
	NatureEffect const atk,
	NatureEffect const def,
	NatureEffect const spa,
	NatureEffect const spd,
	NatureEffect const spe
) -> Nature {
	using enum Nature;
	if (atk == positive) {
		if (def == negative) { return Lonely; }
		else if (spa == negative) { return Adamant; }
		else if (spd == negative) { return Naughty; }
		else if (spe == negative) { return Brave; }
		else { std::unreachable(); }
	} else if (def == positive) {
		if (atk == negative) { return Bold; }
		else if (spa == negative) { return Impish; }
		else if (spd == negative) { return Lax; }
		else if (spe == negative) { return Relaxed; }
		else { std::unreachable(); }
	} else if (spa == positive) {
		if (atk == negative) { return Modest; }
		else if (def == negative) { return Mild; }
		else if (spd == negative) { return Rash; }
		else if (spe == negative) { return Quiet; }
		else { std::unreachable(); }
	} else if (spd == positive) {
		if (atk == negative) { return Calm; }
		else if (def == negative) { return Gentle; }
		else if (spa == negative) { return Careful; }
		else if (spe == negative) { return Sassy; }
		else { std::unreachable(); }
	} else if (spe == positive) {
		if (atk == negative) { return Timid; }
		else if (def == negative) { return Hasty; }
		else if (spa == negative) { return Jolly; }
		else if (spd == negative) { return Naive; }
		else { std::unreachable(); }
	} else {
		return Hardy;
	}
}

template<Generation generation>
constexpr auto combine(
	OffensiveEVs const & o,
	DefensiveEVs const & d,
	SpeedEVs const & s,
	tv::optional<HiddenPower<generation>> const hidden_power
) -> CombinedStats<SpecialInputStyle::split> {
	auto best = tv::optional<CombinedStats<SpecialInputStyle::split>>();
	for (auto const spe : s) {
		auto compatible_offensive = [=](auto const offensive) {
			switch (spe.nature_effect) {
				case negative:
				case positive:
					return containers::none_equal(
						{offensive.atk.nature_effect, offensive.spa.nature_effect},
						spe.nature_effect
					);
				case neutral:
					return true;
			}
		};
		for (auto const offensive : containers::filter(o, compatible_offensive)) {
			auto const certain = containers::array({
				spe.nature_effect,
				offensive.atk.nature_effect,
				offensive.spa.nature_effect
			});
			auto const has_positive = containers::any_equal(
				certain,
				positive
			);
			auto const has_negative = containers::any_equal(
				certain,
				negative
			);
			auto compatible_defensive = [=](auto const defensive) {
				auto const candidate = containers::array({
					defensive.def.nature_effect,
					defensive.spd.nature_effect
				});
				return
					has_positive and has_negative ?
						containers::all_equal(candidate, neutral) :
					has_positive and !has_negative ?
						containers::any_equal(candidate, negative) and
						containers::none_equal(candidate, positive) :
					!has_positive and has_negative ?
						containers::none_equal(candidate, negative) and
						containers::any_equal(candidate, positive) :
					containers::any_equal(candidate, negative) ==
					containers::any_equal(candidate, positive);
			};
			for (auto const defensive : containers::filter(d, compatible_defensive)) {
				auto const candidate_ivs = IVs(
					defensive.hp.iv,
					offensive.atk.iv,
					defensive.def.iv,
					offensive.spa.iv,
					defensive.spd.iv,
					spe.iv
				);
				if (hidden_power and HiddenPower<generation>(candidate_ivs) != *hidden_power) {
					continue;
				}
				auto const candidate_evs = EVs(
					defensive.hp.ev,
					offensive.atk.ev,
					defensive.def.ev,
					offensive.spa.ev,
					defensive.spd.ev,
					spe.ev
				);
				if (!best or ev_sum(candidate_evs) < ev_sum(best->evs)) {
					insert(best, CombinedStats<SpecialInputStyle::split>(
						to_nature(
							offensive.atk.nature_effect,
							defensive.def.nature_effect,
							offensive.spa.nature_effect,
							defensive.spd.nature_effect,
							spe.nature_effect
						),
						candidate_ivs,
						candidate_evs
					));
				}
			}
		}
	}
	BOUNDED_ASSERT(best);
	BOUNDED_ASSERT(ev_sum(best->evs) <= max_total_evs(SpecialInputStyle::split));
	return *best;
}

constexpr auto get_attack(PossibleDVs const all, bool const include_attack) -> DV {
	auto const dvs = all.atk();
	auto const is_odd = [](DV const dv) -> bool {
		return dv.value() % 2_bi == 1_bi;
	};
	if (include_attack) {
		auto const ptr = containers::maybe_find_if(
			containers::reversed(dvs),
			is_odd
		);
		return ptr ? *ptr : containers::back(dvs);
	} else {
		auto const ptr = containers::maybe_find_if(
			dvs,
			is_odd
		);
		return ptr ? *ptr : containers::front(dvs);
	}
}

constexpr auto combined_special_minimal_spread(PossibleDVs const dvs, bool const include_attack) -> CombinedStats<SpecialInputStyle::combined> {
	return CombinedStats<SpecialInputStyle::combined>{
		Nature::Hardy,
		DVs(
			get_attack(dvs, include_attack),
			containers::back(dvs.def()),
			containers::back(dvs.spe()),
			containers::back(dvs.spc())
		),
		OldGenEVs(
			EV(EV::useful_max),
			include_attack ? EV(EV::useful_max) : EV(0_bi),
			EV(EV::useful_max),
			EV(EV::useful_max),
			EV(EV::useful_max)
		)
	};
}

template<Generation generation>
constexpr auto split_special_minimal_spread(
	BaseStats const base_stats,
	Stats<StatStyle::current> stats,
	Level const level,
	PossibleIVs const ivs,
	tv::optional<HiddenPower<generation>> const hidden_power,
	bool const include_attack,
	bool const include_special_attack
) -> CombinedStats<SpecialInputStyle::split> {
	return combine(
		OffensiveEVs(
			level,
			OffensiveEVAtk(
				base_stats.atk(),
				include_attack ?
					possible_optimized_ivs(ivs.atk()) :
					possible_optimized_ivs(containers::reversed(ivs.atk())),
				stats.atk(),
				include_attack
			),
			OffensiveEVSpA(
				base_stats.spa(),
				possible_optimized_ivs(ivs.spa()),
				stats.spa(),
				include_special_attack
			)
		),
		DefensiveEVs(
			level,
			DefensiveEVHP(
				base_stats.hp(),
				possible_optimized_ivs(ivs.hp()),
				stats.hp().max()
			),
			DefensiveEVDef(
				base_stats.def(),
				possible_optimized_ivs(ivs.def()),
				stats.def()
			),
			DefensiveEVSpD(
				base_stats.spd(),
				possible_optimized_ivs(ivs.spd()),
				stats.spd()
			)
		),
		SpeedEVs(
			level,
			{base_stats.spe(), possible_optimized_ivs(ivs.spe()), stats.spe()}
		),
		hidden_power
	);
}

export template<Generation generation>
constexpr auto compute_minimal_spread(
	BaseStats const base_stats,
	Stats<stat_style_for(generation)> stats,
	Level const level,
	tv::optional<HiddenPower<generation>> const hidden_power,
	bool const include_attack,
	bool const include_special_attack
) -> CombinedStatsFor<generation> {
	auto const dvs_or_ivs = possible_dvs_or_ivs(hidden_power);
	if constexpr (generation <= Generation::two) {
		return combined_special_minimal_spread(dvs_or_ivs, include_attack);
	} else {
		return split_special_minimal_spread(
			base_stats,
			stats,
			level,
			dvs_or_ivs,
			hidden_power,
			include_attack,
			include_special_attack
		);
	}
}

} // namespace technicalmachine
