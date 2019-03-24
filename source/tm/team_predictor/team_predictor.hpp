// Predict foe's team
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

#include <tm/team_predictor/lead_stats.hpp>

#include <tm/team.hpp>

#include <random>

namespace technicalmachine {

enum class Generation;
struct UsageStats;

Team predict_team(Generation generation, UsageStats const & usage_stats, LeadStats lead_stats, Team team, std::mt19937 & random_engine);

}	// namespace technicalmachine
