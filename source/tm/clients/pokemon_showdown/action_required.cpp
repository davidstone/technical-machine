export module tm.clients.ps.action_required;

import tm.clients.ps.slot_memory;

import tm.generation_generic;
import tm.state;

namespace technicalmachine::ps {

export struct ActionRequired {
	GenerationGeneric<VisibleState> state;
	SlotMemory slot_memory;
};

} // namespace technicalmachine::ps
