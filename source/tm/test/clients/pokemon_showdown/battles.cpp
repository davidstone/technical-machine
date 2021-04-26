// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/clients/pokemon_showdown/battles.hpp>

#include <tm/clients/pokemon_showdown/battles.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <bounded/integer.hpp>

#include <bounded/scope_guard.hpp>
#include <containers/string.hpp>

#include <iostream>

namespace technicalmachine {
namespace ps {
namespace {

auto load_lines_from_file(std::filesystem::path const & file_name) {
	auto file = std::ifstream(file_name);
	return containers::string(containers::range_view(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
}

auto parse_room(std::string_view const line, std::filesystem::path const & path) {
	if (line.empty() or line.front() != '>') {
		throw std::runtime_error("File does not start with >: " + path.string());
	}
	return line.substr(1);
}

void regression_tests() {
	auto const evaluate = AllEvaluate{};
	// Too large to fit on the stack
	auto const all_usage_stats = std::make_unique<AllUsageStats>();
	constexpr auto depth = DepthValues{1U, 0U};

	auto const battle_output_directory = std::filesystem::path("test/temp-battles");
	auto const remove_temporary_files = [&]{ std::filesystem::remove_all(battle_output_directory); };
	remove_temporary_files();
	constexpr auto log_foe_teams = false;
	{
		auto battles = Battles(battle_output_directory, log_foe_teams);

		auto paths_in_directory = [](std::filesystem::path const & path) {
			return containers::range_view(
				std::filesystem::directory_iterator(path),
				std::filesystem::directory_iterator()
			);
		};

		for (auto const & generation : paths_in_directory("test/battles")) {
			for (auto const & path : paths_in_directory(generation)) {
				auto const data = load_lines_from_file(path.path() / "server_messages.txt");
				auto messages = DelimitedBufferView(std::string_view(data), '\n');
				auto const room = parse_room(messages.pop(), path);
				battles.add_pending(
					containers::string(room),
					"Technical Machine",
					evaluate,
					depth,
					std::mt19937(std::random_device{}())
				);

				auto print_file_on_exception = bounded::scope_guard([&] { std::cerr << "Error in " << path.path() << '\n'; });
				while (!messages.remainder().empty()) {
					auto const next = messages.pop();
					auto print_message_on_exception = bounded::scope_guard([=] { std::cerr << next << '\n'; });
					battles.handle_message(
						*all_usage_stats,
						InMessage(room, next),
						[](std::string_view) {},
						[] {}
					);
					print_message_on_exception.dismiss();
				}
				print_file_on_exception.dismiss();
			}
		}
	}
	remove_temporary_files();
}

} // namespace

void test_battles() {
	std::cout << "Testing ps::Battles\n";
	try {
		regression_tests();
	} catch (std::exception const & ex [[maybe_unused]]) {
		throw;
	}
}

} // namespace ps
} // namespace technicalmachine
