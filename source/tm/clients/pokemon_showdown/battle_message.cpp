// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_message;

import tm.clients.ps.battle_init_message;
import tm.clients.ps.event_block;
import tm.clients.ps.parsed_request;

import tv;

namespace technicalmachine::ps {

export struct CreateBattle {
	friend auto operator==(CreateBattle, CreateBattle) -> bool = default;
};

export using BattleMessage = tv::variant<
	CreateBattle,
	ParsedRequest,
	BattleInitMessage,
	EventBlock
>;

} // namespace technicalmachine::ps
