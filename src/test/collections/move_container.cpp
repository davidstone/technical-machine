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

#include "move_container.hpp"

#include <functional>
#include <iostream>

#include "create_shared_moves.hpp"
#include "invalid_collection.hpp"

#include "../../move/move.hpp"
#include "../../move/container.hpp"
#include "../../move/shared.hpp"

namespace technicalmachine {
namespace {
class Comparator : public std::unary_function<void, Move> {
	public:
		Comparator(std::vector<Move> const & m):
			moves(m),
			n(0) {
		}
		void operator()(Move const & move) {
			if (moves[n] != move)
				throw InvalidCollection("MoveContainer has the wrong moves.");
			++n;
		}
		std::vector<Move> moves;
		unsigned n;
};

}	// unnamed namespace

void move_container_tests() {
	std::cout << "\tRunning MoveContainer tests.\n";
	constexpr unsigned team_size = 4;
	constexpr unsigned shared_moves_size = team_size + 1;
	constexpr unsigned move_additions = 6;
	SharedMoves shared(team_size);
	MoveContainer c(shared);
	if (c.size() != shared_moves_size)
		throw InvalidCollection("MoveContainer has the wrong number of shared moves.");
	for (unsigned n = 1; n <= move_additions; ++n) {
		c.push_back(Move(static_cast<Move::Moves>(n), 0));
		if (c.size() != shared_moves_size + n or c.size() != c.number_of_regular_moves() + shared_moves_size)
			throw InvalidCollection("MoveContainer has the wrong number of moves.");
	}
	std::vector<Move> expected_regular;
	for (unsigned n = 1; n <= move_additions; ++n)
		expected_regular.push_back(Move(static_cast<Move::Moves>(n), 0));
	c.for_each_regular_move(Comparator(expected_regular));
	std::vector<Move> const expected_shared = create_shared_moves(team_size);
	c.for_each_shared(Comparator(expected_shared));
}

}	// namespace technicalmachine
