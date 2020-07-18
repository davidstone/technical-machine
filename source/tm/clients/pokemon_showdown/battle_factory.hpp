// Handle Pokemon Showdown messages setting up a battle
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

#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/evaluate/depth.hpp>
#include <tm/evaluate/evaluate.hpp>

#include <filesystem>
#include <memory>
#include <random>
#include <string>
#include <string_view>

namespace technicalmachine {

struct AllUsageStats;

namespace ps {

struct BattleFactory {
	virtual auto id() const -> std::string_view = 0;
	virtual auto handle_message(InMessage message) -> void = 0;
	virtual auto completed() const -> bool = 0;
	virtual auto make(AllUsageStats const & usage_stats, SendMessageFunction send_message) && -> std::unique_ptr<BattleParser> = 0;
	virtual ~BattleFactory();
};

auto make_battle_factory(
	std::filesystem::path const & base_log_directory,
	bool const log_foe_teams,
	std::string id,
	std::string username,
	AllEvaluate evaluate,
	DepthValues depth,
	std::mt19937 random_engine
) -> std::unique_ptr<BattleFactory>;

} // namespace ps
} // namespace technicalmachine
