// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

export module tm.test.clients.ps.client_message_handler;

import tm.clients.ps.client_message_handler;
import tm.clients.ps.room_message_block;

import tm.evaluate.depth;

import tm.strategy.expectimax;
import tm.strategy.statistical;

import tm.boost_networking;
import tm.get_directory;
import tm.open_file;
import tm.settings_file;
import tm.split_view;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;
using namespace std::string_view_literals;

auto load_lines_from_file(std::filesystem::path const & file_name) {
	auto file = open_text_file_for_reading(file_name);
	return containers::string(containers::subrange(
		std::istreambuf_iterator<char>(file),
		std::default_sentinel
	));
}

constexpr auto depth = Depth(1_bi, 0_bi);

constexpr auto send_message = [](std::string_view) -> void {};
constexpr auto authenticate = [](
	std::string_view,
	std::string_view,
	http::request<http::string_body> const &
) -> http::response<http::string_body> {
	return {};
};

auto paths_in_directory(std::filesystem::path const & path) {
	return containers::transform(
		containers::subrange(
			std::filesystem::directory_iterator(path),
			std::default_sentinel
		),
		[](std::filesystem::directory_entry const & entry) -> std::filesystem::path const & {
			return entry.path();
		}
	);
}

TEST_CASE("Pokemon Showdown regression", "[Pokemon Showdown]") {
	auto const test_directory = get_test_directory();
	auto const battle_output_directory = test_directory / "temp-battles";
	auto const remove_temporary_files = [&] {
		std::filesystem::remove_all(battle_output_directory);
	};
	remove_temporary_files();
	{
		auto client = ps::ClientMessageHandler(
			SettingsFile{
				.host = "",
				.port = "",
				.resource = "",
				.username = "",
				.password = "",
				.team = SettingsFile::NoTeam(),
				.style = SettingsFile::Accept()
			},
			battle_output_directory,
			make_expectimax(
				depth,
				make_statistical()
			),
			send_message,
			authenticate
		);

		for (auto const & generation : paths_in_directory(test_directory / "battles")) {
			for (auto const & path : paths_in_directory(generation)) {
				INFO(path);
				auto const data = load_lines_from_file(path / "server_messages.txt");
				auto const messages = containers::split_range(std::string_view(data).substr(1), "\n>");
				for (std::string_view const message : messages) {
					INFO(message);
					client.handle_messages(ps::RoomMessageBlock(
						message,
						ps::RoomMessageBlock::FirstLineIsRoom()
					));
				}
			}
		}
	}
	remove_temporary_files();
}

} // namespace
} // namespace technicalmachine
