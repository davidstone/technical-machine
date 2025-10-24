// Copyright David Stone 2025.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.for_each_log;

import tm.clients.party;

import tm.ps_usage_stats.battle_log_to_messages;
import tm.ps_usage_stats.files_in_directory;
import tm.ps_usage_stats.parallel_for_each;
import tm.ps_usage_stats.parse_input_log;
import tm.ps_usage_stats.parse_log;
import tm.ps_usage_stats.rated_side;
import tm.ps_usage_stats.thread_count;

import tm.load_json_from_file;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps_usage_stats {
using namespace bounded::literal;

export auto for_each_log(
	ThreadCount const thread_count,
	std::filesystem::path const & input_directory,
	auto process_log
) {
	using Accumulate = decltype(process_log(
		bounded::declval<std::filesystem::path const &>(),
		bounded::declval<RatedSide const &>(),
		bounded::declval<BattleLogMessages const &>()
	));
	auto accumulator = containers::dynamic_array(containers::repeat_default_n<Accumulate>(thread_count));
	parallel_for_each(
		thread_count,
		files_in_directory(input_directory),
		[&](std::filesystem::path const & input_file, ThreadIndex const index) {
			auto const json = load_json_from_file(input_file);
			auto const battle_result = parse_log(json);
			if (!battle_result) {
				return;
			}
			auto const battle_messages = battle_log_to_messages(json.at("log"));
			auto const input_log = parse_input_log(json.at("inputLog"));
			auto const side1 = RatedSide(Party(0_bi), battle_result->side1, input_log.side1);
			auto const side2 = RatedSide(Party(1_bi), battle_result->side2, input_log.side2);
			accumulator[index] +=
				process_log(input_file, side1, battle_messages) +
				process_log(input_file, side2, battle_messages);
		}
	);
	return containers::sum(accumulator);
}

} // namespace technicalmachine::ps_usage_stats
