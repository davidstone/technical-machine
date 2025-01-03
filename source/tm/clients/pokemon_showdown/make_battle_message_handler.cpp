// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.make_battle_message_handler;

import tm.clients.ps.battle_init_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.parsed_request;

namespace technicalmachine::ps {

export auto make_battle_message_handler(
	ParsedRequest const &,
	BattleInitMessage
) -> BattleMessageHandler;

} // namespace technicalmachine::ps
