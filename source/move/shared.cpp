// Handle common moves that all Pokemon select
// Copyright (C) 2016 David Stone
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

#include "is_switch.hpp"
#include "moves.hpp"

#include <containers/common_container_functions.hpp>

namespace technicalmachine {

auto SharedMovesIterator::operator*() const -> value_type {
	using switch_index_type = bounded::integer<
		static_cast<intmax_t>(number_of_weird_moves),
		static_cast<intmax_t>(std::numeric_limits<containers::index_type<SharedMoves>>::max())
	>;
	static_assert(number_of_weird_moves == 1_bi, "Struggle is not the only weird move.");
	return Move((m_index == 0_bi) ?
		Moves::Struggle :
		to_switch(static_cast<switch_index_type>(m_index) - number_of_weird_moves)
	);
}


auto SharedMoves::remove_switch() -> void {
	--m_number_of_switches;
	if (m_number_of_switches == 1_bi) {
		m_number_of_switches = 0_bi;
	}
}

}	// namespace technicalmachine
