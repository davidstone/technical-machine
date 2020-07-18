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

#include <tm/clients/pokemon_lab/write_team_file.hpp>

#include <tm/team_predictor/team_predictor.hpp>

#include <filesystem>
#include <random>

namespace technicalmachine {

template<Generation>
struct Team;

struct UsageStats;

inline auto team_file_directory() {
	return std::filesystem::path("teams/foe");
}

std::filesystem::path generate_team_file_name(std::mt19937 & random_engine);

template<Generation generation>
void log_foe_team(UsageStats const & usage_stats, Team<generation> const & foe_team, std::mt19937 & random_engine) {
	auto const team = predict_team(usage_stats, use_lead_stats, foe_team, random_engine);
	auto const path = generate_team_file_name(random_engine);
	std::filesystem::create_directory(team_file_directory());
	pl::write_team(team, path);
}

}	// namespace technicalmachine
