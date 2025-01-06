// Copyright David Stone 2025.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.make_stats;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.initial_stat;
import tm.stat.hp;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_names;
import tm.stat.stat_style;
import tm.stat.stats;

namespace technicalmachine {

export template<StatStyle stat_style>
constexpr auto make_stats(
	BaseStats const base,
	Level const level,
	CombinedStats<to_input_style(stat_style)> const inputs
) -> Stats<stat_style> {
	auto const hp = HP(base.hp(), level, IV(get_hp(inputs.dvs_or_ivs)), inputs.evs.hp);
	if constexpr (stat_style == StatStyle::gen1) {
		auto make = [=](auto const stat_name) {
			return initial_stat(
				base[stat_name],
				level,
				IV(inputs.dvs_or_ivs[stat_name]),
				inputs.evs[stat_name]
			);
		};
		return Stats<stat_style>{
			.hp = hp,
			.atk = make(SpecialRegularStat::atk),
			.def = make(SpecialRegularStat::def),
			.spe = make(SpecialRegularStat::spe),
			.spc = make(SpecialRegularStat::spc),
		};
	} else if constexpr (stat_style == StatStyle::gen2) {
		auto make = [=](auto const stat_name) {
			return initial_stat(
				base[stat_name],
				level,
				IV(inputs.dvs_or_ivs[to_combined(stat_name)]),
				inputs.evs[to_combined(stat_name)]
			);
		};
		return Stats<stat_style>{
			.hp = hp,
			.atk = make(SplitSpecialRegularStat::atk),
			.def = make(SplitSpecialRegularStat::def),
			.spa = make(SplitSpecialRegularStat::spa),
			.spd = make(SplitSpecialRegularStat::spd),
			.spe = make(SplitSpecialRegularStat::spe),
		};
	} else {
		auto make = [=](auto const stat_name) {
			return initial_stat(
				base[stat_name],
				level,
				to_nature_effect(inputs.nature, stat_name),
				inputs.dvs_or_ivs[stat_name],
				inputs.evs[stat_name]
			);
		};
		return Stats<stat_style>{
			.hp = hp,
			.atk = make(SplitSpecialRegularStat::atk),
			.def = make(SplitSpecialRegularStat::def),
			.spa = make(SplitSpecialRegularStat::spa),
			.spd = make(SplitSpecialRegularStat::spd),
			.spe = make(SplitSpecialRegularStat::spe),
		};
	}
}

} // namespace technicalmachine
