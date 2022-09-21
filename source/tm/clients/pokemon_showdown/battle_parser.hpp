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

#include <tm/generation_generic.hpp>
#include <tm/status_name.hpp>
#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/string.hpp>

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

struct BattleInterface {
	virtual auto id() const -> std::string_view = 0;
	virtual auto handle_message(InMessage message) -> bounded::optional<containers::string> = 0;
	enum class Complete {
		none,
		start,
		finish
	};
	virtual auto completed() const -> Complete = 0;
	virtual ~BattleInterface();
};

struct BattleParser final : BattleInterface {
	BattleParser(
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
		m_battle_logger(std::move(battle_logger)),
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_ai_party(party)
	{
	}

	auto handle_message(InMessage message) -> bounded::optional<containers::string> final;

	auto id() const -> std::string_view final {
		return m_id;
	}
	auto completed() const -> BattleInterface::Complete final {
		return m_battle_manager->completed() ? BattleInterface::Complete::finish : BattleInterface::Complete::none;
	}

private:
	auto is_ai(Party const party) const -> bool {
		return party == m_ai_party;
	}

	auto handle_damage(InMessage message) -> bounded::optional<containers::string> ;

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
	auto move_response(MoveName const move) const -> containers::string;
	auto random_move_response() -> containers::string;
	auto handle_delayed_switch(Party const party) -> bounded::optional<containers::string>;

	SlotMemory m_slot_memory;
	std::unique_ptr<BattleManager> m_battle_manager;

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
	bool m_already_replaced_fainted_end_of_turn = false;
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
