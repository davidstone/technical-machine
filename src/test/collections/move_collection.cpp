// Test container for moves
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

#include "move_collection.hpp"
#include <iostream>
#include <string>
#include "create_shared_moves.hpp"
#include "invalid_collection.hpp"
#include "../../move.hpp"
#include "../../move_collection.hpp"
#include "../../move_container.hpp"
#include "../../shared_moves.hpp"

namespace technicalmachine {

void move_collection_tests() {
	std::cout << "\tRunning MoveCollection tests.\n";
	constexpr unsigned team_size = 4;
	constexpr unsigned move_additions = 6;
	for (unsigned size = 1; size <= team_size; ++size) {
		unsigned const shared_moves_size = (size > 1) ? size + 1 : 1;
		SharedMoves shared(size);
		MoveCollection c(shared);
		if (c.size() != shared_moves_size)
			throw InvalidCollection("MoveCollection has the wrong number of shared moves. Team size == " + std::to_string(size));
		for (unsigned n = 1; n <= move_additions; ++n) {
			c.add(Move(static_cast<Move::Moves>(n), 0));
			if (c.size() != shared_moves_size + n or c.size() != c.number_of_regular_moves() + shared_moves_size)
				throw InvalidCollection("MoveCollection has the wrong number of moves. Team size == " + std::to_string(size));
		}
		
		std::vector<Move> expected;
		for (unsigned n = 1; n <= move_additions; ++n)
			expected.push_back(Move(static_cast<Move::Moves>(n), 0));
		std::vector<Move> const expected_shared = create_shared_moves(size);
		expected.insert(expected.end(), expected_shared.begin(), expected_shared.end());
		for (unsigned n = 0; n != expected.size(); ++n) {
			if (expected[n] != c(n))
				throw InvalidCollection("Iterating by index does not give correct results. Team size == " + std::to_string(size));
		}
	}
}

}	// namespace technicalmachine
