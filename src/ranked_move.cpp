// Class to order moves to improve alpha-beta pruning
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

#include "ranked_move.hpp"

namespace technicalmachine {

RankedMove::RankedMove(uint8_t const i, uint16_t const s):
	score(s),
	index(i) {
}

bool operator<(RankedMove const & lhs, RankedMove const & rhs) {
	return lhs.score < rhs.score;
}

}	// namespace technicalmachine
