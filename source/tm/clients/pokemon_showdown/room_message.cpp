// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.room_message;

import tm.clients.ps.in_message;

import std_module;

namespace technicalmachine::ps {

export struct RoomMessage {
	std::string_view room;
	InMessage message;
};

} // namespace technicalmachine::ps
