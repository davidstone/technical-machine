// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

export module tm.ps_usage_stats.battle_log_to_messages;

import tm.clients.ps.battle_init_message;
import tm.clients.ps.event_block;
import tm.clients.ps.in_message;
import tm.clients.ps.is_chat_message;
import tm.clients.ps.make_battle_init_message;

import tm.nlohmann_json;

import bounded;
import containers;
import std_module;
import tv;

namespace technicalmachine::ps_usage_stats {
using namespace std::string_view_literals;
using namespace ps;

constexpr auto is_part_of_previous_chunk = [](InMessage const lhs, InMessage const rhs) {
	auto matches = [&](InMessage const message, auto const... strs) {
		return (... or (message.type() == strs));
	};
	if (matches(lhs, "teamsize"sv)) {
		return true;
	}
	if (matches(rhs, "teamsize"sv, "inactive"sv, "inactiveoff"sv)) {
		return false;
	}
	if (matches(lhs, "turn"sv)) {
		return false;
	}
	if (matches(lhs, "faint"sv) and matches(rhs, ""sv)) {
		return false;
	}
	return true;
};

constexpr auto is_potentially_useful = [](InMessage const message) {
	return
		!is_chat_message(message) and
		message.type() != "error" and
		message.type() != "gametype" and
		message.type() != "player" and
		message.type() != "t:";
};

export struct BattleLogMessages {
	BattleInitMessage init;
	containers::dynamic_array<EventBlock> messages;
	friend auto operator==(BattleLogMessages const &, BattleLogMessages const &) -> bool = default;
};

export auto battle_log_to_messages(nlohmann::json const & log) -> BattleLogMessages {
	auto range = containers::chunk_by(
		containers::filter(
			containers::transform(log, [](nlohmann::json const & message) {
				return InMessage(message.get<std::string_view>());
			}),
			is_potentially_useful
		),
		is_part_of_previous_chunk
	);
	if (containers::is_empty(range)) {
		throw std::runtime_error("Battle log has no messages");
	}
	auto it = containers::begin(range);
	return BattleLogMessages(
		BattleInitMessage(make_battle_init_message(*it)),
		containers::dynamic_array<EventBlock>(
			containers::transform(
				containers::subrange(containers::next(it), containers::end(range)),
				make_event_block
			)
		)
	);
}

} // namespace technicalmachine::ps_usage_stats
