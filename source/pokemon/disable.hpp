// Copyright (C) 2018 David Stone
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

#pragma once

#include "../operators.hpp"
#include "../move/max_moves_per_pokemon.hpp"
#include "../move/moves.hpp"

#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Disable {
	constexpr auto activate(Moves const move) {
		m_disabled_move.emplace(move);
	}
	constexpr auto move_is_disabled(Moves const move) const {
		return m_disabled_move == move;
	}

	constexpr auto advance_one_turn() {
		if (!m_disabled_move) {
			return;
		}
		// TODO: update with proper probability actions
		if (m_turns_disabled < std::numeric_limits<TurnCount>::max()) {
			++m_turns_disabled;
		} else {
			*this = Disable{};
		}
	}

	friend constexpr auto compare(Disable const lhs, Disable const rhs) noexcept {
		BOUNDED_COMPARE_ONE_MEMBER(m_turns_disabled);
		return compare(lhs.m_disabled_move, rhs.m_disabled_move);
	}

private:
	// TODO: Use an optional struct?
	bounded::optional<Moves> m_disabled_move = bounded::none;
	using TurnCount = bounded::integer<0, 7>;
	TurnCount m_turns_disabled = 0_bi;
};

}	// namespace technicalmachine
