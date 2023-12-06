// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.move_state;

import tm.clients.ps.optional_hp_and_status;

import tm.clients.party;

import tm.move.move_result;

namespace technicalmachine::ps {

export struct MoveState {
	MoveResult move;
	OptionalHPAndStatus user;
	OptionalHPAndStatus other;
	Party party;
	bool user_status_was_cleared;
};

} // namespace technicalmachine::ps