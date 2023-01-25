// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.log_foe_team;

import tm.clients.random_string;
import tm.clients.write_team;

import tm.team_predictor.team_predictor;
import tm.team_predictor.usage_stats;

import tm.any_team;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

auto team_file_directory() {
	return std::filesystem::path("teams/foe");
}

auto generate_team_file_name(std::mt19937 & random_engine, std::string_view const extension) -> std::filesystem::path {
	// Randomly generates a file name in 8.3 format. It then checks to see if
	// that file name already exists. If it does, it randomly generates a new
	// file name, and continues until it generates a name that does not exist.
	// This limits the potential for a race condition down to a 1 / 36^8 chance
	// (about 1 / 2 ^ 41), assuming that another process / thread is also
	// trying to save an 8 character file name with an identical extension at
	// the same time. The result of this is that a team file would not be saved
	// when it should have been, which is not a major issue.
	constexpr auto file_name_length = 8_bi;
	std::filesystem::path foe_team_file;
	do {
		foe_team_file = team_file_directory();
		foe_team_file /= std::string_view(containers::concatenate<containers::string>(random_string(random_engine, file_name_length), extension));
	} while (std::filesystem::exists(foe_team_file));
	return foe_team_file;
}

export auto log_foe_team(UsageStats const & usage_stats, any_seen_team auto const & foe_team, std::mt19937 & random_engine, WriteTeam const & write_team) -> void {
	auto const path = generate_team_file_name(random_engine, write_team.extension);
	std::filesystem::create_directory(team_file_directory());
	auto const team = most_likely_team(usage_stats, random_engine, foe_team);
	write_team.function(GenerationGeneric<Team>(team), path);
}

} // namespace technicalmachine
