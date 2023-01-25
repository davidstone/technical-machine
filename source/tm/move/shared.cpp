// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <operators/bracket.hpp>
#include <bounded/conditional.hpp>

export module tm.move.shared;

import tm.move.move;
import tm.move.move_name;
import tm.move.is_switch;

import tm.pokemon.max_pokemon_per_team;

import tm.generation;
export import tm.operators;

import bounded;
import containers;
import numeric_traits;
import operators;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto number_of_weird_moves = 2_bi; // Pass, Struggle
using SharedMoveSize = decltype(bounded::declval<TeamSize>() + number_of_weird_moves);

template<Generation generation>
struct SharedMovesIterator {
private:
	using underlying_index_type = bounded::integer<
		0,
		bounded::normalize<numeric_traits::max_value<SharedMoveSize>>
	>;
public:
	using difference_type = std::common_type_t<decltype(-bounded::declval<SharedMoveSize>()), SharedMoveSize>;

	constexpr explicit SharedMovesIterator(underlying_index_type const other):
		m_index(other)
	{
	}

	auto operator*() const -> Move {
		using switch_index_type = bounded::integer<
			bounded::normalize<number_of_weird_moves>,
			bounded::normalize<numeric_traits::max_value<SharedMoveSize> - 1_bi>
		>;
		static_assert(number_of_weird_moves == 2_bi, "Add the extra 'weird' move here.");
		auto const move_name = 
			(m_index == 0_bi) ? MoveName::Pass :
			(m_index == 1_bi) ? MoveName::Struggle :
			to_switch(::bounded::assume_in_range<switch_index_type>(m_index) - number_of_weird_moves);
		return Move(generation, move_name);
	}

	friend constexpr auto operator+(SharedMovesIterator const lhs, difference_type const rhs) {
		return SharedMovesIterator(::bounded::assume_in_range<underlying_index_type>(lhs.m_index + rhs));
	}
	friend constexpr auto operator-(SharedMovesIterator const lhs, SharedMovesIterator const rhs) {
		return lhs.m_index - rhs.m_index;
	}

	friend constexpr auto operator<=>(SharedMovesIterator, SharedMovesIterator) = default;

private:
	underlying_index_type m_index;
};

export template<Generation generation>
struct SharedMoves {
	constexpr explicit SharedMoves(TeamSize const team_size):
		m_number_of_switches(BOUNDED_CONDITIONAL(team_size > 1_bi, team_size, 0_bi))
	{
	}

	static constexpr auto begin() {
		return SharedMovesIterator<generation>(0_bi);
	}
	constexpr auto end() const {
		return SharedMovesIterator<generation>(m_number_of_switches + number_of_weird_moves);
	}

	constexpr void remove_switch() {
		--m_number_of_switches;
		if (m_number_of_switches == 1_bi) {
			m_number_of_switches = 0_bi;
		}
	}

	OPERATORS_BRACKET_SEQUENCE_RANGE_DEFINITIONS

	friend auto operator==(SharedMoves, SharedMoves) -> bool = default;

private:
	TeamSize m_number_of_switches;
};

} // namespace technicalmachine
