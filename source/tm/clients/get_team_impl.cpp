// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module tm.clients.get_team;

import tm.clients.load_team_from_file;

import tm.stat.stat_style;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.team_predictor;

import tm.constant_generation;
import tm.generation;
import tm.generation_generic;
import tm.initial_team;
import tm.settings_file;
import tm.team;

import bounded;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

auto get_team(Generation const generation, SettingsFile::Team const & source, AllUsageStats const & all_usage_stats, std::mt19937 & random_engine) -> tv::optional<GenerationGeneric<KnownTeam>> {
	return tv::visit(source, tv::overload(
		[&](SettingsFile::NoTeam) -> tv::optional<GenerationGeneric<KnownTeam>> { return tv::none; },
		[&](SettingsFile::GenerateTeam) -> tv::optional<GenerationGeneric<KnownTeam>> {
			auto & usage_stats = all_usage_stats[generation];
			return constant_generation(generation, [&]<Generation g>(constant_gen_t<g>) {
				return GenerationGeneric<KnownTeam>(
					KnownTeam<g>(random_team<g>(usage_stats, random_engine))
				);
			});
		},
		[&](std::filesystem::path const & path) -> tv::optional<GenerationGeneric<KnownTeam>> {
			auto const team = load_random_team_from_directory(random_engine, path);
			return tv::visit(team, [&]<SpecialInputStyle style>(InitialTeam<style> const & t) {
				return constant_generation(generation, [&]<Generation g>(constant_gen_t<g>) -> GenerationGeneric<KnownTeam> {
					if constexpr (special_input_style_for(g) != style) {
						throw std::runtime_error("Invalid team for generation");
					} else {
						return KnownTeam<g>(t);
					}
				});
			});
		}
	));
}

} // namespace technicalmachine
