// Stats data structures
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

// All three of these can convert to each other
enum class BoostableStat {
	atk,
	def,
	spa,
	spd,
	spe,
	acc,
	eva,
};

enum class PermanentStat {
	hp = -1,
	atk,
	def,
	spa,
	spd,
	spe,
};

enum class RegularStat {
	atk,
	def,
	spa,
	spd,
	spe,
};

constexpr auto && index_stat(auto && stats, PermanentStat const index) {
	switch (index) {
		case PermanentStat::hp: return OPERATORS_FORWARD(stats).hp;
		case PermanentStat::atk: return OPERATORS_FORWARD(stats).atk;
		case PermanentStat::def: return OPERATORS_FORWARD(stats).def;
		case PermanentStat::spa: return OPERATORS_FORWARD(stats).spa;
		case PermanentStat::spd: return OPERATORS_FORWARD(stats).spd;
		case PermanentStat::spe: return OPERATORS_FORWARD(stats).spe;
	}
}

constexpr auto && index_stat(auto && stats, RegularStat const index) {
	switch (index) {
		case RegularStat::atk: return OPERATORS_FORWARD(stats).atk;
		case RegularStat::def: return OPERATORS_FORWARD(stats).def;
		case RegularStat::spa: return OPERATORS_FORWARD(stats).spa;
		case RegularStat::spd: return OPERATORS_FORWARD(stats).spd;
		case RegularStat::spe: return OPERATORS_FORWARD(stats).spe;
	}
}

}	// namespace technicalmachine
namespace numeric_traits {

template<>
inline constexpr auto min_value<technicalmachine::BoostableStat> = technicalmachine::BoostableStat::atk;

template<>
inline constexpr auto max_value<technicalmachine::BoostableStat> = technicalmachine::BoostableStat::eva;

template<>
inline constexpr auto min_value<technicalmachine::PermanentStat> = technicalmachine::PermanentStat::hp;

template<>
inline constexpr auto max_value<technicalmachine::PermanentStat> = technicalmachine::PermanentStat::spe;

template<>
inline constexpr auto min_value<technicalmachine::RegularStat> = technicalmachine::RegularStat::atk;

template<>
inline constexpr auto max_value<technicalmachine::RegularStat> = technicalmachine::RegularStat::spe;

}	// namespace numeric_traits
