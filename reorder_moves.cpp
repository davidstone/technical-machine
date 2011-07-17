// Reorder moves for efficient evaluation
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>
#include "reorder_moves.h"
#include "move.h"
#include <algorithm>

// algorithm must be included last because it uses a macro to define SING. I use SING elsewhere in my program, so to be safe I must #include <algorithm> after all other includes.

namespace technicalmachine {

void reorder (std::vector<Move> const &input, std::vector <std::pair <int64_t, size_t> > output) {
	for (size_t n = 0; n != input.size(); ++n) {
		std::pair <int64_t, size_t> entry;
		entry.first = input [n].score;
		entry.second = n;
		output.push_back (entry);
	}
	std::sort (output.begin(), output.end());
}

}
