// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_message_handler;

import tm.clients.ps.action_required;
import tm.clients.ps.battle_init_message;
import tm.clients.ps.end_of_turn_state;
import tm.clients.ps.move_state;
import tm.clients.ps.parsed_message;
import tm.clients.ps.parsed_request;
import tm.clients.ps.slot_memory;
import tm.clients.ps.switch_message;
import tm.clients.ps.start_of_turn;

import tm.clients.battle_finished;
import tm.clients.client_battle;
import tm.clients.party;
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

struct SwitchState {
	Party party;
	TeamIndex index;
	StatusName status;
	VisibleHP hp;
};

struct HitSelf {
	Party party;
	StatusName status;
	VisibleHP hp;
};

export struct BattleMessageHandler {
	BattleMessageHandler(ParsedRequest const &, BattleInitMessage);

	// We get this message too early so we save it for later.
	// https://github.com/smogon/pokemon-showdown/issues/8546
	auto save_request(ParsedRequest const & request) -> void {
		if (m_request) {
			throw std::runtime_error("Got multiple requests in a row");
		}
		tv::insert(m_request, request);
	}

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
	auto exchange_request() -> ParsedRequest {
		if (!m_request) {
			throw std::runtime_error("No cached request");
		}
		auto result = *std::move(m_request);
		m_request = tv::none;
		return result;
	}
private:
	auto use_move(MoveState) -> void;
	auto use_switch(SwitchState) -> void;
	auto hit_self_in_confusion(HitSelf) -> void;
	auto handle_switch_message(SwitchMessage) -> TeamIndex;

	auto handle_end_of_turn(EndOfTurnState) -> void;

	auto require_is_end_of_turn() const -> void {
		if (!m_client_battle->is_end_of_turn()) {
			throw std::runtime_error("Got end-of-turn message before the end of turn");
		}
	}

	auto try_correct_hp_and_status(bool const is_ai, tv::optional<VisibleHP>, tv::optional<StatusName>) -> void;

	SlotMemory m_slot_memory;
	tv::optional<ParsedRequest> m_request;
	std::unique_ptr<ClientBattle> m_client_battle;
	Party m_party;
};

} // namespace technicalmachine::ps
