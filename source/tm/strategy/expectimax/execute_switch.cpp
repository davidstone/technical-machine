// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.expectimax.execute_switch;

import tm.evaluate.depth;
import tm.evaluate.score;
import tm.evaluate.selector;
import tm.evaluate.win;

import tm.move.switch_;

import tm.generation;
import tm.state;

namespace technicalmachine {
	
export template<Generation generation>
auto execute_switch(State<generation> state, Selector const select, Switch const switch_, Depth const depth, auto const continuation) -> Score {
	auto const selected = select(state);
	selected.selection.switch_pokemon(
		selected.other.pokemon(),
		state.environment,
		switch_.value()
	);
	if (auto const won = win(state.ai, state.foe)) {
		return *won + Score(double(depth.remaining_general()));
	}
	return continuation(state);
}

} // namespace technicalmachine
