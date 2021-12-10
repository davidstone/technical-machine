// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/battle_result_reader.hpp>
#include <tm/ps_usage_stats/battle_result_writer.hpp>
#include <tm/ps_usage_stats/parse_log.hpp>
#include <tm/ps_usage_stats/rating.hpp>
#include <tm/ps_usage_stats/serialize.hpp>
#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/string_conversions/generation.hpp>

#include <tm/files_in_path.hpp>

#include <bounded/to_integer.hpp>

#include <containers/append.hpp>
#include <containers/range_view.hpp>
#include <containers/vector.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <chrono>

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

struct ParsedArgs {
	Mode mode;
	Generation generation;
	std::filesystem::path intermediate_team_path;
	std::filesystem::path output_stats_path;
	containers::vector<std::filesystem::path> inputs;
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
	constexpr auto leading_arguments = 4;
	if (argc < leading_arguments) {
		throw std::runtime_error(
			"Usage is ps_log_parser mode generation output_path battle_logs...\n"
			"mode must be one of: unweighted, simple_weighted, inverse_weighted, simple_weighted_winner, inverse_weighted_winner\n"
			"If \"battle_logs...\" includes a directory, all files in that directory will be parsed. If \"battle_logs...\" is empty, will use pre-generated team file from \"output_path\" instead"
		);
	}
	auto const mode = parse_mode(argv[1]);
	auto const generation = from_string<Generation>(argv[2]);
	auto const output_path = std::filesystem::path(argv[3]);
	auto intermediate_team_path = output_path / "teams.tmmt";
	auto output_stats_path = output_path / "stats.json";
	if (std::filesystem::exists(output_stats_path)) {
		throw std::runtime_error(containers::concatenate<std::string>(output_stats_path.string(), " already exists"sv));
	}
	auto inputs = [=]() {
		auto result = containers::vector<std::filesystem::path>();
		for (char const * const path : containers::range_view(argv + leading_arguments, argv + argc)) {
			containers::append(result, files_in_path(std::filesystem::path(path)));
		}
		return result;
	}();
	if (std::filesystem::exists(intermediate_team_path) and !containers::is_empty(inputs)) {
		throw std::runtime_error(containers::concatenate<std::string>(intermediate_team_path.string(), " already exists"sv));
	}
	return ParsedArgs{
		mode,
		generation,
		std::move(intermediate_team_path),
		std::move(output_stats_path),
		std::move(inputs)
	};
}

constexpr auto weight(Mode const mode, bounded::optional<Rating> const rating) -> double {
	switch (mode) {
		case Mode::unweighted:
			return 1.0;
		case Mode::simple_weighted:
		case Mode::simple_weighted_winner:
			return rating ? chance_to_win(*rating) : 0.0;
		case Mode::inverse_weighted:
		case Mode::inverse_weighted_winner:
			return rating ? 1.0 / (1.0 - chance_to_win(*rating)) : 0.0;
	}
}

constexpr auto do_pass(Mode const mode, BattleResult const & result, auto function) -> void {
	switch (mode) {
		case Mode::unweighted:
		case Mode::simple_weighted:
		case Mode::inverse_weighted:
			function(result.side1.team, weight(mode, result.side1.rating));
			function(result.side1.team, weight(mode, result.side1.rating));
			break;
		case Mode::simple_weighted_winner:
		case Mode::inverse_weighted_winner:
			switch (result.winner) {
				case BattleResult::Winner::side1:
					function(result.side1.team, weight(mode, result.side2.rating));
					break;
				case BattleResult::Winner::side2:
					function(result.side2.team, weight(mode, result.side1.rating));
					break;
				case BattleResult::Winner::tie:
					function(result.side1.team, weight(mode, result.side2.rating) / 2.0);
					function(result.side2.team, weight(mode, result.side1.rating) / 2.0);
					break;
			}
			break;
	}
}

auto turn_logs_into_team_file(std::filesystem::path const & team_path, std::span<std::filesystem::path const> const inputs) -> void {
	if (containers::is_empty(inputs)) {
		return;
	}
	std::filesystem::create_directories(team_path.parent_path());
	auto battle_result_writer = BattleResultWriter(team_path);
	for (auto const & input : inputs) {
		try {
			battle_result_writer(parse_log(input));
		} catch (std::exception const & ex) {
			throw std::runtime_error(containers::concatenate<std::string>("Error parsing "sv, input.string(), ": "sv, std::string_view(ex.what())));
		}
	}
}

} // namespace
} // namespace technicalmachine::ps_usage_stats

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	using namespace technicalmachine::ps_usage_stats;

	using namespace std::chrono;
	auto print_time = [previous = system_clock::now()](std::string_view const str) mutable {
		auto const current = system_clock::now();
		std::cout << str << ": " << duration_cast<seconds>(current - previous).count() << "s\n";
		previous = current;
	};

	auto const args = parse_args(argc, argv);
	print_time("Parsed args");

	auto usage_stats = std::make_unique<UsageStats>();
	print_time("Allocated usage stats");

	turn_logs_into_team_file(args.intermediate_team_path, args.inputs);
	print_time("Turned logs into team file");

	for (auto const & result : battle_result_reader(args.intermediate_team_path)) {
		do_pass(
			args.mode,
			result,
			[&](auto const & team, double const weight) { usage_stats->add(team, weight); }
		);
	}
	print_time("Completed first pass");

	auto correlations = Correlations(*usage_stats);
	print_time("Constructed correlations");

	for (auto const & result : battle_result_reader(args.intermediate_team_path)) {
		do_pass(
			args.mode,
			result,
			[&](auto const & team, double const weight) { correlations.add(team, weight); }
		);
	}
	print_time("Completed second pass");

	auto out_file = std::ofstream(args.output_stats_path);
	out_file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	out_file << serialize(args.generation, *usage_stats, correlations) << '\n';

	print_time("Wrote output");

	return 0;
}
