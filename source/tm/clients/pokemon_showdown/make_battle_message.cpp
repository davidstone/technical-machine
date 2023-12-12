// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.make_battle_message;

import tm.clients.ps.battle_message;
import tm.clients.ps.event_block;
import tm.clients.ps.in_message;
import tm.clients.ps.message_block;
import tm.clients.ps.team_message;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {

using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto ladder_timeout = "Ladder isn't responding, score probably updated but might not have (Request timeout)"sv;

export constexpr auto make_battle_message(MessageBlock const messages) -> tv::optional<BattleMessage> {
	std::same_as<InMessage> auto const first_message = containers::front(messages);
	auto matches = [&](auto const... strs) {
		return (... or (first_message.type() == strs));
	};
	if (matches("init"sv)) {
		if (first_message.remainder() != "battle") {
			throw std::runtime_error(containers::concatenate<std::string>(
				"Invalid init message: "sv,
				first_message.remainder()
			));
		}
		return CreateBattle();
	} else if (matches("-message"sv)) {
		return make_event_block(messages);
	} else if (matches("player"sv)) {
		if (containers::next(containers::begin(messages)) == containers::end(messages)) {
			return tv::none;
		} else {
			return BattleInitMessage(messages);
		}
	} else if (matches("request"sv)) {
		if (containers::linear_size(messages) != 1_bi) {
			throw std::runtime_error("Request message contains too much data");
		}
		auto const json_str = first_message.remainder();
		if (json_str.empty()) {
			return tv::none;
		}
		return TeamMessage(json_str);
	} else if (matches(""sv)) {
		if (first_message.remainder() == ladder_timeout) {
			return tv::none;
		} else {
			return make_event_block(messages);
		}
	} else if (matches("error"sv)) {
		if (containers::linear_size(messages) != 1_bi) {
			throw std::runtime_error("Error message contains too much data");
		}
		return ErrorMessage(first_message.remainder());
	} else if (matches("t:"sv, "inactive"sv, "inactiveoff"sv)) {
		return tv::none;
	} else {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unknown battle message: |"sv,
			first_message.type(),
			"|"sv,
			first_message.remainder()
		));
	}
}

} // namespace technicalmachine::ps
