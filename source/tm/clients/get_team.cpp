// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.get_team;

import tm.clients.load_team_from_file;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.team_predictor;

import tm.generation;
import tm.settings_file;
import tm.team;

import bounded;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
auto get_team(SettingsFile::Team const & source, AllUsageStats const & all_usage_stats, std::mt19937 & random_engine) -> tv::optional<KnownTeam<generation>> {
	return tv::visit(source, tv::overload(
		[&](SettingsFile::NoTeam) -> tv::optional<KnownTeam<generation>> { return tv::none; },
		[&](SettingsFile::GenerateTeam) -> tv::optional<KnownTeam<generation>> {
			return KnownTeam<generation>(random_team<generation>(
				all_usage_stats[generation],
				random_engine
			));
		},
		[&](std::filesystem::path const & path) -> tv::optional<KnownTeam<generation>> {
			auto const team = load_random_team_from_directory(random_engine, path);
			constexpr auto type = bounded::type<KnownTeam<generation>>;
			if (team.index() != type) {
				throw std::runtime_error("Generation mismatch in team file vs. battle.");
			}
			return team[type];
		}
	));
}

} // namespace technicalmachine
