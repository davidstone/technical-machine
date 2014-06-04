// Which Move is disabled and for how long
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

#include "disable.hpp"

#include <limits>

namespace technicalmachine {

auto Disable::activate(RegularMoveIndex const index_of_disabled_move) -> void {
	m_index_of_disabled_move = index_of_disabled_move;
}

auto Disable::advance_one_turn() -> void {
	if (!m_index_of_disabled_move) {
		return;
	}
	// TODO: update with proper probability actions
	if (m_turns_disabled < std::numeric_limits<TurnCount>::max()) {
		++m_turns_disabled;
	} else {
		*this = Disable{};
	}
}

auto Disable::move_is_disabled(RegularMoveIndex const index_of_move_to_check) const -> bool {
	return m_index_of_disabled_move == index_of_move_to_check;
}

auto operator== (Disable const lhs, Disable const rhs) -> bool {
	return
		lhs.m_turns_disabled == rhs.m_turns_disabled and
		lhs.m_index_of_disabled_move == rhs.m_index_of_disabled_move;
}

auto operator!= (Disable const lhs, Disable const rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
