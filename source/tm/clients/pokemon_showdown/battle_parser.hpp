// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/clients/pokemon_showdown/battle_logger.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/clients/party.hpp>

#include <tm/evaluate/depth.hpp>
#include <tm/evaluate/evaluate.hpp>

#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <functional>
#include <fstream>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace technicalmachine {

struct AllUsageStats;

namespace ps {

using SendMessageFunction = std::function<void(std::string_view)>;

template<Generation generation>
struct Teams {
	Team<generation> ai;
	Team<generation> foe;
};
using GenericTeams = bounded::variant<
	Teams<Generation::one>,
	Teams<Generation::two>,
	Teams<Generation::three>,
	Teams<Generation::four>,
	Teams<Generation::five>,
	Teams<Generation::six>,
	Teams<Generation::seven>,
	Teams<Generation::eight>
>;

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
	std::string id,
	std::string username,
	AllUsageStats const & usage_stats,
	AllEvaluate evaluate,
	Party party,
	DepthValues const depth,
	std::mt19937 random_engine,
	GenericTeams const & teams,
	bool log_foe_teams
) -> std::unique_ptr<BattleParser>;


constexpr auto make_party(std::string_view const player_id) {
	if (player_id == "p1") {
		return Party(0_bi);
	} else if (player_id == "p2") {
		return Party(1_bi);
	} else {
		throw std::runtime_error("Invalid player id: " + std::string(player_id));
	}
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
	Level level;
	Gender gender;
	VisibleHP hp;
	Statuses status;
};
auto parse_switch(InMessage message) -> ParsedSwitch;

}	// namespace ps
}	// namespace technicalmachine
