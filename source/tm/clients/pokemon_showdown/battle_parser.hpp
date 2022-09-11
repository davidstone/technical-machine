// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/battle_logger.hpp>
#include <tm/clients/pokemon_showdown/end_of_turn_state.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>
#include <tm/clients/pokemon_showdown/move_state.hpp>
#include <tm/clients/pokemon_showdown/slot_memory.hpp>

#include <tm/clients/battle_manager.hpp>
#include <tm/clients/party.hpp>

#include <tm/evaluate/depth.hpp>
#include <tm/evaluate/evaluate.hpp>

#include <tm/generation_generic.hpp>
#include <tm/known_team.hpp>
#include <tm/seen_team.hpp>
#include <tm/status_name.hpp>
#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/trivial_inplace_function.hpp>

#include <fstream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace technicalmachine {

using namespace std::string_view_literals;

struct AllUsageStats;

namespace ps {

using SendMessageFunction = containers::trivial_inplace_function<void(std::string_view) const, sizeof(void *)>;

struct BattleParser {
	BattleParser(
		SendMessageFunction send_message,
		BattleLogger battle_logger,
		std::ofstream analysis_logger,
		containers::string id_,
		containers::string username,
		AllUsageStats const & usage_stats,
		AllEvaluate evaluate,
		Party party,
		DepthValues const depth,
		std::mt19937 random_engine,
		GenerationGeneric<Teams> generic_teams,
		bool log_foe_teams
	):
		m_slot_memory(bounded::visit(generic_teams, [](auto const & teams) { return teams.ai.size(); })),
		m_battle_manager(make_battle_manager(
			std::move(analysis_logger),
			usage_stats,
			std::move(evaluate),
			depth,
			std::move(random_engine),
			std::move(generic_teams),
			log_foe_teams
		)),
		m_send_message(std::move(send_message)),
		m_battle_logger(std::move(battle_logger)),
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_ai_party(party)
	{
		m_send_message(containers::concatenate<containers::string>(m_id, "|/timer on"sv));
	}

	auto handle_message(InMessage message) -> void;

	auto id() const -> std::string_view {
		return m_id;
	}
	auto completed() const -> bool {
		return m_battle_manager->completed();
	}

private:
	auto is_ai(Party const party) const -> bool {
		return party == m_ai_party;
	}

	auto handle_damage(InMessage message) -> void;

	struct SwitchOrDragResult {
		Party party;
		Species species;
		MoveName move;
		VisibleHP hp;
		StatusName status;
	};
	auto handle_switch_or_drag(InMessage message) -> SwitchOrDragResult;
	auto set_value_on_pokemon(Party const party, auto const value) -> void;
	auto queue_hp_or_status_checks(Party const party, auto const value) -> void;

	struct Switch {
		MoveName move;
		VisibleHP hp;
		StatusName status;
	};
	auto get_switch(Party const party) -> bounded::optional<Switch> & {
		return is_ai(party) ? m_ai_switch : m_foe_switch;
	}

	auto maybe_commit_switch(Party const party) -> void;
	auto maybe_use_previous_move() -> void;
	auto maybe_use_previous_move_impl() -> void;
	auto try_correct_hp_and_status(bool const is_ai, bounded::optional<VisibleHP> const hp, bounded::optional<StatusName> const status, auto... maybe_index) -> void;
	auto send_move_impl(bool const is_switch, auto const switch_move, auto const move_index) -> void;
	auto send_move(MoveName const move) -> void;
	auto send_random_move() -> void;
	auto handle_delayed_switch(Party const party) -> void;

	SlotMemory m_slot_memory;
	std::unique_ptr<BattleManager> m_battle_manager;

	SendMessageFunction m_send_message;
	BattleLogger m_battle_logger;
	containers::string m_id;
	containers::string m_username;

	Party m_ai_party;
	MoveState m_move_state;
	EndOfTurnState m_end_of_turn_state;

	bounded::optional<Switch> m_ai_switch;
	bounded::optional<Switch> m_foe_switch;

	bool m_ignore_next_cure_status = false;
	bool m_replacing_fainted = false;
};

constexpr auto make_party(std::string_view const party_str) {
	if (party_str == "p1") {
		return Party(0_bi);
	} else if (party_str == "p2") {
		return Party(1_bi);
	} else {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid player id: "sv, party_str));
	}
}

constexpr auto parse_identity(std::string_view const str) {
	struct result {
		Party party;
		std::string_view nickname;
	};
	auto const [party_str, nickname] = split_view(str, ": "sv);
	return result{make_party(party_str.substr(0, 2)), nickname};
}

// This adds additional string scanning, but I don't think the performance
// matters that much here
struct ParsedDetails {
	Species species;
	Level level;
	Gender gender;
};
auto parse_details(std::string_view details) -> ParsedDetails;

struct ParsedSwitch {
	Party party;
	Species species;
	containers::string nickname;
	Level level;
	Gender gender;
	VisibleHP hp;
	StatusName status;
};
auto parse_switch(InMessage message) -> ParsedSwitch;

} // namespace ps
} // namespace technicalmachine
