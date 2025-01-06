// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.stat_names;

import numeric_traits;
import std_module;

namespace technicalmachine {

export enum class BoostableStat {
	atk,
	def,
	spa,
	spd,
	spe,
	acc,
	eva,
};

export enum class SpecialPermanentStat {
	hp = -1,
	atk,
	def,
	spe,
	spc,
};

export enum class SplitSpecialPermanentStat {
	hp = -1,
	atk,
	def,
	spa,
	spd,
	spe,
};

export enum class SpecialRegularStat {
	atk,
	def,
	spe,
	spc,
};

export enum class SplitSpecialRegularStat {
	atk,
	def,
	spa,
	spd,
	spe,
};

export constexpr auto to_combined(SplitSpecialRegularStat const stat_name) -> SpecialRegularStat {
	switch (stat_name) {
		case SplitSpecialRegularStat::atk: return SpecialRegularStat::atk;
		case SplitSpecialRegularStat::def: return SpecialRegularStat::def;
		case SplitSpecialRegularStat::spa: return SpecialRegularStat::spc;
		case SplitSpecialRegularStat::spd: return SpecialRegularStat::spc;
		case SplitSpecialRegularStat::spe: return SpecialRegularStat::spe;
	}
}

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::BoostableStat> = technicalmachine::BoostableStat::atk;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::BoostableStat> = technicalmachine::BoostableStat::eva;

template<>
constexpr auto numeric_traits::min_value<technicalmachine::SpecialPermanentStat> = technicalmachine::SpecialPermanentStat::hp;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::SpecialPermanentStat> = technicalmachine::SpecialPermanentStat::spc;

template<>
constexpr auto numeric_traits::min_value<technicalmachine::SplitSpecialPermanentStat> = technicalmachine::SplitSpecialPermanentStat::hp;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::SplitSpecialPermanentStat> = technicalmachine::SplitSpecialPermanentStat::spe;

template<>
constexpr auto numeric_traits::min_value<technicalmachine::SpecialRegularStat> = technicalmachine::SpecialRegularStat::atk;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::SpecialRegularStat> = technicalmachine::SpecialRegularStat::spc;

template<>
constexpr auto numeric_traits::min_value<technicalmachine::SplitSpecialRegularStat> = technicalmachine::SplitSpecialRegularStat::atk;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::SplitSpecialRegularStat> = technicalmachine::SplitSpecialRegularStat::spe;


namespace technicalmachine {

constexpr auto same_int(BoostableStat const boostable, SplitSpecialPermanentStat const permanent, SplitSpecialRegularStat const regular) {
	return std::to_underlying(boostable) == std::to_underlying(permanent) and std::to_underlying(boostable) == std::to_underlying(regular);
}

static_assert(same_int(BoostableStat::atk, SplitSpecialPermanentStat::atk, SplitSpecialRegularStat::atk));
static_assert(same_int(BoostableStat::def, SplitSpecialPermanentStat::def, SplitSpecialRegularStat::def));
static_assert(same_int(BoostableStat::spa, SplitSpecialPermanentStat::spa, SplitSpecialRegularStat::spa));
static_assert(same_int(BoostableStat::spd, SplitSpecialPermanentStat::spd, SplitSpecialRegularStat::spd));
static_assert(same_int(BoostableStat::spe, SplitSpecialPermanentStat::spe, SplitSpecialRegularStat::spe));

} // namespace technicalmachine