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

#include "create_regular_moves.hpp"
#include "create_shared_moves.hpp"
#include "invalid_collection.hpp"

#include "../../move/move.hpp"
#include "../../move/collection.hpp"
#include "../../move/container.hpp"

#include "../../string_conversions/conversion.hpp"

namespace technicalmachine {

void move_collection_tests() {
	std::cout << "\tRunning MoveCollection tests.\n";
	constexpr unsigned team_size = 4;
	for (unsigned size = 1; size <= team_size; ++size) {
		unsigned const shared_moves_size = (size > 1) ? size + 1 : 1;
		MoveCollection c(size);
		if (c.size() != shared_moves_size)
			throw InvalidCollection("MoveCollection has the wrong number of shared moves. Team size == " + std::to_string(size));
		auto expected = create_regular_moves();
		for (unsigned n = 0; n != expected.size(); ++n) {
			c.add(expected[n]);
			if (c.size() != shared_moves_size + n + 1 or c.size() != c.number_of_regular_moves() + shared_moves_size) {
				throw InvalidCollection("MoveCollection has the wrong number of moves. Team size == " + std::to_string(size));
			}
		}
		
		auto const expected_shared = create_shared_moves(size);
		expected.insert(expected.end(), expected_shared.begin(), expected_shared.end());
		for (unsigned n = 0; n != expected.size(); ++n) {
			if (expected[n] != c(n).name) {
				throw InvalidCollection("Iterating by index does not give correct results. Team size == " + std::to_string(size) + ". Stored: " + c(n).to_string() + " -- Expected: " + to_string(expected[n]));
			}
		}
	}
}

}	// namespace technicalmachine
