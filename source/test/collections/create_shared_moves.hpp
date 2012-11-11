// Basis for some move tests
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

#ifndef TEST__COLLECTIONS__CREATE_SHARED_MOVES_HPP_
#define TEST__COLLECTIONS__CREATE_SHARED_MOVES_HPP_

#include <vector>
#include "../../move/move.hpp"
#include "../../move/moves.hpp"

namespace technicalmachine {

inline std::vector<Moves> create_shared_moves(unsigned const team_size) {
	std::vector<Moves> shared ({ Moves::Struggle });
	if (team_size != 1) {
		for (unsigned n = 0; n != team_size; ++n)
			shared.emplace_back(Move::from_replacement(n));
	}
	return shared;
}

}	// namespace technicalmachine
#endif	// TEST__COLLECTIONS__CREATE_SHARED_MOVES_HPP_
