// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <ostream>
#include <string_view>

export module tm.clients.ps.battles;

import tm.clients.ps.action_required;
import tm.clients.ps.battle_manager;
import tm.clients.ps.battle_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.battle_response_switch;
import tm.clients.ps.battle_started;
import tm.clients.ps.parse_generation_from_format;
import tm.clients.ps.room;
import tm.clients.ps.slot_memory;
import tm.clients.ps.start_of_turn;

import tm.clients.battle_already_finished;
import tm.clients.battle_continues;
import tm.clients.battle_finished;
import tm.clients.battle_response_error;
import tm.clients.determine_action;
import tm.clients.party;
import tm.clients.turn_count;

import tm.evaluate.all_evaluate;
import tm.evaluate.depth;

import tm.move.is_switch;
import tm.move.move_name;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.usage_stats;

import tm.generation;
import tm.generation_generic;
import tm.open_file;
import tm.team;
import tm.visible_state;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;
using namespace bounded::literal;

auto print_begin_turn(std::ostream & stream, TurnCount const turn_count) -> void {
	stream << containers::string(containers::repeat_n(20_bi, '=')) << "\nBegin turn " << turn_count << '\n';
}

using Result = tv::variant<
	MoveName,
	BattleResponseSwitch,
	BattleFinished,
	BattleContinues,
	BattleStarted,
	BattleResponseError,
	BattleAlreadyFinished
>;
auto move_response(
	GenerationGeneric<VisibleState> const & generic_state,
	SlotMemory const slot_memory,
	std::ostream & stream,
	AllEvaluate const all_evaluate,
	AllUsageStats const & all_usage_stats,
	Depth const depth
) -> Result {
	auto const move = tv::visit(
		generic_state,
		[&]<Generation generation>(VisibleState<generation> const & state) -> MoveName {
			return determine_action(
				state,
				stream,
				all_usage_stats[generation],
				all_evaluate.get<generation>(),
				depth
			);
		}
	);
	return
		move == MoveName::Pass ? Result(BattleContinues()) :
		is_switch(move) ? Result(slot_memory[to_replacement(move)]) :
		Result(move);
}

export struct Battles {
	explicit Battles(std::filesystem::path log_directory):
		m_log_directory(std::move(log_directory))
	{
	}

	using Result = ::technicalmachine::ps::Result;
	auto handle_message(
		Room const room,
		BattleMessage const & generic_message,
		std::string_view const user,
		AllEvaluate const all_evaluate,
		AllUsageStats const & all_usage_stats,
		Depth const depth
	) -> Result {
		auto const it = m_container.find(room);
		return tv::visit(generic_message, tv::overload(
			[&](CreateBattle) -> Result {
				if (it != containers::end(m_container)) {
					throw std::runtime_error("Tried to create an existing battle");
				}
				create_battle(room);
				return BattleContinues();
			},
			[&](auto const & message) -> Result {
				if (it == containers::end(m_container)) {
					return BattleAlreadyFinished();
				}
				auto & battle = it->mapped;
				auto analysis_logger = open_text_file(m_log_directory / room / "analysis.txt");
				auto action_required = [&](ActionRequired const & inputs) {
					return move_response(
						inputs.state,
						inputs.slot_memory,
						analysis_logger,
						all_evaluate,
						all_usage_stats,
						depth
					);
				};
				auto result = battle.handle_message(message, user);
				return tv::visit(std::move(result), tv::overload(
					action_required,
					[&](StartOfTurn const & value) -> Result {
						print_begin_turn(analysis_logger, value.turn_count);
						return action_required(value);
					},
					[&](BattleFinished) -> Result {
						m_container.erase(it);
						return BattleFinished();
					},
					[&](BattleStarted const & value) -> Result {
						return action_required(value);
					},
					[](auto value) -> Result {
						return value;
					}
				));
			}
		));
	}

private:
	auto create_battle(Room const room) -> void {
		m_container.lazy_insert(
			containers::string(room),
			[&] {
				auto const generation = parse_generation_from_format(room, "battle-gen"sv);
				return BattleManager(generation);
			}
		);
	}

	std::filesystem::path m_log_directory;
	containers::linear_map<containers::string, BattleManager> m_container;
};

} // namespace technicalmachine::ps
