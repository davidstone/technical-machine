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

#include <tm/compress.hpp>
#include <tm/operators.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Disable {
	auto activate(Moves const move) {
		if (is_regular(move)) {
			insert(m_disabled_move, move);
		}
	}
	constexpr auto move_is_disabled(Moves const move) const {
		return m_disabled_move == move;
	}

	constexpr auto advance_one_turn() {
		if (!m_disabled_move) {
			return;
		}
		// TODO: update with proper probability actions
		if (m_turns_disabled < bounded::max_value<TurnCount>) {
			++m_turns_disabled;
		} else {
			*this = Disable{};
		}
	}

	friend auto operator==(Disable const &, Disable const &) -> bool = default;
	friend constexpr auto compress(Disable const value) {
		return compress_combine(value.m_disabled_move, value.m_turns_disabled);
	}
private:
	// TODO: Use an optional struct?
	bounded::optional<Moves> m_disabled_move = bounded::none;
	using TurnCount = bounded::integer<0, 7>;
	TurnCount m_turns_disabled = 0_bi;
};

}	// namespace technicalmachine
