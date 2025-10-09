// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.make_battle_message;

import tm.clients.ps.battle_message;
import tm.clients.ps.event_block;
import tm.clients.ps.in_message;
import tm.clients.ps.make_battle_init_message;
import tm.clients.ps.parse_request;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps {

using namespace bounded::literal;
using namespace std::string_view_literals;

export constexpr auto make_battle_message(auto const messages) -> BattleMessage {
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
		return make_battle_init_message(messages);
	} else if (matches("teamsize"sv)) {
		// This never starts a block in the real stream. However, we have to
		// filter out all the "player" messages when parsing PS logs due to bugs
		// on the PS side. That makes "teamsize" the beginning of the block when
		// parsing those files.
		return make_battle_init_message(messages);
	} else if (matches(""sv)) {
		return make_event_block(messages);
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
