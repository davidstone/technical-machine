// Handle common moves that all Pokemon select
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

#include "shared.hpp"

#include "global_move.hpp"
#include "is_switch.hpp"
#include "moves.hpp"

namespace technicalmachine {
namespace {
}

auto SharedMovesIterator::operator*() const -> reference {
	using switch_index_type = bounded::integer<
		static_cast<intmax_t>(number_of_weird_moves),
		static_cast<intmax_t>(std::numeric_limits<index_type>::max())
	>;
	static_assert(number_of_weird_moves == 1_bi, "Struggle is not the only weird move.");
	return global_move((m_index == 0_bi) ?
		Moves::Struggle :
		to_switch(static_cast<switch_index_type>(m_index) - number_of_weird_moves)
	);
}


// Once a Pokemon is the last on the team, we remove switching entirely. This
// takes place if we construct SharedMoves from a one-Pokemon team or if we call
// remove_switch() and it brings a Pokemon down to only the ability to switch to
// itself

SharedMoves::SharedMoves(TeamSize const team_size):
	m_number_of_switches(BOUNDED_CONDITIONAL(team_size > 1_bi, team_size, 0_bi)) {
}

auto SharedMoves::begin() const -> const_iterator {
	return const_iterator(0_bi);
}
auto SharedMoves::end() const -> const_iterator {
	return const_iterator(m_number_of_switches + number_of_weird_moves);
}

auto SharedMoves::operator[](index_type const index) const -> Move const & {
	return *const_iterator(index);
}

auto SharedMoves::remove_switch() -> void {
	--m_number_of_switches;
	if (m_number_of_switches == 1_bi) {
		m_number_of_switches = 0_bi;
	}
}

auto operator==(SharedMoves const & lhs, SharedMoves const & rhs) -> bool {
	return size(lhs) == size(rhs);
}

}	// namespace technicalmachine
