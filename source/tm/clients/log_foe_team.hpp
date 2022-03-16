// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_lab/write_team_file.hpp>

#include <tm/team_predictor/team_predictor.hpp>

#include <tm/any_team.hpp>

#include <filesystem>
#include <random>

namespace technicalmachine {

struct UsageStats;

inline auto team_file_directory() {
	return std::filesystem::path("teams/foe");
}

std::filesystem::path generate_team_file_name(std::mt19937 & random_engine);

void log_foe_team(UsageStats const & usage_stats, any_seen_team auto const & foe_team, std::mt19937 & random_engine) {
	auto const team = most_likely_team(usage_stats, random_engine, foe_team);
	auto const path = generate_team_file_name(random_engine);
	std::filesystem::create_directory(team_file_directory());
	pl::write_team(team, path);
}

} // namespace technicalmachine
