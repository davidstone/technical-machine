// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.is_chat_message;

import tm.clients.ps.in_message;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace containers::string_literals;

constexpr auto chat_message_types = containers::array{
	":"_s,
	"c"_s,
	"chat"_s,
	"c:"_s,
	"deinit"_s,
	"expire"_s,
	"j"_s,
	"J"_s,
	"join"_s,
	"l"_s,
	"L"_s,
	"leave"_s,
	"n"_s,
	"N"_s,
	"name"_s
};

export constexpr auto is_chat_message(InMessage const message) -> bool {
	return containers::any_equal(chat_message_types, message.type());
}

} // namespace technicalmachine::ps
