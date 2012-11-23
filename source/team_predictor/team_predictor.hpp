// Predict foe's team header
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

#ifndef TEAM_PREDICTOR__TEAM_PREDICTOR_HPP_
#define TEAM_PREDICTOR__TEAM_PREDICTOR_HPP_

#include <random>

namespace technicalmachine {
class DetailedStats;
class Team;

Team predict_team(DetailedStats const & detailed, Team team, std::mt19937 & random_engine, bool using_lead = true);

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__TEAM_PREDICTOR_HPP_
