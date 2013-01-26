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

#ifndef EVALUATE__RANKED_HPP_
#define EVALUATE__RANKED_HPP_

#include <cstdint>
#include "../move/moves_forward.hpp"

namespace technicalmachine {

class RankedMove {
	public:
		explicit RankedMove(Moves set_move, int16_t set_score);
		Moves name() const;
		friend bool operator<(RankedMove const & lhs, RankedMove const & rhs);
		friend bool operator>(RankedMove const & lhs, RankedMove const & rhs);
	private:
		int16_t score;
		Moves move;
};

}	// namespace technicalmachine
#endif	// EVALUATE__RANKED_HPP_
