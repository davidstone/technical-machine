// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.scored_move;

import tm.evaluate.extreme_element_value;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;

import containers;

namespace technicalmachine {

export struct ScoredMove {
	MoveName name;
	double score;
};
export using ScoredMoves = containers::static_vector<ScoredMove, maximum_possible_selections>;

export constexpr auto max_score(ScoredMoves const scored_moves) {
	return max_element_value(containers::transform(scored_moves, &ScoredMove::score));
}
} // namespace technicalmachine
