// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.move;

import tm.move.move_name;
import tm.move.pp;

import tm.compress;
import tm.generation;

import bounded;
import numeric_traits;

namespace technicalmachine {
using namespace bounded::literal;

export struct Move {
	explicit constexpr Move(Generation const generation, MoveName const move, PP::pp_ups_type const pp_ups = numeric_traits::max_value<PP::pp_ups_type>) :
		m_name(move),
		m_pp(generation, move, pp_ups)
	{
	}

	constexpr auto name() const {
		return m_name;
	}

	constexpr auto pp() const {
		return m_pp;
	}

	constexpr auto reduce_pp(auto const value) & {
		m_pp.reduce(value);
	}

	constexpr auto restore_pp(auto const value) & {
		m_pp.restore(value);
	}

	friend auto operator==(Move, Move) -> bool = default;
	friend constexpr auto operator==(Move const lhs, MoveName const rhs) -> bool {
		return lhs.name() == rhs;
	}
	// Pokemon don't change their moves throughout a battle, so we don't need
	// to include move name
	friend constexpr auto compress(Move const value) {
		return compress(value.m_pp);
	}

private:
	constexpr explicit Move(bounded::tombstone_tag, auto const make) noexcept:
		m_name(),
		m_pp(make())
	{
	}

	MoveName m_name;
	PP m_pp;

	friend bounded::tombstone<Move>;
	friend bounded::tombstone_member<&Move::m_pp>;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone<technicalmachine::Move> : bounded::tombstone_member<&technicalmachine::Move::m_pp> {
};
