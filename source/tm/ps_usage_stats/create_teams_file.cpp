// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/battle_result_writer.hpp>
#include <tm/ps_usage_stats/parse_log.hpp>

#include <tm/files_in_path.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/append.hpp>
#include <containers/range_view.hpp>
#include <containers/vector.hpp>

#include <filesystem>
#include <span>
#include <string>
#include <string_view>
#include <stdexcept>
#include <utility>

namespace technicalmachine::ps_usage_stats {
namespace {

using namespace std::string_view_literals;

struct ParsedArgs {
	std::filesystem::path output_file;
	containers::vector<std::filesystem::path> input_files;
};

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	constexpr auto leading_arguments = 2;
	if (argc < leading_arguments) {
		throw std::runtime_error(
			"Usage is create_teams_file output_file battle_logs...\n"
			"If \"battle_logs...\" includes a directory, all files in that directory will be parsed."
		);
	}
	auto const output_file = std::filesystem::path(argv[1]);
	if (std::filesystem::exists(output_file)) {
		throw std::runtime_error(containers::concatenate<std::string>(output_file.string(), " already exists"sv));
	}
	auto input_files = containers::vector<std::filesystem::path>();
	for (char const * const path : containers::range_view(argv + leading_arguments, argv + argc)) {
		containers::append(input_files, files_in_path(std::filesystem::path(path)));
	}
	return ParsedArgs{
		std::move(output_file),
		std::move(input_files)
	};
}

auto turn_logs_into_team_file(std::filesystem::path const & output_file, std::span<std::filesystem::path const> const input_files) -> void {
	std::filesystem::create_directories(output_file.parent_path());
	auto battle_result_writer = BattleResultWriter(output_file);
	for (auto const & input_file : input_files) {
		try {
			battle_result_writer(parse_log(input_file));
		} catch (std::exception const & ex) {
			throw std::runtime_error(containers::concatenate<std::string>("Error parsing "sv, input_file.string(), ": "sv, std::string_view(ex.what())));
		}
	}
}

} // namespace
} // namespace technicalmachine::ps_usage_stats

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	auto const args = ps_usage_stats::parse_args(argc, argv);
	ps_usage_stats::turn_logs_into_team_file(args.output_file, args.input_files);
	return 0;
}
