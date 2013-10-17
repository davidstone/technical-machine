// Handle common moves that all Pokemon select
// Copyright (C) 2013 David Stone
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

#include "global_move.hpp"
#include "move.hpp"
#include "moves.hpp"

namespace technicalmachine {
namespace {
// Struggle is the only other special move right now
constexpr unsigned number_of_non_switches = 1;
}

// Once a Pokemon is the last on the team, we remove switching entirely. This
// takes place if we construct SharedMoves from a one-Pokemon team or if we call
// remove_switch() and it brings a Pokemon down to only the ability to switch to
// itself

SharedMoves::SharedMoves(unsigned const team_size):
	number_of_switches((team_size > 1) ? team_size : 0) {
}

void SharedMoves::remove_switch() {
	assert(number_of_switches != 0);
	--number_of_switches;
	if (number_of_switches == 1)
		number_of_switches = 0;
}

Move const & SharedMoves::operator[](size_t const index) const {
	assert(index <= number_of_switches + number_of_non_switches);
	auto const name = (index == 0) ? Moves::Struggle : from_replacement(index - number_of_non_switches);
	return global_move(name);
}

size_t SharedMoves::size() const {
	return number_of_switches + number_of_non_switches;
}

bool operator==(SharedMoves const & lhs, SharedMoves const & rhs) {
	return lhs.number_of_switches == rhs.number_of_switches;
}

}	// namespace technicalmachine
