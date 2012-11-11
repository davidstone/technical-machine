// Reorder moves for efficient evaluation
// Copyright (C) 2012 David Stone
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

#include <algorithm>
#include <vector>

#include "move.hpp"

namespace technicalmachine {

std::vector<RankedMove> reorder(std::vector<Move> const & input, bool ai) {
	// This takes all of a Pokemon's moves and sorts them based on previously
	// evaluated scores. Moves that haven't been evaluated are sorted to the
	// end. I do this because alpha-beta pruning is most efficient when the
	// best moves are searched first. The reason for this is that the search
	// then only has to prove that further moves aren't as good as the move
	// already searched; it is not important to know how much worse they are.
	// Moves that cannot be selected are excluded.
	std::vector<RankedMove> output;
	for (auto const & move : input) {
		if (move.selectable())
			output.emplace_back(move.name, move.score);
	}
	std::sort(output.begin(), output.end());
	if (ai)
		std::reverse(output.begin(), output.end());
	return output;
}

}	// namespace technicalmachine
