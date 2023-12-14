// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_init_message;

import tm.clients.ps.message_block;

namespace technicalmachine::ps {

export struct BattleInitMessage {
	constexpr explicit BattleInitMessage(MessageBlock messages):
		m_messages(messages)
	{
	}
	constexpr auto messages() const -> MessageBlock {
		return m_messages;
	}
private:
	MessageBlock m_messages;
};

} // namespace technicalmachine::ps
