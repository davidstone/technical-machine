// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.get_team;

import tm.team_predictor.all_usage_stats;

import tm.generation;
import tm.generation_generic;
import tm.settings_file;
import tm.team;

import tv;
import std_module;

namespace technicalmachine {

export auto get_team(Generation const generation, SettingsFile::Team const & source, AllUsageStats const & all_usage_stats, std::mt19937 & random_engine) -> tv::optional<GenerationGeneric<KnownTeam>>;

} // namespace technicalmachine
