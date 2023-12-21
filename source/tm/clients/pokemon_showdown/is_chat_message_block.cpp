// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.is_chat_message_block;

import tm.clients.ps.is_chat_message;
import tm.clients.ps.message_block;

import containers;

namespace technicalmachine::ps {

export constexpr auto is_chat_message_block(MessageBlock const messages) -> bool {
	return is_chat_message(containers::front(messages));
}

} // namespace technicalmachine::ps
