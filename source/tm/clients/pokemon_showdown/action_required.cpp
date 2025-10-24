// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.action_required;

import tm.clients.ps.slot_memory;

import tm.generation_generic;
import tm.visible_state;

namespace technicalmachine::ps {

export struct ActionRequired {
	GenerationGeneric<VisibleState> state;
	SlotMemory slot_memory;
};

} // namespace technicalmachine::ps
