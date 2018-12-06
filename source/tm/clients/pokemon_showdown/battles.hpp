// Handles challenges / current battles
// Copyright (C) 2018 David Stone
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
#include <tm/clients/pokemon_showdown/../no_pending_challenges.hpp>

#include <algorithm>
#include <list>
#include <string>
#include <unordered_map>

namespace technicalmachine {
namespace ps {

struct Battles {
	template<typename ... Args>
	void add_pending(Args && ... args) {
		m_pending.emplace_back(std::forward<Args>(args)...);
	}
	
	bool handle_message(InMessage message, boost::beast::websocket::stream<boost::asio::ip::tcp::socket &> & websocket) {
		if (handle_message_impl(m_active, message, [&](auto const it) { m_active.erase(it); })) {
			return true;
		}
		if (handle_message_impl(m_pending, message, [&](auto const it) { move_to_active(it, websocket); })) {
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
	
	void move_to_active(std::list<BattleFactory>::iterator it, boost::beast::websocket::stream<boost::asio::ip::tcp::socket &> & websocket) {
		m_active.push_back(std::move(*it).make(websocket));
		m_pending.erase(it);
	}

	std::list<BattleFactory> m_pending;
	std::list<BattleParser> m_active;
};

}	// namespace ps
}	// namespace technicalmachine
