// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.is_chat_message_block;

import tm.clients.ps.in_message;
import tm.clients.ps.message_block;

import containers;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

constexpr auto chat_message_types = containers::array({
	":"sv,
	"c"sv,
	"chat"sv,
	"c:"sv,
	"deinit"sv,
	"expire"sv,
	"j"sv,
	"J"sv,
	"join"sv,
	"l"sv,
	"L"sv,
	"leave"sv,
	"n"sv,
	"N"sv,
	"name"sv
});

export constexpr auto is_chat_message_block(MessageBlock const messages) -> bool {
	return containers::any_equal(chat_message_types, containers::front(messages).type());
}

} // namespace technicalmachine::ps
