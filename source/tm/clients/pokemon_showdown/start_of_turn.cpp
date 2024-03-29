// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.start_of_turn;

import tm.clients.ps.action_required;

import tm.clients.turn_count;

namespace technicalmachine::ps {

export struct StartOfTurn : ActionRequired {
	TurnCount turn_count;
};

} // namespace technicalmachine::ps
