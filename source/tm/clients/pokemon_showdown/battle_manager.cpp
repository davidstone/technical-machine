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
import tm.clients.determine_action;
import tm.clients.ps.event_block;
import tm.clients.ps.make_battle_message_handler;

import tm.clients.action_required;
import tm.clients.battle_continues;
import tm.clients.battle_finished;
import tm.clients.battle_response_error;
import tm.clients.party;
import tm.clients.turn_count;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.evaluate;

import tm.move.is_switch;
import tm.move.move_name;

import tm.string_conversions.team;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.usage_stats;

import tm.constant_generation;
import tm.generation;
import tm.generation_generic;
import tm.team;
import tm.visible_state;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
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

auto print_begin_turn(AnalysisLogger & logger, TurnCount const turn_count) -> void {
	logger << containers::string(containers::repeat_n(20_bi, '=')) << "\nBegin turn " << turn_count << '\n';
}

export struct BattleManager {
	// The lifetime of the object referred to by `usage_stats` must exceed the
	// lifetime of this object.
	explicit BattleManager(
		Generation const generation,
		UsageStats const & usage_stats,
		AnalysisLogger analysis_logger,
		AllEvaluate const evaluate,
		Depth const depth
	):
		m_battle(BattleExists(generation)),
		m_usage_stats(usage_stats),
		m_analysis_logger(std::move(analysis_logger)),
		m_evaluate(constant_generation(
			generation,
			[&]<Generation gen>(constant_gen_t<gen>) -> GenerationGeneric<Evaluate> {
				return evaluate.get<gen>();
			}
		)),
		m_depth(depth)
	{
	}

	auto handle_message(TeamMessage const message, std::string_view) -> BattleContinues {
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

	auto handle_message(BattleInitMessage const message, std::string_view const user) -> BattleMessageResult {
		tv::visit(m_battle, tv::overload(
			[](BattleExists) -> void {
				throw std::runtime_error("Received a BattleInitMessage before getting a team");
			},
			[&](BattleTeam & team) -> void {
				m_battle.emplace([&] -> BattleMessageHandler {
					return make_battle_message_handler(
						std::move(team),
						user,
						message
					);
				});
			},
			[](BattleMessageHandler const &) -> void {
				throw std::runtime_error("Tried to initialize an existing battle");
			}
		));
		// TODO: This should return BattleStarted
		return move_response();
	}

	auto handle_message(EventBlock const & message, std::string_view) -> BattleMessageResult {
		auto const result = tv::visit(m_battle, tv::overload(
			[](BattleExists) -> BattleMessageHandler::Result {
				throw std::runtime_error("Received an event before getting a team");
			},
			[](BattleTeam const &) -> BattleMessageHandler::Result {
				throw std::runtime_error("Received an event before a BattleInitMessage");
			},
			[&](BattleMessageHandler & handler) -> BattleMessageHandler::Result {
				return handler.handle_message(message);
			}
		));
		return tv::visit(result, tv::overload(
			[&](ActionRequired) -> BattleMessageResult {
				return move_response();
			},
			[&](TurnCount const count) -> BattleMessageResult {
				print_begin_turn(m_analysis_logger, count);
				return move_response();
			},
			[](BattleFinished) -> BattleMessageResult {
				return BattleFinished();
			}
		));
	}

	auto handle_message(ErrorMessage const error, std::string_view) const -> BattleMessageResult {
		// TODO: Log? Throw an exception sometimes?
		std::cerr << "|error|" << error.message() << '\n';
		if (error.message() == "[Invalid choice] There's nothing to choose") {
			return BattleContinues();
		} else {
			return BattleResponseError();
		}
	}

private:
	auto move_response() -> BattleMessageResult {
		auto const & handler = m_battle[bounded::type<BattleMessageHandler>];
		auto const generic_state = handler.state();
		auto const move = tv::visit(generic_state, m_evaluate, tv::overload(
			[&]<Generation generation>(VisibleState<generation> const & state, Evaluate<generation> const evaluate) -> MoveName {
				return determine_action(
					state,
					m_analysis_logger,
					m_usage_stats,
					evaluate,
					m_depth
				);
			},
			[](auto const &, auto const &) -> MoveName { std::unreachable(); }
		));
		auto const slot_memory = handler.slot_memory();
		return
			move == MoveName::Pass ? BattleMessageResult(BattleContinues()) :
			is_switch(move) ? BattleMessageResult(slot_memory[to_replacement(move)]) :
			BattleMessageResult(move);
	}

	BattleState m_battle;
	UsageStats const & m_usage_stats;
	AnalysisLogger m_analysis_logger;
	GenerationGeneric<Evaluate> m_evaluate;
	Depth m_depth;
};

} // namespace technicalmachine::ps
