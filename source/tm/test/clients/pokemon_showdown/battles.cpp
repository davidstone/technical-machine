// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <filesystem>
#include <catch2/catch_test_macros.hpp>

export module tm.test.clients.ps.battles;

import tm.clients.ps.battles;
import tm.clients.ps.make_battle_message;
import tm.clients.ps.message_block;
import tm.clients.ps.room_message_block;

import tm.evaluate.all_evaluate;
import tm.evaluate.depth;

import tm.team_predictor.all_usage_stats;

import tm.get_directory;
import tm.open_file;
import tm.split_view;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;
using namespace std::string_view_literals;

auto load_lines_from_file(std::filesystem::path const & file_name) {
	auto file = open_text_file_for_reading(file_name);
	return containers::string(containers::range_view(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()));
}

TEST_CASE("Pokemon Showdown regression", "[Pokemon Showdown]") {
	auto const evaluate = AllEvaluate();
	auto const all_usage_stats = AllUsageStats(StatsForGeneration(stats_for_generation));
	constexpr auto depth = Depth(1_bi, 0_bi);

	auto const battle_output_directory = get_test_directory() / "temp-battles";
	auto const remove_temporary_files = [&] {
		std::filesystem::remove_all(battle_output_directory);
	};
	remove_temporary_files();
	{
		auto battles = ps::Battles(battle_output_directory);

		auto paths_in_directory = [](std::filesystem::path const & path) {
			return containers::range_view(
				std::filesystem::directory_iterator(path),
				std::filesystem::directory_iterator()
			);
		};

		constexpr auto username = "Technical Machine"sv;

		for (auto const & generation : paths_in_directory(get_test_directory() / "battles")) {
			for (auto const & path : paths_in_directory(generation)) {
				INFO(path.path());
				auto const data = load_lines_from_file(path.path() / "server_messages.txt");
				auto const messages = containers::split_range(std::string_view(data).substr(1), "\n>");
				for (std::string_view const message : messages) {
					INFO(message);
					auto const block = ps::RoomMessageBlock(
						message,
						ps::RoomMessageBlock::FirstLineIsRoom()
					);
					auto const battle_message = make_battle_message(ps::message_block(block.str()));
					if (!battle_message) {
						continue;
					}
					battles.handle_message(
						block.room(),
						*battle_message,
						username,
						evaluate,
						all_usage_stats,
						depth
					);
				}
			}
		}
	}
	remove_temporary_files();
}

} // namespace
} // namespace technicalmachine
