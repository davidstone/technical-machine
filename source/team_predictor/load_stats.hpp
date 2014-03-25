// Header for loading stats like Pokemon usages
// Copyright (C) 2014 David Stone
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

#ifndef TEAM_PREDICTOR__LOAD_STATS_HPP_
#define TEAM_PREDICTOR__LOAD_STATS_HPP_

#include "../pokemon/species.hpp"

#include <bounded_integer/array.hpp>

namespace technicalmachine {

auto overall_stats() -> bounded_integer::array<unsigned, number_of_species>;

// Multiplier for Pokemon after you've seen the lead
auto lead_stats() -> bounded_integer::array<float, number_of_species>;

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__LOAD_STATS_HPP_
