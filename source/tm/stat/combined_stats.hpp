// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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

	friend auto operator==(CombinedStats const &, CombinedStats const &) -> bool = default;
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
