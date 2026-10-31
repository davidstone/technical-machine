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