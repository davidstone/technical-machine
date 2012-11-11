// Hold all moves for easy use by certain moves that call other moves
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

#include "global_move.hpp"
#include <cassert>
#include <vector>
#include "move.hpp"
#include "moves.hpp"

namespace technicalmachine {
namespace {

std::vector<Move> all_moves() {
	std::vector<Move> all;
	all.reserve(number_of_moves);
	for (size_t n = 0; n != number_of_moves; ++n) {
		all.emplace_back(static_cast<Moves>(n));
	}
	return all;
}

}	// unnamed namespace

Move const & global_move(Moves const name) {
	static auto const all = all_moves();
	assert(name != Moves::END);
	return all[static_cast<size_t>(name)];
}

}	// namespace technicalmachine
