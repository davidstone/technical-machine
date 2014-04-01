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

#include "move_container.hpp"

#include <iostream>
#include <string>

#include "create_regular_moves.hpp"
#include "create_shared_moves.hpp"
#include "invalid_collection.hpp"

#include "../../move/moves_forward.hpp"
#include "../../move/container.hpp"

#include "../../string_conversions/move.hpp"

namespace technicalmachine {
namespace {
using namespace bounded::literal;

class Comparator {
public:
	Comparator(std::vector<Moves> const & m, std::string && str_type):
		moves(m),
		type(std::move(str_type)),
		n(0) {
	}
	void operator()(Moves const move) {
		if (moves[n] != move) {
			throw InvalidCollection("MoveContainer has the wrong " + type + " moves. Expected: " + to_string(moves[n]) + " but got " + to_string(move));
		}
		++n;
	}
private:
	std::vector<Moves> moves;
	std::string type;
	unsigned n;
};

}	// namespace

void move_container_tests() {
	std::cout << "\tRunning MoveContainer tests.\n";
	constexpr TeamSize team_size(4_bi);
	constexpr auto shared_moves_size = team_size + 1_bi;
	MoveContainer c(team_size);
	if (c.size() != shared_moves_size) {
		throw InvalidCollection("MoveContainer has the wrong number of shared moves. Expecting " + bounded::to_string(shared_moves_size) + " but got " + bounded::to_string(c.size()));
	}
	auto const moves = create_regular_moves();
	for (auto const n : bounded::range(static_cast<bounded::checked_integer<0, 100>>(moves.size()))) {
		c.emplace_back(moves[static_cast<std::size_t>(n)]);
		if (c.size() != shared_moves_size + n + 1_bi or c.size() != static_cast<bounded::checked_integer<0, 100>>(c.number_of_regular_moves()) + shared_moves_size) {
			throw InvalidCollection("MoveContainer has the wrong number of moves during addition of moves.");
		}
	}
	c.for_each_regular_move(Comparator(moves, "regular"));
	c.for_each_shared(Comparator(create_shared_moves(team_size), "shared"));
}

}	// namespace technicalmachine
