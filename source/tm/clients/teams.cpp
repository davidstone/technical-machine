export module tm.clients.teams;

import tm.generation;
import tm.team;

namespace technicalmachine {

export template<Generation generation>
struct Teams {
	KnownTeam<generation> ai;
	SeenTeam<generation> foe;
};

} // namespace technicalmachine
