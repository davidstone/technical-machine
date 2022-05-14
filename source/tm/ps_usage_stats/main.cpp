// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/battle_result_reader.hpp>
#include <tm/ps_usage_stats/glicko1.hpp>
#include <tm/ps_usage_stats/rating.hpp>
#include <tm/ps_usage_stats/serialize.hpp>
#include <tm/ps_usage_stats/usage_stats.hpp>
#include <tm/ps_usage_stats/worker.hpp>

#include <tm/string_conversions/generation.hpp>

#include <boost/thread/scoped_thread.hpp>

#include <bounded/to_integer.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/dynamic_array.hpp>

#include <filesystem>
#include <ios>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace technicalmachine::ps_usage_stats {
namespace {

using namespace std::string_view_literals;

enum class Mode {
	unweighted,
	simple_weighted,
	inverse_weighted,
	simple_weighted_winner,
	inverse_weighted_winner
};

using ThreadCount = bounded::integer<1, 10'000>;

struct ParsedArgs {
	Mode mode;
	Generation generation;
	ThreadCount thread_count;
	std::filesystem::path teams_file_path;
	std::filesystem::path output_stats_path;
};

constexpr auto parse_mode(std::string_view const str) -> Mode {
	return
		str == "unweighted" ? Mode::unweighted :
		str == "simple_weighted" ? Mode::simple_weighted :
		str == "inverse_weighted" ? Mode::inverse_weighted :
		str == "simple_weighted_winner" ? Mode::simple_weighted_winner :
		str == "inverse_weighted_winner" ? Mode::inverse_weighted_winner :
		throw std::runtime_error("Invalid mode");
}

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	if (argc != 6) {
		throw std::runtime_error(
			"Usage is ps_usage_stats output_path mode generation thread_count teams_file_path\n"
			"mode must be one of: unweighted, simple_weighted, inverse_weighted, simple_weighted_winner, inverse_weighted_winner\n"
		);
	}
	auto const output_path = std::filesystem::path(argv[1]);
	auto output_stats_path = output_path / "stats.json";
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

constexpr auto weight(Mode const mode, Rating const rating) -> double {
	switch (mode) {
		case Mode::unweighted:
			return 1.0;
		case Mode::simple_weighted:
		case Mode::simple_weighted_winner:
			return chance_to_win(rating, initial_rating);
		case Mode::inverse_weighted:
		case Mode::inverse_weighted_winner:
			return 1.0 / (1.0 - chance_to_win(rating, initial_rating));
	}
}

constexpr auto do_pass(Mode const mode, Glicko1 const & ratings_estimate, BattleResult const & result, auto function) -> void {
	auto get_rating = [&](BattleResult::Side const & side) {
		if (side.rating) {
			throw std::runtime_error("Shouldn't have rating");
		}
		return side.rating ? *side.rating : ratings_estimate.get(side.id);
	};
	switch (mode) {
		case Mode::unweighted:
		case Mode::simple_weighted:
		case Mode::inverse_weighted:
			function(result.side1.team, weight(mode, get_rating(result.side1)));
			function(result.side2.team, weight(mode, get_rating(result.side2)));
			break;
		case Mode::simple_weighted_winner:
		case Mode::inverse_weighted_winner:
			switch (result.winner) {
				case BattleResult::Winner::side1:
					function(result.side1.team, weight(mode, get_rating(result.side2)));
					break;
				case BattleResult::Winner::side2:
					function(result.side2.team, weight(mode, get_rating(result.side1)));
					break;
				case BattleResult::Winner::tie:
					function(result.side1.team, weight(mode, get_rating(result.side2)) / 2.0);
					function(result.side2.team, weight(mode, get_rating(result.side1)) / 2.0);
					break;
			}
			break;
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
	{
		auto workers = containers::dynamic_array(containers::generate_n(thread_count, [&] {
			return make_worker<BattleResult>([&](BattleResult const & result) {
				do_pass(
					mode,
					ratings_estimate,
					result,
					[&](auto const & team, double const weight) { if (weight != 0.0) { correlations.add(team, weight); } }
				);
			});
		}));
		auto reader = battle_result_reader(teams_file_path);
		auto it = containers::begin(reader);
		auto const last = containers::end(reader);
		while (true) {
			for (auto & worker : workers) {
				if (it == last) {
					break;
				}
				worker.add_work(*it);
				++it;
			}
			if (it == last) {
				break;
			}
		}
	}
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

	auto out_file = open_file(args.output_stats_path, std::ios_base::out | std::ios_base::binary);
	serialize(out_file, args.generation, *usage_stats, correlations);

	return 0;
}
