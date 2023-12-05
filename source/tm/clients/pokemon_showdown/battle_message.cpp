// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_message;

import tm.clients.ps.event_block;
import tm.clients.ps.message_block;
import tm.clients.ps.parse_team;

import tm.constant_generation;
import tm.generation;
import tm.generation_generic;
import tm.team;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {

export struct CreateBattle {
};

export struct TeamMessage {
	constexpr explicit TeamMessage(std::string_view const json_str):
		m_json_str(json_str)
	{
	}
	auto team(Generation const runtime_generation) const -> GenerationGeneric<KnownTeam> {
		auto make = [&]<Generation generation>(constant_gen_t<generation>) -> GenerationGeneric<KnownTeam> {
			return parse_team<generation>(m_json_str);
		};
		return constant_generation(runtime_generation, make);
	}
private:
	std::string_view m_json_str;
};

export struct BattleInitMessage {
	constexpr explicit BattleInitMessage(MessageBlock messages):
		m_messages(messages)
	{
	}
	constexpr auto messages() const -> MessageBlock {
		return m_messages;
	}
private:
	MessageBlock m_messages;
};

export struct ErrorMessage {
	constexpr explicit ErrorMessage(std::string_view const message_):
		m_message(message_)
	{
	}
	constexpr auto message() const -> std::string_view {
		return m_message;
	}
private:
	std::string_view m_message;
};

export using BattleMessage = tv::variant<
	CreateBattle,
	TeamMessage,
	BattleInitMessage,
	EventBlock,
	ErrorMessage
>;

} // namespace technicalmachine::ps
