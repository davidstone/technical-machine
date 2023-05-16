// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <filesystem>

import tm.ps_usage_stats.battle_result_writer;
import tm.ps_usage_stats.parse_log;
import tm.ps_usage_stats.worker;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {
namespace {

using namespace std::string_view_literals;

using ThreadCount = bounded::integer<1, 10'000>;

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

auto files_in_directory(std::filesystem::path const & path) {
	auto not_directory = [](std::filesystem::path const & p) { return !std::filesystem::is_directory(p); };
	return containers::filter(
		containers::range_view(
			std::filesystem::recursive_directory_iterator(path),
			std::filesystem::recursive_directory_iterator()
		),
		not_directory
	);
}

auto turn_logs_into_team_file(std::filesystem::path const & output_file, ThreadCount const thread_count, std::filesystem::path const & input_directory) -> void {
	auto battle_result_writer = BattleResultWriter(output_file);
	auto writer_mutex = std::mutex();
	auto workers = containers::dynamic_array(containers::generate_n(thread_count, [&] {
		return make_worker<std::filesystem::path>([&](std::filesystem::path const & input_file) {
			try {
				auto const battle_result = parse_log(input_file);
				if (!battle_result) {
					return;
				}
				auto lock = std::scoped_lock(writer_mutex);
				battle_result_writer(*battle_result);
			} catch (std::exception const & ex) {
				throw std::runtime_error(containers::concatenate<std::string>("Error parsing "sv, input_file.string(), ": "sv, std::string_view(ex.what())));
			}
		});
	}));
	auto inputs = files_in_directory(input_directory);
	auto it = containers::begin(inputs);
	auto const last = containers::end(inputs);
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

} // namespace
} // namespace technicalmachine::ps_usage_stats

auto main(int argc, char ** argv) -> int {
	using namespace technicalmachine;
	auto const args = ps_usage_stats::parse_args(argc, argv);
	ps_usage_stats::turn_logs_into_team_file(args.output_file, args.thread_count, args.input_directory);
	return 0;
}
