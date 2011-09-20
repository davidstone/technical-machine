// Header for loading stats like Pokemon usages
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef LEAD_STATS_H_
#define LEAD_STATS_H_

#include <vector>
#include "species.h"

namespace technicalmachine {

class Map;

void overall_stats (std::vector<unsigned> & overall);

void team_stats (std::vector<unsigned> const & overall, unsigned const total, float multiplier [END_SPECIES][END_SPECIES]);

void lead_stats (std::vector<float> & lead);

void detailed_stats (int detailed [][7]);

}

#endif
