// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

export module tm.test.clients.ps.battles;

import tm.clients.ps.action_required;
import tm.clients.ps.battles;
import tm.clients.ps.make_battle_message;
import tm.clients.ps.message_block;
import tm.clients.ps.room_message_block;

import tm.clients.determine_selection;

import tm.evaluate.depth;

import tm.strategy.expectimax;
import tm.strategy.statistical;
import tm.strategy.strategy;

import tm.team_predictor.all_usage_stats;

import tm.generation;
import tm.get_directory;
import tm.open_file;
import tm.split_view;
import tm.visible_state;

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
	return containers::string(containers::subrange(
		std::istreambuf_iterator<char>(file),
		std::default_sentinel
	));
}

constexpr auto depth = Depth(1_bi, 0_bi);

struct Evaluator {
	template<typename T>
	auto operator()(T const & value, std::ostream & logger) {
		if constexpr (bounded::convertible_to<T, ps::ActionRequired>) {
			determine_selection(
				value.state,
				logger,
				m_all_usage_stats,
				m_strategy,
				m_random_engine
			);
		}
	}
private:
	Strategy m_strategy = make_expectimax(
		depth,
		make_statistical()
	);
	AllUsageStats m_all_usage_stats;
	std::mt19937 m_random_engine;
};

TEST_CASE("Pokemon Showdown regression", "[Pokemon Showdown]") {
	auto evaluator = Evaluator();
	auto const battle_output_directory = get_test_directory() / "temp-battles";
	auto const remove_temporary_files = [&] {
		std::filesystem::remove_all(battle_output_directory);
	};
	remove_temporary_files();
	{
		auto battles = ps::Battles();

		auto paths_in_directory = [](std::filesystem::path const & path) {
			return containers::transform(
				containers::subrange(
					std::filesystem::directory_iterator(path),
					std::default_sentinel
				),
				[](std::filesystem::directory_entry const & entry) -> std::filesystem::path const & {
					return entry.path();
				}
			);
		};

		for (auto const & generation : paths_in_directory(get_test_directory() / "battles")) {
			for (auto const & path : paths_in_directory(generation)) {
				INFO(path);
				auto analysis_logger = open_text_file(battle_output_directory / path.filename() / "analysis.txt");
				auto const data = load_lines_from_file(path / "server_messages.txt");
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
					auto const result = battles.handle_message(
						block.room(),
						*battle_message
					);
					tv::visit(result, [&](auto const & value) {
						evaluator(value, analysis_logger);
					});
				}
			}
		}
	}
	remove_temporary_files();
}

} // namespace
} // namespace technicalmachine
