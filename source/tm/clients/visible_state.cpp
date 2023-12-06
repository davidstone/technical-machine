// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.visible_state;

import tm.environment;
import tm.generation;
import tm.team;

namespace technicalmachine {

export template<Generation generation>
struct VisibleState {
	KnownTeam<generation> ai;
	SeenTeam<generation> foe;
	Environment environment;
};

} // namespace technicalmachine
