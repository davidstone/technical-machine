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

#include <algorithm>
#include <list>
#include <filesystem>
#include <string>

namespace technicalmachine {
namespace ps {

struct Battles {
	explicit Battles(std::filesystem::path log_directory):
		m_log_directory(std::move(log_directory))
	{
		std::filesystem::create_directories(m_log_directory);
	}

	template<typename ... Args>
	void add_pending(Args && ... args) {
		m_pending.emplace_back(m_log_directory, std::forward<Args>(args)...);
	}
	
	template<typename SendMessageFunction>
	bool handle_message(InMessage message, SendMessageFunction send_message) {
		auto complete_active_battle = [&](auto const it) {
			m_active.erase(it);
			send_message("|/leave " + std::string(message.room()));
		};
		if (handle_message_impl(m_active, message, complete_active_battle)) {
			return true;
		}
		auto begin_pending_battle = [&](auto const it) { move_to_active(it, std::move(send_message)); };
		if (handle_message_impl(m_pending, message, begin_pending_battle)) {
			return true;
		}

		return false;
	}

private:
	template<typename Container, typename Function>
	static bool handle_message_impl(Container & container, InMessage message, Function if_completed) {
		auto const function = [=](auto const & battle) { return battle.id() == message.room(); };
		auto const it = std::find_if(container.begin(), container.end(), function);
		if (it == container.end()) {
			return false;
		}
		it->handle_message(message);
		if (it->completed()) {
			if_completed(it);
		}
		return true;
	}
	
	template<typename SendMessageFunction>
	void move_to_active(std::list<BattleFactory>::iterator it, SendMessageFunction send_message) {
		m_active.push_back(std::move(*it).make(std::move(send_message)));
		m_pending.erase(it);
	}

	std::filesystem::path m_log_directory;
	std::list<BattleFactory> m_pending;
	std::list<BattleParser> m_active;
};

}	// namespace ps
}	// namespace technicalmachine
