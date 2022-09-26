// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/battles.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <tm/get_directory.hpp>
#include <tm/open_file.hpp>

#include <bounded/integer.hpp>

#include <containers/string.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

auto load_lines_from_file(std::filesystem::path const & file_name) {
	auto file = open_text_file_for_reading(file_name);
	return containers::string(containers::range_view(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
}

auto parse_room(std::string_view const line, std::filesystem::path const & path) {
	if (line.empty() or line.front() != '>') {
		FAIL("File does not start with >: " + path.string());
	}
	return line.substr(1);
}

TEST_CASE("Pokemon Showdown regression", "[Pokemon Showdown]") {
	auto const evaluate = AllEvaluate{};
	// Too large to fit on the stack
	auto const all_usage_stats = std::make_unique<AllUsageStats>();
	constexpr auto depth = Depth(1_bi, 0_bi);

	auto const battle_output_directory = get_test_directory() / "temp-battles";
	auto const remove_temporary_files = [&] {
		std::filesystem::remove_all(battle_output_directory);
	};
	remove_temporary_files();
	constexpr auto log_foe_teams = false;
	{
		auto battles = ps::Battles(battle_output_directory, log_foe_teams);

		auto paths_in_directory = [](std::filesystem::path const & path) {
			return containers::range_view(
				std::filesystem::directory_iterator(path),
				std::filesystem::directory_iterator()
			);
		};

		auto random_device = std::random_device();
		for (auto const & generation : paths_in_directory(get_test_directory() / "battles")) {
			for (auto const & path : paths_in_directory(generation)) {
				auto const data = load_lines_from_file(path.path() / "server_messages.txt");
				auto messages = DelimitedBufferView(std::string_view(data), '\n');
				auto const room = parse_room(messages.pop(), path);
				battles.add_pending(
					containers::string(room),
					"Technical Machine",
					evaluate,
					depth,
					std::mt19937(random_device())
				);

				INFO(path.path());
				while (!messages.remainder().empty()) {
					auto const next = messages.pop();
					INFO(next);
					battles.handle_message(
						*all_usage_stats,
						ps::InMessage(room, next),
						[](std::string_view) {},
						[] {}
					);
				}
			}
		}
	}
	remove_temporary_files();
}

} // namespace
} // namespace technicalmachine
