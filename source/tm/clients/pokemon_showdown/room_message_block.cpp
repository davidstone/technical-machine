// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.room_message_block;

import tm.clients.ps.room;

import tm.split_view;

import bounded;
import containers;
import std_module;

namespace technicalmachine::ps {
using namespace bounded::literal;
using namespace containers::string_literals;

constexpr auto has_room(containers::string_view const str) -> bool {
	return !containers::is_empty(str) and containers::front(str) == '>';
}

using SplitView = std::pair<containers::string_view, containers::string_view>;

export struct RoomMessageBlock {
	struct FirstLineIsRoom {};

	constexpr explicit RoomMessageBlock(containers::string_view const input):
		RoomMessageBlock(has_room(input) ?
			split_view(containers::drop_exactly(input, 1_bi), '\n') :
			SplitView(""_s, input)
		)
	{
	}

	constexpr explicit RoomMessageBlock(containers::string_view const input, FirstLineIsRoom):
		RoomMessageBlock(split_view(input, '\n'))
	{
	}

	constexpr auto room() const -> Room {
		return m_room;
	}
	constexpr auto str() const -> containers::string_view {
		return m_str;
	}

private:
	constexpr explicit RoomMessageBlock(SplitView const both):
		m_room(both.first),
		m_str(both.second)
	{
	}

	Room m_room;
	containers::string_view m_str;
};

} // namespace technicalmachine::ps
