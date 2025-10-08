// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_manager;

import tm.clients.ps.action_required;
import tm.clients.ps.battle_init_message;
import tm.clients.ps.battle_message;
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

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

struct BattleExists {
};

using BattleState = tv::variant<
	BattleExists,
	ParsedRequest,
	BattleMessageHandler
>;

// We create a state machine.
// BattleExists + ParsedRequest => ParsedRequest
// ParsedRequest + BattleInitMessage => BattleMessageHandler
// BattleMessageHandler + ParsedRequest => BattleMessageHandler
// BattleMessageHandler + EventBlock => BattleMessageHandler

export struct BattleManager {
	BattleManager() = default;

	using Result = tv::variant<
		ActionRequired,
		StartOfTurn,
		BattleFinished,
		BattleContinues,
		BattleStarted
	>;

	constexpr auto handle_message(ParsedRequest const & message) -> Result {
		tv::visit(m_battle, tv::overload(
			[&](BattleExists) {
				m_battle = message;
			},
			[](ParsedRequest const &) {
				throw std::runtime_error("Got two teams while handling messages");
			},
			[&](BattleMessageHandler & handler) {
				handler.save_request(message);
			}
		));
		return BattleContinues();
	}

	auto handle_message(BattleInitMessage const message) -> Result {
		return tv::visit(m_battle, tv::overload(
			[](BattleExists) -> BattleStarted {
				throw std::runtime_error("Received a BattleInitMessage before getting a team");
			},
			[&](ParsedRequest const & request) -> BattleStarted {
				auto & handler = m_battle.emplace([&] -> BattleMessageHandler {
					return BattleMessageHandler(
						auto(request),
						message
					);
				});
				return BattleStarted(ActionRequired(
					handler.state(),
					handler.slot_memory(),
					handler.exchange_request()
				));
			},
			[](BattleMessageHandler const &) -> BattleStarted {
				throw std::runtime_error("Tried to initialize an existing battle");
			}
		));
	}

	auto handle_message(std::span<ParsedMessage const> const message) -> Result {
		return tv::visit(m_battle, tv::overload(
			[](BattleExists) -> Result {
				throw std::runtime_error("Received an event before getting a team");
			},
			[](ParsedRequest const &) -> Result {
				throw std::runtime_error("Received an event before a BattleInitMessage");
			},
			[&](BattleMessageHandler & handler) -> Result {
				auto result = handler.handle_message(message);
				return tv::visit(std::move(result), [](auto value) -> Result {
					return value;
				});
			}
		));
	}
private:
	BattleState m_battle = BattleExists();
};

} // namespace technicalmachine::ps
