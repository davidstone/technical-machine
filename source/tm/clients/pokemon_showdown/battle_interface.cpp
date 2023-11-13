// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_interface;

import tm.clients.ps.battle_message_result;
import tm.clients.ps.in_message;

namespace technicalmachine::ps {

export struct BattleInterface {
	virtual auto handle_message(InMessage message) -> BattleMessageResult = 0;
	virtual ~BattleInterface() = default;
};

} // namespace technicalmachine::ps
