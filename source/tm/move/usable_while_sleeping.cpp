export module tm.move.usable_while_sleeping;

import tm.move.move_name;

namespace technicalmachine {

export constexpr bool usable_while_sleeping(MoveName const move) {
	switch (move) {
		case MoveName::Sleep_Talk:
		case MoveName::Snore:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
