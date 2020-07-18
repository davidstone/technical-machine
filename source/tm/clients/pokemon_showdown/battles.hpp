// Handles challenges / current battles
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

#include <tm/clients/pokemon_showdown/battle_factory.hpp>
#include <tm/clients/pokemon_showdown/battle_parser.hpp>

#include <containers/emplace_back.hpp>
#include <containers/vector.hpp>

#include <algorithm>
#include <filesystem>
#include <string>

namespace technicalmachine {

struct AllUsageStats;

namespace ps {

struct Battles {
	explicit Battles(std::filesystem::path log_directory, bool const log_foe_teams):
		m_log_directory(std::move(log_directory)),
		m_log_foe_teams(log_foe_teams)
	{
		std::filesystem::create_directories(m_log_directory);
	}

	void add_pending(auto && ... args) {
		containers::push_back(
			m_pending,
			make_battle_factory(m_log_directory, m_log_foe_teams, OPERATORS_FORWARD(args)...)
		);
	}
	
	bool handle_message(AllUsageStats const & usage_stats, InMessage message, auto send_message, auto challenge) {
		auto complete_active_battle = [&](Active::const_iterator const it) {
			containers::erase(m_active, it);
			send_message("|/leave " + std::string(message.room()));
			challenge();
		};
		if (handle_message_impl(m_active, message, complete_active_battle)) {
			return true;
		}
		auto begin_pending_battle = [&](auto const it) { move_to_active(usage_stats, it, std::move(send_message)); };
		if (handle_message_impl(m_pending, message, begin_pending_battle)) {
			return true;
		}

		return false;
	}

private:
	using Pending = containers::vector<std::unique_ptr<BattleFactory>>;
	using Active = containers::vector<std::unique_ptr<BattleParser>>;

	static bool handle_message_impl(auto & container, InMessage message, auto if_completed) {
		auto const function = [=](auto const & battle) { return battle->id() == message.room(); };
		auto const it = containers::find_if(container, function);
		if (it == end(container)) {
			return false;
		}
		(*it)->handle_message(message);
		if ((*it)->completed()) {
			if_completed(it);
		}
		return true;
	}
	
	void move_to_active(AllUsageStats const & usage_stats, Pending::iterator it, auto send_message) {
		containers::push_back(m_active, std::move(**it).make(usage_stats, std::move(send_message)));
		containers::erase(m_pending, it);
	}

	std::filesystem::path m_log_directory;
	Pending m_pending;
	Active m_active;
	bool m_log_foe_teams;
};

}	// namespace ps
}	// namespace technicalmachine
