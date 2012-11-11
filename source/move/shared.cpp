// Handle common moves that all Pokemon select
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

#include "shared.hpp"

#include <cassert>

#include "moves.hpp"

namespace technicalmachine {

SharedMoves::SharedMoves(unsigned const team_size) {
	moves.reserve ((team_size > 1) ? team_size + 1 : 1);
	moves.emplace_back(Moves::Struggle, 0);
	// A Pokemon has a new "Switch" move for each Pokemon in the party.
	if (team_size > 1) {
		for (unsigned count = 0; count != team_size; ++count) {
			moves.emplace_back(Move::from_replacement(count), 0);
		}
	}
}

void SharedMoves::remove_switch() {
	assert(moves.back().is_switch());
	moves.pop_back();
	if (moves.back().name == Moves::Switch0)
		moves.pop_back();
}

Move const & SharedMoves::operator[](size_t const index) const {
	assert(index < moves.size());
	return moves[index];
}

Move & SharedMoves::operator[](size_t const index) {
	assert(index < moves.size());
	return moves[index];
}

size_t SharedMoves::size() const {
	return moves.size();
}

}	// namespace technicalmachine
