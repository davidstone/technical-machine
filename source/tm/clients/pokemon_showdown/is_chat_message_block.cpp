export module tm.clients.ps.is_chat_message_block;

import tm.clients.ps.is_chat_message;
import tm.clients.ps.message_block;

import containers;

namespace technicalmachine::ps {

export constexpr auto is_chat_message_block(MessageBlock const messages) -> bool {
	return is_chat_message(containers::front(messages));
}

} // namespace technicalmachine::ps
