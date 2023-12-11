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
import tm.clients.ps.battle_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.battle_started;
import tm.clients.ps.event_block;
import tm.clients.ps.make_battle_message_handler;
import tm.clients.ps.slot_memory;
import tm.clients.ps.start_of_turn;

import tm.clients.battle_continues;
import tm.clients.battle_finished;
import tm.clients.battle_response_error;
import tm.clients.party;
import tm.clients.turn_count;

import tm.string_conversions.team;

import tm.generation;
import tm.generation_generic;
import tm.team;
import tm.visible_state;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

struct BattleExists {
	Generation generation;
};

using BattleTeam = GenerationGeneric<KnownTeam>;

using BattleState = tv::variant<
	BattleExists,
	BattleTeam,
	BattleMessageHandler
>;

// We essentially create a state machine.
// (nothing) + CreateBattle => BattleExists
// BattleExists + TeamMessage => BattleTeam
// BattleTeam + TeamMessage => BattleTeam (check they are the same)
// BattleTeam + BattleInitMessage => BattleMessageHandler
// BattleMessageHandler + TeamMessage => BattleMessageHandler (check they are the same)
// BattleMessageHandler + EventBlock => BattleMessageHandler or (nothing)

constexpr auto team_to_string = [](auto const & team) {
	return to_string(team);
};

export struct BattleManager {
	explicit constexpr BattleManager(Generation const generation):
		m_battle(BattleExists(generation))
	{
	}

	using Result = tv::variant<
		ActionRequired,
		StartOfTurn,
		BattleFinished,
		BattleContinues,
		BattleStarted,
		BattleResponseError
	>;

	auto handle_message(TeamMessage const message, std::string_view) -> Result {
		tv::visit(m_battle, tv::overload(
			[&](BattleExists const exists) {
				m_battle.emplace([&] -> BattleTeam {
					return message.team(exists.generation);
				});
			},
			[&](BattleTeam const & team) {
				auto const generation = get_generation<KnownTeam>(team);
				auto const received = message.team(generation);
				if (team != received) {
					throw std::runtime_error(containers::concatenate<std::string>(
						"Mismatched teams -- expected:\n"sv,
						tv::visit(team, team_to_string),
						"\nbut got:\n"sv,
						tv::visit(received, team_to_string)
					));
				}
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

	auto handle_message(BattleInitMessage const message, std::string_view const user) -> Result {
		return tv::visit(m_battle, tv::overload(
			[](BattleExists) -> BattleStarted {
				throw std::runtime_error("Received a BattleInitMessage before getting a team");
			},
			[&](BattleTeam & team) -> BattleStarted {
				auto & handler = m_battle.emplace([&] -> BattleMessageHandler {
					return make_battle_message_handler(
						std::move(team),
						user,
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

	auto handle_message(EventBlock const & message, std::string_view) -> Result {
		return tv::visit(m_battle, tv::overload(
			[](BattleExists) -> Result {
				throw std::runtime_error("Received an event before getting a team");
			},
			[](BattleTeam const &) -> Result {
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

	auto handle_message(ErrorMessage const error, std::string_view) const -> Result {
		// TODO: Log? Throw an exception sometimes?
		std::cerr << "|error|" << error.message() << '\n';
		if (error.message() == "[Invalid choice] There's nothing to choose") {
			return BattleContinues();
		} else {
			return BattleResponseError();
		}
	}
private:
	BattleState m_battle;
};

} // namespace technicalmachine::ps
