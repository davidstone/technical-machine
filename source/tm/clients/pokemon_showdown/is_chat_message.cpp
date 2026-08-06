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
	":"_sv,
	"c"_sv,
	"chat"_sv,
	"c:"_sv,
	"deinit"_sv,
	"expire"_sv,
	"j"_sv,
	"J"_sv,
	"join"_sv,
	"l"_sv,
	"L"_sv,
	"leave"_sv,
	"n"_sv,
	"N"_sv,
	"name"_sv
};

export constexpr auto is_chat_message(InMessage const message) -> bool {
	return containers::any_equal(chat_message_types, message.type());
}

} // namespace technicalmachine::ps
