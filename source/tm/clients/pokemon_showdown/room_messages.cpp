// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.room_messages;

import tm.clients.ps.room_message;

import tm.split_view;

import containers;
import std_module;

namespace technicalmachine::ps {

constexpr auto has_room(std::string_view const str) -> bool {
	return !str.empty() and str.front() == '>';
}

using SplitView = std::pair<std::string_view, std::string_view>;

export struct RoomMessages {
	constexpr explicit RoomMessages(std::string_view const str):
		RoomMessages(has_room(str) ? split_view(str.substr(1), '\n') : SplitView("", str))
	{
	}

	constexpr auto room() const -> std::string_view {
		return m_room;
	}
	constexpr auto messages() const {
		return containers::transform(
			containers::split(m_messages, '\n'),
			[&](std::string_view const message) {
				return RoomMessage(m_room, message);
			}
		);
	}

private:
	constexpr explicit RoomMessages(SplitView const both):
		m_room(both.first),
		m_messages(both.second)
	{
	}

	std::string_view m_room;
	std::string_view m_messages;
};

} // namespace technicalmachine::ps
