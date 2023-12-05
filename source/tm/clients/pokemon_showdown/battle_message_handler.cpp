// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_message_handler;

import tm.clients.ps.battle_message_result;
import tm.clients.ps.end_of_turn_state;
import tm.clients.ps.event_block;
import tm.clients.ps.move_state;
import tm.clients.ps.slot_memory;
import tm.clients.ps.switch_message;

import tm.clients.client_battle;
import tm.clients.client_battle_inputs;
import tm.clients.party;

import tm.evaluate.analysis_logger;
import tm.evaluate.depth;
import tm.evaluate.state;

import tm.move.is_switch;
import tm.move.move_name;

import tm.pokemon.max_pokemon_per_team;

import tm.status.status_name;

import tm.team_predictor.usage_stats;

import tm.generation_generic;
import tm.visible_hp;

import tv;
import std_module;

namespace technicalmachine::ps {

export struct BattleMessageHandler {
	BattleMessageHandler(
		Party party,
		GenerationGeneric<ClientBattleInputs> inputs,
		AnalysisLogger analysis_logger,
		UsageStats const & usage_stats,
		Depth const depth
	);

	auto handle_message(EventBlock const & block) -> bool;

	auto state() const -> GenerationGeneric<State> {
		return m_client_battle->state();
	}

	auto switch_index_to_switch(BattleResponseSwitch const index) const -> MoveName {
		return to_switch(m_slot_memory.reverse_lookup(index));
	}

	auto move_response() -> BattleMessageResult {
		auto const move = m_client_battle->determine_action();
		return
			move == MoveName::Pass ? BattleMessageResult(BattleContinues()) :
			is_switch(move) ? BattleMessageResult(m_slot_memory[to_replacement(move)]) :
			BattleMessageResult(move);
	}

private:
	auto use_move(MoveState) -> void;
	auto handle_switch_message(SwitchMessage) -> TeamIndex;

	auto handle_end_of_turn(EndOfTurnState) -> void;

	auto require_is_end_of_turn() const -> void {
		if (!m_client_battle->is_end_of_turn()) {
			throw std::runtime_error("Got end-of-turn message before the end of turn");
		}
	}

	auto try_correct_hp_and_status(bool const is_ai, tv::optional<VisibleHP>, tv::optional<StatusName>) -> void;

	SlotMemory m_slot_memory;
	std::unique_ptr<ClientBattle> m_client_battle;
	Party m_party;
};

} // namespace technicalmachine::ps
