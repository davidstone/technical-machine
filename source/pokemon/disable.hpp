// Which Move is disabled and for how long
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

#pragma once

#include "../hash.hpp"
#include "../operators.hpp"
#include "../move/max_moves_per_pokemon.hpp"

#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Disable {
	auto activate(RegularMoveIndex index_of_disabled_move) -> void;
	auto advance_one_turn() -> void;
	auto move_is_disabled(RegularMoveIndex index_of_move_to_check) const -> bool;
	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_index_of_disabled_move, m_turns_disabled);
	}
	friend auto operator==(Disable lhs, Disable rhs) -> bool;
private:
	bounded::optional<RegularMoveIndex> m_index_of_disabled_move = bounded::none;
	using TurnCount = bounded::integer<0, 7>;
	TurnCount m_turns_disabled = 0_bi;
};

constexpr auto hash(Disable const disable) noexcept {
	return disable.hash();
}

}	// namespace technicalmachine
