// Handle common moves that all Pokemon can select
// Copyright (C) 2016 David Stone
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

#include "max_moves_per_pokemon.hpp"
#include "move.hpp"

#include "../operators.hpp"

#include <bounded/integer.hpp>

#include <containers/size.hpp>

namespace technicalmachine {
using namespace bounded::literal;

BOUNDED_COMPARISON
BOUNDED_COMMON_ARITHMETIC

struct SharedMovesIterator {
	using value_type = Move const;
	using difference_type = containers::basic_difference_type<SharedMoveSize>;
	using pointer = value_type *;
	using reference = value_type;
	using iterator_category = std::random_access_iterator_tag;

	auto operator*() const -> value_type;

	friend constexpr auto operator+(SharedMovesIterator const lhs, difference_type const rhs) {
		return SharedMovesIterator(static_cast<underlying_index_type>(lhs.m_index + rhs));
	}
	friend constexpr auto operator-(SharedMovesIterator const lhs, SharedMovesIterator const rhs) {
		return lhs.m_index - rhs.m_index;
	}

	friend constexpr auto compare(SharedMovesIterator const lhs, SharedMovesIterator const rhs) noexcept {
		return bounded::compare(lhs.m_index, rhs.m_index);
	}

	CONTAINERS_OPERATOR_BRACKET_DEFINITIONS(SharedMovesIterator)
private:
	using underlying_index_type = bounded::integer<0, static_cast<intmax_t>(std::numeric_limits<SharedMoveSize>::max())>;
	friend struct SharedMoves;
	constexpr explicit SharedMovesIterator(underlying_index_type const other) noexcept:
		m_index(other) {
	}

	underlying_index_type m_index;
};


struct SharedMoves {
	using size_type = SharedMoveSize;
	using const_iterator = SharedMovesIterator;

	constexpr explicit SharedMoves(TeamSize const team_size = max_pokemon_per_team):
		m_number_of_switches(BOUNDED_CONDITIONAL(team_size > 1_bi, team_size, 0_bi)) {
	}

	friend constexpr auto begin(SharedMoves) {
		return const_iterator(0_bi);
	}
	friend constexpr auto end(SharedMoves const container) {
		return const_iterator(container.m_number_of_switches + number_of_weird_moves);
	}

	auto remove_switch() -> void;

	CONTAINERS_OPERATOR_BRACKET_DEFINITIONS(SharedMoves)
private:
	TeamSize m_number_of_switches;
};

constexpr auto compare(SharedMoves const lhs, SharedMoves const rhs) {
	return bounded::compare(containers::size(lhs), containers::size(rhs));
}


}	// namespace technicalmachine
