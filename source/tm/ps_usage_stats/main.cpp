// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

namespace technicalmachine::ps_usage_stats {
namespace {

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
	std::filesystem::path output;
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
			"Usage is ps_log_parser mode generation output_file battle_logs...\n"
			"mode must be one of: unweighted, simple_weighted, inverse_weighted, simple_weighted_winner, inverse_weighted_winner\n"
			"If \"battle_logs...\" includes a directory, all files in that directory will be parsed."
		);
	}
	auto const mode = parse_mode(argv[1]);
	auto const generation = from_string<Generation>(argv[2]);
	auto output_path = std::filesystem::path(argv[3]);
	if (std::filesystem::exists(output_path) and output_path != "/dev/stdout") {
		throw std::runtime_error("Output file already exists");
	}
	auto input_files = [=]() {
		auto result = containers::vector<std::filesystem::path>();
		for (char const * const path : containers::range_view(argv + leading_arguments, argv + argc)) {
			containers::append(result, files_in_path(std::filesystem::path(path)));
		}
		return result;
	}();
	return ParsedArgs{mode, generation, std::move(output_path), std::move(input_files)};
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
			function(result.winner.team, weight(mode, result.winner.rating));
			function(result.loser.team, weight(mode, result.loser.rating));
			break;
		case Mode::simple_weighted_winner:
		case Mode::inverse_weighted_winner:
			function(result.winner.team, weight(mode, result.loser.rating));
			break;
	}
}

} // namespace
} // namespace technicalmachine::ps_usage_stats

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	using namespace technicalmachine::ps_usage_stats;
	auto const args = parse_args(argc, argv);
	auto usage_stats = std::make_unique<UsageStats>();
	for (auto const & input : args.inputs) {
		try {
			do_pass(
				args.mode,
				parse_log(input),
				[&](auto const & team, double const weight) { usage_stats->add(team, weight); }
			);
		} catch (std::exception const & ex) {
			std::cerr << "Error parsing " << input << ": " << ex.what() << '\n';
			return 1;
		}
	}
	auto correlations = Correlations(*usage_stats);
	for (auto const & input : args.inputs) {
		try {
			do_pass(
				args.mode,
				parse_log(input),
				[&](auto const & team, double const weight) { correlations.add(team, weight); }
			);
		} catch (std::exception const & ex) {
			std::cerr << "Error in second pass over " << input << ": " << ex.what() << '\n';
			return 1;
		}
	}

	auto out_file = std::ofstream(args.output);
	out_file << serialize(args.generation, *usage_stats, correlations) << '\n';

	return 0;
}
