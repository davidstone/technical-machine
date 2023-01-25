// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.other_move;

import tm.move.category;
import tm.move.known_move;
import tm.move.move_name;

import tm.generation;

import tv;

namespace technicalmachine {

export struct FutureMove {
	bool is_damaging;
};

export struct OtherMove {
	constexpr OtherMove(KnownMove const move):
		m_move(move)
	{
	}
	constexpr OtherMove(FutureMove const move):
		m_move(move)
	{
	}

	constexpr auto is_counterable(Generation const generation) const {
		return tv::visit(m_move, tv::overload(
			[=](KnownMove const move) {
				return generation <= Generation::three and move.name == MoveName::Hidden_Power ? true : is_physical(generation, move);
			},
			[](FutureMove) { return false; }
		));
	}
	constexpr auto is_mirror_coatable(Generation const generation) const {
		return tv::visit(m_move, tv::overload(
			[=](KnownMove const move) {
				return generation <= Generation::three and move.name == MoveName::Hidden_Power ? false : is_special(generation, move);
			},
			[](FutureMove) { return false; }
		));
	}

	constexpr auto future_move_is_damaging() const {
		return tv::visit(m_move, tv::overload(
			[](KnownMove) { return false; },
			[](FutureMove const move) { return move.is_damaging; }
		));
	}

private:
	tv::variant<KnownMove, FutureMove> m_move;
};

} // namespace technicalmachine
