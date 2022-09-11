// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/battle_logger.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>

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
	virtual auto handle_message(InMessage message) -> void = 0;
	virtual auto id() const -> std::string_view = 0;
	virtual auto completed() const -> bool = 0;
	virtual ~BattleParser();
};

auto make_battle_parser(
	SendMessageFunction send_message,
	BattleLogger battle_logger,
	std::ofstream analysis_logger,
	containers::string id,
	containers::string username,
	AllUsageStats const & usage_stats,
	AllEvaluate evaluate,
	Party party,
	DepthValues const depth,
	std::mt19937 random_engine,
	GenerationGeneric<Teams> teams,
	bool log_foe_teams
) -> std::unique_ptr<BattleParser>;


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
