// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <string_view>

import tm.ps_usage_stats.battle_result;
import tm.ps_usage_stats.battle_result_reader;
import tm.ps_usage_stats.glicko1;
import tm.ps_usage_stats.mode;
import tm.ps_usage_stats.parallel_for_each;
import tm.ps_usage_stats.rating;
import tm.ps_usage_stats.serialize;
import tm.ps_usage_stats.thread_count;
import tm.ps_usage_stats.usage_stats;

import tm.string_conversions.generation;

import tm.generation;
import tm.open_file;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {
namespace {

using namespace std::string_view_literals;

struct ParsedArgs {
	Mode mode;
	Generation generation;
	ThreadCount thread_count;
	std::filesystem::path teams_file_path;
	std::filesystem::path output_stats_path;
};

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	if (argc != 6) {
		throw std::runtime_error(
			"Usage is ps_usage_stats output_path mode generation thread_count teams_file_path\n"
			"mode must be one of: unweighted, weighted, weighted_winner, top_players\n"
		);
	}
	auto const output_path = std::filesystem::path(argv[1]);
	auto output_stats_path = output_path / "stats.tmus";
	if (std::filesystem::exists(output_stats_path)) {
		throw std::runtime_error(containers::concatenate<std::string>(output_stats_path.string(), " already exists"sv));
	}
	auto const mode = parse_mode(argv[2]);
	auto const generation = from_string<Generation>(argv[3]);
	auto const thread_count = bounded::to_integer<ThreadCount>(argv[4]);
	auto teams_file_path = std::filesystem::path(argv[5]);
	if (!std::filesystem::exists(teams_file_path)) {
		throw std::runtime_error(containers::concatenate<std::string>(teams_file_path.string(), " does not exist"sv));
	}
	std::filesystem::create_directories(output_path);
	return ParsedArgs{
		mode,
		generation,
		thread_count,
		std::move(teams_file_path),
		std::move(output_stats_path)
	};
}

constexpr auto do_pass(Mode const mode, Glicko1 const & ratings_estimate, BattleResult const & result, auto function) -> void {
	auto get_p = [&](BattleResult::Side const & side) {
		auto const rating = side.rating ? *side.rating : ratings_estimate.get(side.id);
		return chance_to_win(rating, initial_rating);
	};
	switch (mode) {
		case Mode::unweighted:
		case Mode::weighted:
			function(result.side1.team, weight(mode, get_p(result.side1)));
			function(result.side2.team, weight(mode, get_p(result.side2)));
			break;
		case Mode::weighted_winner:
			switch (result.winner) {
				case BattleResult::Winner::side1:
					function(result.side1.team, weight(mode, get_p(result.side2)));
					break;
				case BattleResult::Winner::side2:
					function(result.side2.team, weight(mode, get_p(result.side1)));
					break;
				case BattleResult::Winner::tie:
					function(result.side1.team, weight(mode, get_p(result.side2)) / 2.0);
					function(result.side2.team, weight(mode, get_p(result.side1)) / 2.0);
					break;
			}
			break;
		case Mode::top_players: {
			auto function_if = [&](BattleResult::Side const & winner, BattleResult::Side const & loser, double const scalar) {
				auto const p = get_p(loser);
				if (p > 0.75) {
					function(winner.team, weight(mode, p) * scalar);
				}
			};
			switch (result.winner) {
				case BattleResult::Winner::side1:
					function_if(result.side1, result.side2, 1.0);
					break;
				case BattleResult::Winner::side2:
					function_if(result.side2, result.side1, 1.0);
					break;
				case BattleResult::Winner::tie:
					function_if(result.side1, result.side2, 0.5);
					function_if(result.side2, result.side1, 0.5);
					break;
			}
			break;
		}
	}
}

auto populate_ratings_estimate(std::filesystem::path const & team_path) -> Glicko1 {
	auto ratings_estimate = Glicko1();
	for (auto const & battle_result : battle_result_reader(team_path)) {
		ratings_estimate.add_result(battle_result.side1.id, battle_result.side2.id, battle_result.winner);
	}
	ratings_estimate.finalize();
	return ratings_estimate;
}

auto make_correlations(Mode const mode, ThreadCount const thread_count, std::filesystem::path const & teams_file_path, Glicko1 const & ratings_estimate, UsageStats const & usage_stats) {
	auto correlations = Correlations(usage_stats);
	parallel_for_each(
		thread_count,
		battle_result_reader(teams_file_path),
		[&](BattleResult const & result) {
			do_pass(
				mode,
				ratings_estimate,
				result,
				[&](auto const & team, double const weight) {
					if (weight != 0.0) {
						correlations.add(team, weight);
					}
				}
			);
		}
	);
	return correlations;
}

} // namespace
} // namespace technicalmachine::ps_usage_stats

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	using namespace technicalmachine::ps_usage_stats;

	auto const args = parse_args(argc, argv);

	auto const ratings_estimate = populate_ratings_estimate(args.teams_file_path);

	auto usage_stats = std::make_unique<UsageStats>();
	for (auto const & result : battle_result_reader(args.teams_file_path)) {
		do_pass(
			args.mode,
			ratings_estimate,
			result,
			[&](auto const & team, double const weight) { if (weight != 0.0) { usage_stats->add(team, weight); } }
		);
	}

	auto const correlations = make_correlations(args.mode, args.thread_count, args.teams_file_path, ratings_estimate, *usage_stats);

	auto out_file = open_binary_file_for_writing(args.output_stats_path);
	serialize(out_file, args.generation, *usage_stats, correlations);

	return 0;
}
