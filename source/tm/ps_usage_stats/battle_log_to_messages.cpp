// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

#include <bounded/assert.hpp>

#include <operators/forward.hpp>

export module tm.ps_usage_stats.battle_log_to_messages;

import tm.clients.ps.battle_message;
import tm.clients.ps.in_message;
import tm.clients.ps.is_chat_message;
import tm.clients.ps.make_battle_message;

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
	if (matches(rhs, "player"sv, "inactive"sv, "inactiveoff"sv)) {
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

constexpr auto is_not_chat_or_error_message = [](InMessage const message) {
	return !is_chat_message(message) and message.type() != "error";
};

export auto battle_log_to_messages(nlohmann::json const & log) -> containers::vector<BattleMessage> {
	return containers::vector<BattleMessage>(
		containers::remove_none(containers::transform(
			containers::chunk_by(
				containers::filter(
					containers::transform(log, [](nlohmann::json const & message) {
						return InMessage(message.get<std::string_view>());
					}),
					is_not_chat_or_error_message
				),
				is_part_of_previous_chunk
			),
			[](auto const messages) {
				return make_battle_message(messages);
			}
		))
	);
}

} // namespace technicalmachine::ps_usage_stats
