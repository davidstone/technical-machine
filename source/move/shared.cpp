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
#include "move.hpp"
#include "moves.hpp"

#include "../pokemon/collection.hpp"

namespace technicalmachine {
namespace {
}

// Once a Pokemon is the last on the team, we remove switching entirely. This
// takes place if we construct SharedMoves from a one-Pokemon team or if we call
// remove_switch() and it brings a Pokemon down to only the ability to switch to
// itself

SharedMoves::SharedMoves(TeamSize const team_size):
	m_number_of_switches(BOUNDED_INTEGER_CONDITIONAL(team_size > 1_bi, team_size, 0_bi)) {
}

void SharedMoves::remove_switch() {
	--m_number_of_switches;
	if (m_number_of_switches == 1_bi) {
		m_number_of_switches = 0_bi;
	}
}

Move const & SharedMoves::operator[](index_type const index) const {
	using switch_index_type = bounded_integer::checked_integer<
		static_cast<intmax_t>(number_of_weird_moves),
		static_cast<intmax_t>(std::numeric_limits<index_type>::max())
	>;
	auto const name = (index == 0_bi) ? Moves::Struggle : from_replacement(static_cast<switch_index_type>(index) - number_of_weird_moves);
	return global_move(name);
}

auto SharedMoves::size() const -> size_type {
	return m_number_of_switches + number_of_weird_moves;
}

bool operator==(SharedMoves const & lhs, SharedMoves const & rhs) {
	return lhs.m_number_of_switches == rhs.m_number_of_switches;
}

}	// namespace technicalmachine
