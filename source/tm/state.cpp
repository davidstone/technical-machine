export module tm.state;

import tm.environment;
import tm.generation;
import tm.team;

namespace technicalmachine {

export template<Generation generation>
struct State {
	Team<generation> ai;
	Team<generation> foe;
	Environment environment;

	friend auto operator==(State const &, State const &) -> bool = default;
};

export template<Generation generation>
struct VisibleState {
	KnownTeam<generation> ai;
	SeenTeam<generation> foe;
	Environment environment;
	
	friend auto operator==(VisibleState const &, VisibleState const &) -> bool = default;
};

} // namespace technicalmachine
