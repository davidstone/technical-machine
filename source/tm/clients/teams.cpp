// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
