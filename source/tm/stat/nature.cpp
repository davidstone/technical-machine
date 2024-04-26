// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.stat.nature;

import tm.stat.nature_effect;
import tm.stat.stat_names;

import tm.rational;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export enum class Nature : std::uint8_t {
	Adamant,
	Bashful,
	Bold,
	Brave,
	Calm,
	Careful,
	Docile,
	Gentle,
	Hardy,
	Hasty,
	Impish,
	Jolly,
	Lax,
	Lonely,
	Mild,
	Modest,
	Naive,
	Naughty,
	Quiet,
	Quirky,
	Rash,
	Relaxed,
	Sassy,
	Serious,
	Timid,
};

export constexpr auto boosts_stat(Nature const nature, SplitSpecialRegularStat const stat) -> bool {
	switch (nature) {
		case Nature::Adamant:
		case Nature::Brave:
		case Nature::Lonely:
		case Nature::Naughty:
			return stat == SplitSpecialRegularStat::atk;
		case Nature::Bold:
		case Nature::Impish:
		case Nature::Lax:
		case Nature::Relaxed:
			return stat == SplitSpecialRegularStat::def;
		case Nature::Mild:
		case Nature::Modest:
		case Nature::Quiet:
		case Nature::Rash:
			return stat == SplitSpecialRegularStat::spa;
		case Nature::Calm:
		case Nature::Careful:
		case Nature::Gentle:
		case Nature::Sassy:
			return stat == SplitSpecialRegularStat::spd;
		case Nature::Hasty:
		case Nature::Jolly:
		case Nature::Naive:
		case Nature::Timid:
			return stat == SplitSpecialRegularStat::spe;
		case Nature::Bashful:
		case Nature::Docile:
		case Nature::Hardy:
		case Nature::Quirky:
		case Nature::Serious:
			return false;
	}
}

export constexpr auto lowers_stat(Nature const nature, SplitSpecialRegularStat const stat) -> bool {
	switch (nature) {
		case Nature::Bold:
		case Nature::Calm:
		case Nature::Modest:
		case Nature::Timid:
			return stat == SplitSpecialRegularStat::atk;
		case Nature::Gentle:
		case Nature::Hasty:
		case Nature::Lonely:
		case Nature::Mild:
			return stat == SplitSpecialRegularStat::def;
		case Nature::Adamant:
		case Nature::Careful:
		case Nature::Impish:
		case Nature::Jolly:
			return stat == SplitSpecialRegularStat::spa;
		case Nature::Lax:
		case Nature::Naive:
		case Nature::Naughty:
		case Nature::Rash:
			return stat == SplitSpecialRegularStat::spd;
		case Nature::Brave:
		case Nature::Quiet:
		case Nature::Relaxed:
		case Nature::Sassy:
			return stat == SplitSpecialRegularStat::spe;
		case Nature::Bashful:
		case Nature::Docile:
		case Nature::Hardy:
		case Nature::Quirky:
		case Nature::Serious:
			return false;
	}
}

export constexpr auto boosts_attacking_stat(Nature const nature) -> bool {
	return boosts_stat(nature, SplitSpecialRegularStat::atk) or boosts_stat(nature, SplitSpecialRegularStat::spa);
}

export constexpr auto boosts_defending_stat(Nature const nature) -> bool {
	return boosts_stat(nature, SplitSpecialRegularStat::def) or boosts_stat(nature, SplitSpecialRegularStat::spd);
}

export constexpr auto lowers_attacking_stat(Nature const nature) -> bool {
	return lowers_stat(nature, SplitSpecialRegularStat::atk) or lowers_stat(nature, SplitSpecialRegularStat::spa);
}

export constexpr auto lowers_defending_stat(Nature const nature) -> bool {
	return lowers_stat(nature, SplitSpecialRegularStat::def) or lowers_stat(nature, SplitSpecialRegularStat::spd);
}

export constexpr auto to_nature_effect(Nature const nature, SplitSpecialRegularStat const stat) -> NatureEffect {
	return 
		boosts_stat(nature, stat) ? NatureEffect::positive :
		lowers_stat(nature, stat) ? NatureEffect::negative :
		NatureEffect::neutral;
}

export constexpr auto boost(Nature const nature, SplitSpecialRegularStat const stat) {
	return boost(to_nature_effect(nature, stat));
}

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::Nature> = technicalmachine::Nature();

template<>
constexpr auto numeric_traits::max_value<technicalmachine::Nature> = technicalmachine::Nature::Timid;
