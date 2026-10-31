export module tm.clients.ps.start_of_turn;

import tm.clients.turn_count;

namespace technicalmachine::ps {

export struct StartOfTurn {
	TurnCount turn_count;
	friend auto operator==(StartOfTurn, StartOfTurn) -> bool = default;
};

} // namespace technicalmachine::ps
