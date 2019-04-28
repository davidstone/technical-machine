// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/test/clients/pokemon_showdown/battles.hpp>

#include <tm/files_in_directory.hpp>
#include <tm/clients/pokemon_showdown/battles.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <bounded/integer.hpp>

#include <containers/scope_guard.hpp>

#include <iostream>

namespace technicalmachine {
namespace ps {
namespace {

auto load_lines_from_file(std::filesystem::path const & file_name) {
	auto file = std::ifstream(file_name);
	return containers::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>{});
}

auto parse_room(std::string_view const line, std::filesystem::path const & path) {
	if (line.empty() or line.front() != '>') {
		throw std::runtime_error("File does not start with >: " + path.string());
	}
	return line.substr(1);
}

void regression_tests() {
	auto const usage_stats = UsageStats("settings/4/OU");
	auto const evaluate = Evaluate{};
	constexpr auto depth = 2;

	auto const battle_output_directory = std::filesystem::path("test/temp-battles");	
	auto const remove_temporary_files = [&]{ std::filesystem::remove_all(battle_output_directory); };
	remove_temporary_files();
	constexpr auto log_foe_teams = false;
	{
		auto battles = Battles(battle_output_directory, log_foe_teams);

		for (auto const & path : paths_in_directory("test/battles")) {
			auto const data = load_lines_from_file(path.path() / "server_messages.txt");
			auto messages = BufferView(data, '\n');
			auto const room = parse_room(messages.next(), path);
			battles.add_pending(
				std::string(room),
				"Technical Machine",
				usage_stats,
				evaluate,
				depth,
				std::mt19937(std::random_device{}()),
				Team(1_bi, true)
			);

			auto print_file_on_exception = containers::scope_guard([&] { std::cerr << "Error in " << path.path() << '\n'; });
			while (!messages.remainder().empty()) {
				auto const next = messages.next();
				auto print_message_on_exception = containers::scope_guard([=] { std::cerr << next << '\n'; });
				battles.handle_message(
					InMessage(room, next),
					[](std::string_view) {},
					[] {}
				);
				print_message_on_exception.dismiss();
			}
			print_file_on_exception.dismiss();
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
