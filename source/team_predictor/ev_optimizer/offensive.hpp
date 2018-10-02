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

#include "../../pokemon/pokemon.hpp"
#include "../../stat/calculate.hpp"
#include "../../stat/ev.hpp"
#include "../../stat/nature.hpp"

#include <bounded/integer.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
struct Pokemon;
using namespace bounded::literal;

struct OffensiveEVs {
	explicit OffensiveEVs(Pokemon const & pokemon);

	auto find(Nature const nature) const {
		return containers::maybe_find_if(m_container, [=](auto const value) { return value.nature == nature; });
	}
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
	struct OffensiveStats {
		constexpr explicit OffensiveStats(Nature const nature_):
			nature(nature_)
		{
		}

		Nature nature;
		EV attack = EV(0_bi);
		EV special_attack = EV(0_bi);
	};
	containers::static_vector<OffensiveStats, size(containers::enum_range<Nature>()).value()> m_container;
};

inline bool has_physical_move(Pokemon const & pokemon) {
	return containers::any(regular_moves(pokemon), is_physical);
}

inline bool has_special_move(Pokemon const & pokemon) {
	return containers::any(regular_moves(pokemon), is_special);
}

}	// namespace technicalmachine
