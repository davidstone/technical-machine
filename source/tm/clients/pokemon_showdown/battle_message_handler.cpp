// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_message_handler;

import tm.clients.ps.action_required;
import tm.clients.ps.end_of_turn_state;
import tm.clients.ps.move_state;
import tm.clients.ps.parsed_message;
import tm.clients.ps.slot_memory;
import tm.clients.ps.switch_message;
import tm.clients.ps.start_of_turn;

import tm.clients.battle_finished;
import tm.clients.client_battle;
import tm.clients.party;
import tm.clients.teams;
import tm.clients.turn_count;

import tm.move.move_name;

import tm.pokemon.max_pokemon_per_team;

import tm.status.status_name;

import tm.generation_generic;
import tm.visible_hp;
import tm.visible_state;

import tv;
import std_module;

namespace technicalmachine::ps {

export struct BattleMessageHandler {
	BattleMessageHandler(Party party, GenerationGeneric<Teams> teams);

	using Result = tv::variant<
		ActionRequired,
		StartOfTurn,
		BattleFinished
	>;
	auto handle_message(std::span<ParsedMessage const> const block) -> Result;

	auto state() const -> GenerationGeneric<VisibleState> {
		return m_client_battle->state();
	}

	auto slot_memory() const -> SlotMemory {
		return m_slot_memory;
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
