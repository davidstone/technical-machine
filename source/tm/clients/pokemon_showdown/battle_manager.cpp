// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

export module tm.clients.ps.battle_manager;

import tm.clients.ps.action_required;
import tm.clients.ps.battle_init_message;
import tm.clients.ps.battle_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.battle_started;
import tm.clients.ps.event_block;
import tm.clients.ps.make_battle_message_handler;
import tm.clients.ps.parsed_team;
import tm.clients.ps.slot_memory;
import tm.clients.ps.start_of_turn;

import tm.clients.battle_continues;
import tm.clients.battle_finished;
import tm.clients.battle_response_error;
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
	ParsedSide,
	BattleMessageHandler
>;

// We essentially create a state machine.
// (nothing) + CreateBattle => BattleExists
// BattleExists + ParsedSide => ParsedSide
// ParsedSide + ParsedSide => ParsedSide (check they are the same)
// ParsedSide + BattleInitMessage => BattleMessageHandler
// BattleMessageHandler + ParsedSide => BattleMessageHandler (check they are the same)
// BattleMessageHandler + EventBlock => BattleMessageHandler or (nothing)

export struct BattleManager {
	BattleManager() = default;

	using Result = tv::variant<
		ActionRequired,
		StartOfTurn,
		BattleFinished,
		BattleContinues,
		BattleStarted,
		BattleResponseError
	>;

	constexpr auto handle_message(ParsedSide const message) -> Result {
		tv::visit(m_battle, tv::overload(
			[&](BattleExists) {
				m_battle = message;
			},
			[&](ParsedSide const &) {
				throw std::runtime_error("Got two teams while handling messages");
			},
			[&](BattleMessageHandler const &) {
				// We get this message too early to do anything with it. We
				// could save it for later, but instead just wait for the
				// resolution of
				// https://github.com/smogon/pokemon-showdown/issues/8546
			}
		));
		return BattleContinues();
	}

	auto handle_message(BattleInitMessage const message) -> Result {
		return tv::visit(m_battle, tv::overload(
			[](BattleExists) -> BattleStarted {
				throw std::runtime_error("Received a BattleInitMessage before getting a team");
			},
			[&](ParsedSide & side) -> BattleStarted {
				auto & handler = m_battle.emplace([&] -> BattleMessageHandler {
					return make_battle_message_handler(
						side,
						message
					);
				});
				return BattleStarted(ActionRequired(handler.state(), handler.slot_memory()));
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
			[](ParsedSide const &) -> Result {
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

	auto handle_message(ErrorMessage const error) const -> Result {
		// TODO: Log? Throw an exception sometimes?
		std::cerr << "|error|" << error.message() << '\n';
		if (error.message() == "[Invalid choice] There's nothing to choose") {
			return BattleContinues();
		} else {
			return BattleResponseError();
		}
	}
private:
	BattleState m_battle = BattleExists();
};

} // namespace technicalmachine::ps
