export module tm.status.toxic_resets_on_switch;

import tm.generation;

namespace technicalmachine {

export constexpr auto toxic_resets_on_switch(Generation const generation) -> bool {
	switch (generation) {
		case Generation::one:
		case Generation::two:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
