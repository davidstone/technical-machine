// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/stat_names.hpp>

namespace technicalmachine {

template<typename HP, typename Stat = HP>
struct GenericStats {
	HP hp;
	Stat atk;
	Stat def;
	Stat spa;
	Stat spd;
	Stat spe;
	
	constexpr auto operator[](auto const index) const -> Stat const & {
		return index_stat(*this, index);
	}
	constexpr auto operator[](auto const index) -> Stat & {
		return index_stat(*this, index);
	}
	
	friend auto operator==(GenericStats, GenericStats) -> bool = default;
};

template<typename HP, typename Stat>
GenericStats(HP, Stat, Stat, Stat, Stat, Stat) -> GenericStats<HP, Stat>;

using IVs = GenericStats<IV>;
using EVs = GenericStats<EV>;

}	// namespace technicalmachine
