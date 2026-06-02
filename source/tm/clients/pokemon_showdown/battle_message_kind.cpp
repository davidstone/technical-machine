// Copyright David Stone 2025.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_message_kind;

import tm.clients.ps.in_message;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

constexpr auto ladder_timeout = "Ladder isn't responding, score probably updated but might not have (Request timeout)"_s;

export enum class BattleMessageKind {
	junk,
	init,
	regular,
	request,
	error
};

// https://github.com/smogon/pokemon-showdown/blob/master/sim/SIM-PROTOCOL.md
export constexpr auto get_battle_message_kind(InMessage const first_message, bool const has_more_data) -> BattleMessageKind {
	auto matches = [&](auto const... strs) {
		return (... or (first_message.type() == strs));
	};
	using enum BattleMessageKind;
	if (matches("init"_s, "raw"_s, "t:"_s, "inactive"_s, "inactiveoff"_s)) {
		return junk;
	} else if (matches("teamsize"_s)) {
		// "teamsize" never starts a block in the real stream. However, we have
		// to filter out all the "player" messages when parsing PS logs due to
		// bugs on the PS side. That makes "teamsize" the beginning of the block
		// when parsing those files.
		return init;
	} else if (matches("player"_s)) {
		return has_more_data ? init	: junk;
	} else if (matches("-message"_s)) {
		return regular;
	} else if (matches(""_s)) {
		return first_message.remainder() == ladder_timeout ? junk : regular;
	} else if (matches("request"_s)) {
		if (has_more_data) {
			throw std::runtime_error("Request message contains too much data");
		}
		return request;
	} else if (matches("error"_s)) {
		if (has_more_data) {
			throw std::runtime_error("Error message contains too much data");
		}
		return error;
	} else {
		throw std::runtime_error(containers::concatenate<std::string>(
			"Unknown battle message kind: |"_s,
			first_message.type(),
			"|"_s,
			first_message.remainder()
		));
	}
}

} // namespace technicalmachine::ps
