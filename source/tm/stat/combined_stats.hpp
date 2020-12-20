// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/generic_stats.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/operators.hpp>

namespace technicalmachine {

template<typename T>
struct CombinedStats {
	Nature nature;
	T hp;
	T atk;
	T def;
	T spa;
	T spd;
	T spe;

	constexpr auto const & operator[](PermanentStat const name) const {
		return index_stat(*this, name);
	}
	constexpr auto & operator[](PermanentStat const name) {
		return index_stat(*this, name);
	}

	friend auto operator==(CombinedStats, CombinedStats) -> bool = default;
};

constexpr auto combine(Nature const nature, GenericStats<IV> const ivs, GenericStats<EV> const evs) {
	return CombinedStats<IVAndEV>{
		nature,
		{ivs.hp, evs.hp},
		{ivs.atk, evs.atk},
		{ivs.def, evs.def},
		{ivs.spa, evs.spa},
		{ivs.spd, evs.spd},
		{ivs.spe, evs.spe},
	};
}

constexpr auto ev_sum(CombinedStats<IVAndEV> const stats) {
	constexpr auto impl = [](auto... args) { return (... + args.ev.value()); };
	return impl(stats.hp, stats.atk, stats.def, stats.spa, stats.spd, stats.spe);
}

}	// namespace technicalmachine
