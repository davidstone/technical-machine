// Convert between a switch and the equivalent Pokemon index
// Copyright (C) 2015 David Stone
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

#include <tm/move/is_switch.hpp>

#include <tm/move/moves.hpp>

#include <cassert>

namespace technicalmachine {

auto is_switch(Moves const move) -> bool {
	switch (move) {
		case Moves::Switch0:
		case Moves::Switch1:
		case Moves::Switch2:
		case Moves::Switch3:
		case Moves::Switch4:
		case Moves::Switch5:
			return true;
		default:
			return false;
	}
}

auto to_switch(TeamIndex const replacement) -> Moves {
	return static_cast<Moves>(replacement + bounded::constant<static_cast<std::intmax_t>(Moves::Switch0)>);
}

auto to_replacement(Moves const move) -> TeamIndex {
	assert(is_switch(move));
	return static_cast<TeamIndex>(bounded::integer(move) - bounded::constant<static_cast<std::intmax_t>(Moves::Switch0)>);
}

}	// namespace technicalmachine
