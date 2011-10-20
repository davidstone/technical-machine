// Reorder moves for efficient evaluation
// Copyright (C) 2011 David Stone
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

#include "reorder_moves.h"

#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <utility>
#include <vector>

#include "move.h"

namespace technicalmachine {

// reorder (...) takes all of a Pokemon's moves and sorts them based on previously evaluated scores. Moves that haven't been evaluated are sorted to the end. I do this because alpha-beta pruning is most efficient when the best moves are searched first. The reason for this is that the search then only has to prove that further moves aren't as good as the move already searched; it is not important to know how much worse they are.

void reorder (std::vector<Move> const &input, std::vector <std::pair <int64_t, size_t> > &output, bool ai) {
	for (size_t n = 0; n != input.size(); ++n) {
		std::pair <int64_t, size_t> entry;
		entry.first = input [n].score;
		entry.second = n;
		output.push_back (entry);
	}
	std::sort (output.begin(), output.end());
	if (ai)
		std::reverse (output.begin(), output.end());
}

}
