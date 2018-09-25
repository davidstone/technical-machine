// Optimize offensive EVs and nature to remove waste
// Copyright (C) 2018 David Stone
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

#include "../../stat/calculate.hpp"
#include "../../stat/ev.hpp"
#include "../../stat/nature.hpp"

#include <bounded/integer.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
struct Pokemon;
struct DefensiveEVs;
struct SpeedEVs;
using namespace bounded::literal;

struct OffensiveEVs {
	explicit OffensiveEVs(Pokemon const & pokemon);
private:
	auto optimize(Pokemon const & pokemon) -> void;
	struct OffensiveData {
	private:
		using StatType = decltype(initial_stat(std::declval<StatNames>(), std::declval<Stat>(), std::declval<Level>(), std::declval<Nature>()));
	public:
		StatType atk;
		StatType spa;
	};
	auto equal_stats(OffensiveData initial, Species species, Level level) -> void;
	friend auto combine(OffensiveEVs const & offensive, DefensiveEVs const & defensive, SpeedEVs const & speed, Pokemon & pokemon) -> void;
	struct OffensiveStats {
		constexpr explicit OffensiveStats(Nature const nature_):
			nature(nature_)
		{
		}

		Nature nature;
		EV attack = EV(0_bi);
		EV special_attack = EV(0_bi);
	};
	containers::static_vector<OffensiveStats, size(containers::enum_range<Nature>()).value()> container;
};

}	// namespace technicalmachine
