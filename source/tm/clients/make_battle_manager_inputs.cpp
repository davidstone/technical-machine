// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.make_battle_manager_inputs;

import tm.clients.teams;

import tm.evaluate.evaluate;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
struct BattleManagerInputs {
	Teams<generation> teams;
	Evaluate<generation> evaluate;
};

} // namespace technicalmachine
