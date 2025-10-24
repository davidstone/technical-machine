// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_manager;

import tm.clients.ps.action_required;
import tm.clients.ps.battle_init_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.battle_started;
import tm.clients.ps.parsed_message;
import tm.clients.ps.parsed_request;
import tm.clients.ps.slot_memory;
import tm.clients.ps.start_of_turn;

import tm.clients.battle_continues;
import tm.clients.battle_finished;
import tm.clients.party;
import tm.clients.turn_count;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

export struct BattleManager {
	explicit constexpr BattleManager(BattleInitMessage const & message):
		m_battle(message)
	{
	}

	constexpr auto handle_request(ParsedRequest const & message) -> ActionRequired {
		auto & handler = tv::visit(m_battle, tv::overload(
			[&](BattleInitMessage const init) -> BattleMessageHandler & {
				return m_battle.emplace([&] -> BattleMessageHandler {
					return BattleMessageHandler(
						message.party,
						message.team,
						init
					);
				});
			},
			[](BattleMessageHandler & h) -> BattleMessageHandler & {
				return h;
			}
		));
		return ActionRequired(
			handler.state(),
			handler.slot_memory()
		);
	}

	using Result = tv::variant<
		BattleContinues,
		StartOfTurn,
		BattleFinished
	>;
	constexpr auto handle_message(std::span<ParsedMessage const> const message) -> Result {
		return tv::visit(m_battle, tv::overload(
			[](BattleInitMessage const &) -> Result {
				throw std::runtime_error("Received an event before the first action");
			},
			[&](BattleMessageHandler & handler) -> Result {
				auto result = handler.handle_message(message);
				return tv::visit(
					std::move(result),
					bounded::construct<Result>
				);
			}
		));
	}
private:
	using State = tv::variant<
		BattleInitMessage,
		BattleMessageHandler
	>;
	State m_battle;
};

} // namespace technicalmachine::ps
