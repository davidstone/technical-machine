// Handle common moves that all Pokemon can select
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/is_switch.hpp>

#include <tm/operators.hpp>

#include <bounded/integer.hpp>

#include <containers/begin_end.hpp>

#include <operators/operators.hpp>

namespace technicalmachine {
using namespace bounded::literal;

BOUNDED_COMMON_ARITHMETIC

template<Generation generation>
struct SharedMovesIterator {
private:
	using underlying_index_type = bounded::integer<
		0,
		static_cast<int>(bounded::max_value<SharedMoveSize>)
	>;
public:
	using value_type = Move const;
	using difference_type = containers::basic_difference_type<SharedMoveSize>;
	using pointer = value_type *;
	using reference = value_type;
	using iterator_category = std::random_access_iterator_tag;

	constexpr explicit SharedMovesIterator(underlying_index_type const other):
		m_index(other)
	{
	}

	auto operator*() const -> value_type {
		using switch_index_type = bounded::integer<
			static_cast<int>(number_of_weird_moves),
			static_cast<int>(bounded::max_value<SharedMoveSize>) - 1
		>;
		static_assert(number_of_weird_moves == 2_bi, "Add the extra 'weird' move here.");
		auto const move_name = 
			(m_index == 0_bi) ? Moves::Pass :
			(m_index == 1_bi) ? Moves::Struggle :
			to_switch(static_cast<switch_index_type>(m_index) - number_of_weird_moves);
		return Move(generation, move_name);
	}

	friend constexpr auto operator+(SharedMovesIterator const lhs, difference_type const rhs) {
		return SharedMovesIterator(static_cast<underlying_index_type>(lhs.m_index + rhs));
	}
	friend constexpr auto operator-(SharedMovesIterator const lhs, SharedMovesIterator const rhs) {
		return lhs.m_index - rhs.m_index;
	}

	friend constexpr auto operator<=>(SharedMovesIterator const &, SharedMovesIterator const &) = default;

private:
	underlying_index_type m_index;
};

template<Generation generation>
struct SharedMoves {
	using size_type = SharedMoveSize;
	using const_iterator = SharedMovesIterator<generation>;

	constexpr explicit SharedMoves(TeamSize const team_size):
		m_number_of_switches(BOUNDED_CONDITIONAL(team_size > 1_bi, team_size, 0_bi))
	{
	}

	static constexpr auto begin() {
		return const_iterator(0_bi);
	}
	constexpr auto end() const {
		return const_iterator(m_number_of_switches + number_of_weird_moves);
	}

	constexpr void remove_switch() {
		--m_number_of_switches;
		if (m_number_of_switches == 1_bi) {
			m_number_of_switches = 0_bi;
		}
	}

	OPERATORS_BRACKET_SEQUENCE_RANGE_DEFINITIONS

	friend auto operator==(SharedMoves const &, SharedMoves const &) -> bool = default;
private:
	TeamSize m_number_of_switches;
};

}	// namespace technicalmachine
