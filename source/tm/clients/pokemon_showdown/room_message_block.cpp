// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.room_message_block;

import tm.clients.ps.room;

import tm.split_view;

import std_module;

namespace technicalmachine::ps {

constexpr auto has_room(std::string_view const str) -> bool {
	return !str.empty() and str.front() == '>';
}

using SplitView = std::pair<std::string_view, std::string_view>;

export struct RoomMessageBlock {
	struct FirstLineIsRoom {};

	constexpr explicit RoomMessageBlock(std::string_view const input):
		RoomMessageBlock(has_room(input) ? split_view(input.substr(1), '\n') : SplitView("", input))
	{
	}

	constexpr explicit RoomMessageBlock(std::string_view const input, FirstLineIsRoom):
		RoomMessageBlock(split_view(input, '\n'))
	{
	}

	constexpr auto room() const -> Room {
		return m_room;
	}
	constexpr auto str() const -> std::string_view {
		return m_str;
	}

private:
	constexpr explicit RoomMessageBlock(SplitView const both):
		m_room(both.first),
		m_str(both.second)
	{
	}

	Room m_room;
	std::string_view m_str;
};

} // namespace technicalmachine::ps
