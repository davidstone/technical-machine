// Test container for moves
// Copyright (C) 2014 David Stone
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

#include "../../move/collection.hpp"
#include "../../move/container.hpp"

#include "../../string_conversions/move.hpp"

namespace technicalmachine {
using namespace bounded_integer::literal;

void move_collection_tests() {
	using bounded_integer::to_string;
	std::cout << "\tRunning MoveCollection tests.\n";
	constexpr TeamSize team_size = 4_bi;
	for (auto const & size : bounded_integer::range(1_bi, team_size + 1_bi)) {
		auto const shared_moves_size = BOUNDED_INTEGER_CONDITIONAL(size > 1_bi, size + 1_bi, 1_bi);
		MoveCollection collection(size);
		if (collection.size() != shared_moves_size)
			throw InvalidCollection("MoveCollection has the wrong number of shared moves. Team size == " + to_string(size));
		auto expected = create_regular_moves();
		using ExpectedSize = bounded_integer::checked_integer<0, 100>;
		for (auto const & n : bounded_integer::range(static_cast<ExpectedSize>(expected.size()))) {
			collection.add(expected[static_cast<std::size_t>(n)]);
			if (collection.size() != shared_moves_size + n + 1_bi or collection.size() != collection.number_of_regular_moves() + shared_moves_size) {
				throw InvalidCollection("MoveCollection has the wrong number of moves. Team size == " + to_string(size));
			}
		}
		
		auto const expected_shared = create_shared_moves(size);
		expected.insert(expected.end(), expected_shared.begin(), expected_shared.end());
		for (auto const & n : bounded_integer::range(static_cast<ExpectedSize>(expected.size()))) {
			if (expected[static_cast<std::size_t>(n)] != collection(static_cast<unsigned>(n))) {
				throw InvalidCollection("Iterating by index does not give correct results. Team size == " + to_string(size) + ". Stored: " + to_string(collection(static_cast<unsigned>(n))) + " -- Expected: " + to_string(expected[static_cast<unsigned>(n)]));
			}
		}
	}
}

}	// namespace technicalmachine
