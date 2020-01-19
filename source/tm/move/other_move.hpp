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

#include <tm/move/category.hpp>
#include <tm/move/known_move.hpp>
#include <tm/move/move.hpp>
#include <tm/type/type.hpp>

#include <bounded/detail/variant/variant.hpp>
#include <bounded/detail/overload.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct FutureMove {
	bool is_damaging;
};

struct OtherMove {
	constexpr OtherMove(KnownMove const move):
		m_move(move)
	{
	}
	constexpr OtherMove(FutureMove const move):
		m_move(move)
	{
	}

	constexpr auto used_move_is_physical(Generation const generation) const {
		return bounded::visit(m_move, bounded::overload(
			[=](KnownMove const move) { return is_physical(generation, move); },
			[](FutureMove) { return false; }
		));
	}
	constexpr auto used_move_is_special(Generation const generation) const {
		return bounded::visit(m_move, bounded::overload(
			[=](KnownMove const move) { return is_special(generation, move); },
			[](FutureMove) { return false; }
		));
	}

	constexpr auto future_move_is_damaging() const {
		return bounded::visit(m_move, bounded::overload(
			[](KnownMove) { return false; },
			[](FutureMove const move) { return move.is_damaging; }
		));
	}
private:
	bounded::variant<KnownMove, FutureMove> m_move;
};

}	// namespace technicalmachine
