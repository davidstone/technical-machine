// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

import tm.ps_usage_stats.battle_result_writer;
import tm.ps_usage_stats.files_in_directory;
import tm.ps_usage_stats.parallel_for_each;
import tm.ps_usage_stats.parse_log;
import tm.ps_usage_stats.thread_count;

import tm.load_json_from_file;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {
namespace {

using namespace std::string_view_literals;

struct ParsedArgs {
	std::filesystem::path output_file;
	ThreadCount thread_count;
	std::filesystem::path input_directory;
};

auto parse_args(int argc, char const * const * argv) -> ParsedArgs {
	if (argc != 4) {
		throw std::runtime_error("Usage is create_teams_file output_file thread_count input_directory");
	}
	auto output_file = std::filesystem::path(argv[1]);
	if (std::filesystem::exists(output_file)) {
		throw std::runtime_error(containers::concatenate<std::string>(output_file.string(), " already exists"sv));
	}
	auto const thread_count = bounded::to_integer<ThreadCount>(argv[2]);
	auto input_directory = std::filesystem::path(argv[3]);
	if (!std::filesystem::exists(input_directory)) {
		throw std::runtime_error(containers::concatenate<std::string>(input_directory.string(), " does not exist"sv));
	}
	return ParsedArgs{
		std::move(output_file),
		thread_count,
		std::move(input_directory)
	};
}

auto turn_logs_into_team_file(std::filesystem::path const & output_file, ThreadCount const thread_count, std::filesystem::path const & input_directory) -> void {
	auto battle_result_writer = BattleResultWriter(output_file);
	auto writer_mutex = std::mutex();
	parallel_for_each(
		thread_count,
		files_in_directory(input_directory),
		[&](std::filesystem::path const & input_file) {
			try {
				auto const battle_result = parse_log(load_json_from_file(input_file));
				if (!battle_result) {
					return;
				}
				auto lock = std::scoped_lock(writer_mutex);
				battle_result_writer(*battle_result);
			} catch (std::exception const & ex) {
				throw std::runtime_error(containers::concatenate<std::string>("Error parsing "sv, input_file.string(), ": "sv, std::string_view(ex.what())));
			}
		}
	);
}

} // namespace
} // namespace technicalmachine::ps_usage_stats

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	auto const args = ps_usage_stats::parse_args(argc, argv);
	ps_usage_stats::turn_logs_into_team_file(args.output_file, args.thread_count, args.input_directory);
	return 0;
}
