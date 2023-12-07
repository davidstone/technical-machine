// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <iostream>
#include <string_view>

export module tm.clients.ps.battle_manager;

import tm.clients.ps.battle_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.battle_message_result;
import tm.clients.ps.event_block;
import tm.clients.ps.make_battle_message_handler;
import tm.clients.ps.room;

import tm.clients.party;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;

import tm.string_conversions.team;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.usage_stats;

import tm.generation;
import tm.generation_generic;
import tm.team;

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
	constexpr explicit BattleManager(Generation const generation):
		m_battle(BattleExists(generation))
	{
	}

	auto handle_message(
		TeamMessage const message,
		std::string_view,
		Room,
		std::filesystem::path const &,
		AllEvaluate,
		AllUsageStats const &,
		Depth
	) -> BattleContinues {
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

	auto handle_message(
		BattleInitMessage const message,
		std::string_view const user,
		Room const room,
		std::filesystem::path const & log_directory,
		AllEvaluate const evaluate,
		AllUsageStats const & usage_stats,
		Depth const depth
	) -> BattleMessageResult {
		// TODO: This should return BattleStarted
		return tv::visit(m_battle, tv::overload(
			[](BattleExists) -> BattleMessageResult {
				throw std::runtime_error("Received a BattleInitMessage before getting a team");
			},
			[&](BattleTeam & team) -> BattleMessageResult {
				auto & handler = m_battle.emplace([&] -> BattleMessageHandler {
					return make_battle_message_handler(
						std::move(team),
						user,
						evaluate,
						usage_stats[get_generation<KnownTeam>(team)],
						depth,
						AnalysisLogger(log_directory / room),
						message
					);
				});
				return handler.move_response();
			},
			[](BattleMessageHandler const &) -> BattleMessageResult {
				throw std::runtime_error("Tried to initialize an existing battle");
			}
		));
	}

	auto handle_message(
		EventBlock const & message,
		std::string_view,
		Room,
		std::filesystem::path const &,
		AllEvaluate,
		AllUsageStats const &,
		Depth
	) -> BattleMessageResult {
		return tv::visit(m_battle, tv::overload(
			[](BattleExists) -> BattleMessageResult {
				throw std::runtime_error("Received an event before getting a team");
			},
			[](BattleTeam const &) -> BattleMessageResult {
				throw std::runtime_error("Received an event before a BattleInitMessage");
			},
			[&](BattleMessageHandler & handler) -> BattleMessageResult {
				auto const finished = handler.handle_message(message);
				if (finished) {
					return BattleFinished();
				}
				return handler.move_response();
			}
		));
	}

	auto handle_message(
		ErrorMessage const error,
		std::string_view,
		Room,
		std::filesystem::path const &,
		AllEvaluate,
		AllUsageStats const &,
		Depth
	) const -> BattleMessageResult {
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
