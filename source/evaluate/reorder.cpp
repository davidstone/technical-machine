// Reorder moves for efficient evaluation
// Copyright (C) 2015 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "reorder.hpp"

#include "move_scores.hpp"

#include "../block.hpp"
#include "../move/moves.hpp"

#include <algorithm>

namespace technicalmachine {

StaticVectorMoves reorder(LegalSelections const & input, MoveScores const & move_scores, bool ai) {
	// This takes all of a Pokemon's moves and sorts them based on previously
	// evaluated scores. Moves that haven't been evaluated are sorted to the
	// end. I do this because alpha-beta pruning is most efficient when the
	// best moves are searched first. The reason for this is that the search
	// then only has to prove that further moves aren't as good as the move
	// already searched; it is not important to know how much worse they are.
	// Moves that cannot be selected are excluded.
	StaticVectorMoves output(input.begin(), input.end());
	auto compare = [&](auto const & lhs, auto const & rhs) {
		auto const lhs_score = move_scores.get(lhs);
		auto const rhs_score = move_scores.get(rhs);
		return ai ? (lhs_score > rhs_score) : (lhs_score < rhs_score);
	};
	std::sort(output.begin(), output.end(), compare);
	return output;
}

}	// namespace technicalmachine
