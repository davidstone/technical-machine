// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.battle_message;

import tm.clients.ps.battle_init_message;
import tm.clients.ps.event_block;
import tm.clients.ps.parsed_team;

import std_module;
import tv;

namespace technicalmachine::ps {

export struct CreateBattle {
	friend auto operator==(CreateBattle, CreateBattle) -> bool = default;
};

export struct ErrorMessage {
	constexpr explicit ErrorMessage(std::string_view const message_):
		m_message(message_)
	{
	}
	constexpr auto message() const -> std::string_view {
		return m_message;
	}
	friend auto operator==(ErrorMessage, ErrorMessage) -> bool = default;
private:
	std::string_view m_message;
};

export using BattleMessage = tv::variant<
	CreateBattle,
	ParsedSide,
	BattleInitMessage,
	EventBlock,
	ErrorMessage
>;

} // namespace technicalmachine::ps
