// Optimize defensive EVs and nature to remove waste
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

#include <tm/team_predictor/ev_optimizer/defensive_data_point.hpp>

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/integer_range.hpp>
#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {

enum class Generation : std::uint8_t;

struct DefensiveEVs {
	DefensiveEVs(Generation, BaseStats, Species, Level, Nature, HP, Stat defense, Stat special_defense);
	friend auto begin(DefensiveEVs const & defensive) {
		return begin(defensive.m_container);
	}
	friend auto end(DefensiveEVs const & defensive) {
		return end(defensive.m_container);
	}
	auto find(Nature const nature) const {
		return containers::find_if(m_container, [=](auto const value) { return value.nature == nature; });
	}
private:
	static constexpr auto number_of_natures = size(containers::enum_range<Nature>());
	containers::static_vector<DataPoint, number_of_natures.value()> m_container;
};

}	// namespace technicalmachine
