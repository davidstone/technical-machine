// Copyright (C) 2019 David Stone
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

#include <tm/move/move.hpp>

#include <bounded/detail/variant/variant.hpp>
#include <bounded/detail/overload.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct FutureMove {
	bool is_damaging;
};

struct OtherMove {
	constexpr OtherMove(Moves const used_move):
		m_move(used_move)
	{
	}
	constexpr OtherMove(FutureMove const future_move):
		m_move(future_move)
	{
	}

	constexpr auto used_move_is_physical() const {
		return bounded::visit(m_move, bounded::overload(
			[&](Moves const used) { return is_physical(used); },
			[](FutureMove) { return false; }
		));
	}
	constexpr auto used_move_is_special() const {
		return bounded::visit(m_move, bounded::overload(
			[&](Moves const used) { return is_special(used); },
			[](FutureMove) { return false; }
		));
	}

	constexpr auto future_move_is_damaging() const {
		return bounded::visit(m_move, bounded::overload(
			[](Moves) { return false; },
			[](FutureMove const move) { return move.is_damaging; }
		));
	}
private:
	bounded::variant<Moves, FutureMove> m_move;
};

}	// namespace technicalmachine
