// Handle common moves that all Pokemon can select
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

#include "max_moves_per_pokemon.hpp"
#include "move.hpp"

#include "../operators.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct SharedMovesIterator {
private:
	static constexpr auto max_size = static_cast<intmax_t>(std::numeric_limits<SharedMoveSize>::max());
public:
	using value_type = Move const;
	using difference_type = bounded::integer<-max_size, max_size>;
	using index_type = SharedMoveIndex;
	using iterator_category = std::random_access_iterator_tag;

	auto operator*() const -> value_type;
	decltype(auto) operator[](index_type const index) const {
		return *(*this + index);
	}

	friend constexpr auto operator+(SharedMovesIterator const lhs, difference_type const rhs) -> SharedMovesIterator {
		return SharedMovesIterator(static_cast<underlying_index_type>(lhs.m_index + rhs));
	}
	friend constexpr auto operator-(SharedMovesIterator const lhs, SharedMovesIterator const rhs) -> difference_type {
		return lhs.m_index - rhs.m_index;
	}

	friend constexpr auto operator==(SharedMovesIterator const lhs, SharedMovesIterator const rhs) noexcept -> bool {
		return lhs.m_index == rhs.m_index;
	}
	friend constexpr auto operator<(SharedMovesIterator const lhs, SharedMovesIterator const rhs) noexcept -> bool {
		return lhs.m_index < rhs.m_index;
	}

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
	using index_type = SharedMoveIndex;
	using const_iterator = SharedMovesIterator;
	explicit SharedMoves(TeamSize team_size = max_pokemon_per_team);
	auto begin() const ->const_iterator;
	auto end() const -> const_iterator;
	auto operator[](index_type index) const -> Move;
	auto remove_switch() -> void;
private:
	TeamSize m_number_of_switches;
};

auto operator==(SharedMoves const & lhs, SharedMoves const & rhs) -> bool;

}	// namespace technicalmachine
