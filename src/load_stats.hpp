// Header for loading stats like Pokemon usages
// Copyright (C) 2012 David Stone
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

#ifndef LOAD_STATS_HPP_
#define LOAD_STATS_HPP_

#include <vector>
#include "species.hpp"

namespace technicalmachine {

std::vector <unsigned> overall_stats ();

void team_stats (std::vector<unsigned> const & overall, unsigned const total, float multiplier [Species::END][Species::END]);

// Multiplier for Pokemon after you've seen the lead
std::vector <float> lead_stats ();

void detailed_stats (int detailed [][7]);

}	// namespace technicalmachine
#endif	// LOAD_STATS_HPP_
