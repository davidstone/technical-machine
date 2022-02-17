// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <bounded/integer.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

enum class BoostableStat {
	atk,
	def,
	spa,
	spd,
	spe,
	acc,
	eva,
};

enum class SpecialPermanentStat {
	hp = -1,
	atk,
	def,
	spe,
	spc,
};

enum class SplitSpecialPermanentStat {
	hp = -1,
	atk,
	def,
	spa,
	spd,
	spe,
};

enum class SpecialRegularStat {
	atk,
	def,
	spe,
	spc,
};

enum class SplitSpecialRegularStat {
	atk,
	def,
	spa,
	spd,
	spe,
};

constexpr decltype(auto) index_stat(auto && stats, SpecialPermanentStat const index) {
	switch (index) {
		case SpecialPermanentStat::hp: return OPERATORS_FORWARD(stats).hp();
		case SpecialPermanentStat::atk: return OPERATORS_FORWARD(stats).atk();
		case SpecialPermanentStat::def: return OPERATORS_FORWARD(stats).def();
		case SpecialPermanentStat::spe: return OPERATORS_FORWARD(stats).spe();
		case SpecialPermanentStat::spc: return OPERATORS_FORWARD(stats).spc();
	}
}

constexpr decltype(auto) index_stat(auto && stats, SplitSpecialPermanentStat const index) {
	switch (index) {
		case SplitSpecialPermanentStat::hp: return OPERATORS_FORWARD(stats).hp();
		case SplitSpecialPermanentStat::atk: return OPERATORS_FORWARD(stats).atk();
		case SplitSpecialPermanentStat::def: return OPERATORS_FORWARD(stats).def();
		case SplitSpecialPermanentStat::spa: return OPERATORS_FORWARD(stats).spa();
		case SplitSpecialPermanentStat::spd: return OPERATORS_FORWARD(stats).spd();
		case SplitSpecialPermanentStat::spe: return OPERATORS_FORWARD(stats).spe();
	}
}

constexpr decltype(auto) index_stat(auto && stats, SpecialRegularStat const index) {
	switch (index) {
		case SpecialRegularStat::atk: return OPERATORS_FORWARD(stats).atk();
		case SpecialRegularStat::def: return OPERATORS_FORWARD(stats).def();
		case SpecialRegularStat::spe: return OPERATORS_FORWARD(stats).spe();
		case SpecialRegularStat::spc: return OPERATORS_FORWARD(stats).spc();
	}
}

constexpr decltype(auto) index_stat(auto && stats, SplitSpecialRegularStat const index) {
	switch (index) {
		case SplitSpecialRegularStat::atk: return OPERATORS_FORWARD(stats).atk();
		case SplitSpecialRegularStat::def: return OPERATORS_FORWARD(stats).def();
		case SplitSpecialRegularStat::spa: return OPERATORS_FORWARD(stats).spa();
		case SplitSpecialRegularStat::spd: return OPERATORS_FORWARD(stats).spd();
		case SplitSpecialRegularStat::spe: return OPERATORS_FORWARD(stats).spe();
	}
}

} // namespace technicalmachine

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::BoostableStat> = technicalmachine::BoostableStat::atk;

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::BoostableStat> = technicalmachine::BoostableStat::eva;

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::SpecialPermanentStat> = technicalmachine::SpecialPermanentStat::hp;

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::SpecialPermanentStat> = technicalmachine::SpecialPermanentStat::spc;

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::SplitSpecialPermanentStat> = technicalmachine::SplitSpecialPermanentStat::hp;

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::SplitSpecialPermanentStat> = technicalmachine::SplitSpecialPermanentStat::spe;

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::SpecialRegularStat> = technicalmachine::SpecialRegularStat::atk;

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::SpecialRegularStat> = technicalmachine::SpecialRegularStat::spc;

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::SplitSpecialRegularStat> = technicalmachine::SplitSpecialRegularStat::atk;

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::SplitSpecialRegularStat> = technicalmachine::SplitSpecialRegularStat::spe;
