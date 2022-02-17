// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/category.hpp>
#include <tm/move/known_move.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
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

	constexpr auto is_counterable(Generation const generation) const {
		return bounded::visit(m_move, bounded::overload(
			[=](KnownMove const move) {
				return generation <= Generation::three and move.name == Moves::Hidden_Power ? true : is_physical(generation, move);
			},
			[](FutureMove) { return false; }
		));
	}
	constexpr auto is_mirror_coatable(Generation const generation) const {
		return bounded::visit(m_move, bounded::overload(
			[=](KnownMove const move) {
				return generation <= Generation::three and move.name == Moves::Hidden_Power ? false : is_special(generation, move);
			},
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

} // namespace technicalmachine
