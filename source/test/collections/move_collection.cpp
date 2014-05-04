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

#include "create_regular_moves.hpp"
#include "create_shared_moves.hpp"
#include "invalid_collection.hpp"

#include "../../move/collection.hpp"
#include "../../move/container.hpp"

#include "../../string_conversions/move.hpp"

#include <bounded_integer/integer_range.hpp>

#include <iostream>
#include <string>

namespace technicalmachine {
using namespace bounded::literal;

void move_collection_tests() {
	using bounded::to_string;
	std::cout << "\tRunning MoveCollection tests.\n";
	constexpr TeamSize team_size = 4_bi;
	for (auto const & test_size : bounded::integer_range(1_bi, team_size + 1_bi)) {
		auto const shared_moves_size = BOUNDED_INTEGER_CONDITIONAL(test_size > 1_bi, test_size + 1_bi, 1_bi);
		MoveCollection collection(test_size);
		if (collection.size() != shared_moves_size)
			throw InvalidCollection("MoveCollection has the wrong number of shared moves. Team size == " + to_string(test_size));
		auto expected = create_regular_moves();
		for (auto const & n : bounded::integer_range(static_cast<MoveSize>(expected.size()))) {
			collection.add(expected[static_cast<std::size_t>(n)]);
			if (collection.size() != shared_moves_size + n + 1_bi or collection.size() != static_cast<RegularMoveSize>(size(collection.regular())) + shared_moves_size) {
				throw InvalidCollection("MoveCollection has the wrong number of moves. Team size == " + to_string(test_size));
			}
		}
		
		auto const expected_shared = create_shared_moves(test_size);
		expected.insert(expected.end(), expected_shared.begin(), expected_shared.end());
		for (auto const & n : bounded::integer_range(static_cast<MoveSize>(expected.size()))) {
			if (expected[static_cast<std::size_t>(n)] != collection(n)) {
				throw InvalidCollection("Iterating by index does not give correct results. Team size == " + to_string(test_size) + ". Stored: " + to_string(collection(n)) + " -- Expected: " + to_string(expected[static_cast<unsigned>(n)]));
			}
		}
	}
}

}	// namespace technicalmachine
