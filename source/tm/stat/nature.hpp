// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/stat_names.hpp>

#include <tm/rational.hpp>

#include <bounded/integer.hpp>

#include <cstdint>
#include <functional>
#include <type_traits>

namespace technicalmachine {
using namespace bounded::literal;

enum class Nature : std::uint8_t {
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

constexpr auto boosts_stat(Nature const nature, RegularStat const stat) -> bool {
	switch (nature) {
		case Nature::Adamant:
		case Nature::Brave:
		case Nature::Lonely:
		case Nature::Naughty:
			return stat == RegularStat::atk;
		case Nature::Bold:
		case Nature::Impish:
		case Nature::Lax:
		case Nature::Relaxed:
			return stat == RegularStat::def;
		case Nature::Mild:
		case Nature::Modest:
		case Nature::Quiet:
		case Nature::Rash:
			return stat == RegularStat::spa;
		case Nature::Calm:
		case Nature::Careful:
		case Nature::Gentle:
		case Nature::Sassy:
			return stat == RegularStat::spd;
		case Nature::Hasty:
		case Nature::Jolly:
		case Nature::Naive:
		case Nature::Timid:
			return stat == RegularStat::spe;
		case Nature::Bashful:
		case Nature::Docile:
		case Nature::Hardy:
		case Nature::Quirky:
		case Nature::Serious:
			return false;
	}
}

constexpr auto lowers_stat(Nature const nature, RegularStat const stat) -> bool {
	switch (nature) {
		case Nature::Bold:
		case Nature::Calm:
		case Nature::Modest:
		case Nature::Timid:
			return stat == RegularStat::atk;
		case Nature::Gentle:
		case Nature::Hasty:
		case Nature::Lonely:
		case Nature::Mild:
			return stat == RegularStat::def;
		case Nature::Adamant:
		case Nature::Careful:
		case Nature::Impish:
		case Nature::Jolly:
			return stat == RegularStat::spa;
		case Nature::Lax:
		case Nature::Naive:
		case Nature::Naughty:
		case Nature::Rash:
			return stat == RegularStat::spd;
		case Nature::Brave:
		case Nature::Quiet:
		case Nature::Relaxed:
		case Nature::Sassy:
			return stat == RegularStat::spe;
		case Nature::Bashful:
		case Nature::Docile:
		case Nature::Hardy:
		case Nature::Quirky:
		case Nature::Serious:
			return false;
	}
}

constexpr auto boosts_attacking_stat(Nature const nature) -> bool {
	return boosts_stat(nature, RegularStat::atk) or boosts_stat(nature, RegularStat::spa);
}

constexpr auto boosts_defending_stat(Nature const nature) -> bool {
	return boosts_stat(nature, RegularStat::def) or boosts_stat(nature, RegularStat::spd);
}

constexpr auto lowers_attacking_stat(Nature const nature) -> bool {
	return lowers_stat(nature, RegularStat::atk) or lowers_stat(nature, RegularStat::spa);
}

constexpr auto lowers_defending_stat(Nature const nature) -> bool {
	return lowers_stat(nature, RegularStat::def) or lowers_stat(nature, RegularStat::spd);
}

constexpr auto boost(Nature const nature, RegularStat const stat) {
	auto const numerator =
		BOUNDED_CONDITIONAL(boosts_stat(nature, stat), 11_bi,
		BOUNDED_CONDITIONAL(lowers_stat(nature, stat), 9_bi,
		10_bi
	));
	return rational(numerator, 10_bi);
}

}	// namespace technicalmachine
namespace bounded {

template<>
inline constexpr auto min_value<technicalmachine::Nature> = technicalmachine::Nature();

template<>
inline constexpr auto max_value<technicalmachine::Nature> = technicalmachine::Nature::Timid;

}	// namespace bounded
namespace std {

template<>
struct hash<technicalmachine::Nature> {
	auto operator()(technicalmachine::Nature const e) const {
		using T = std::underlying_type_t<technicalmachine::Nature>;
		return std::hash<T>{}(static_cast<T>(e));
	}
};

}	// namespace std

