// Moves specific to one Pokemon and shared moves (Struggle and switches)
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
#include "shared.hpp"

#include "../fixed_capacity_vector.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/integer_range.hpp>

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

using RegularMoveContainer = fixed_capacity_vector<Move, max_moves_per_pokemon.value()>;

struct MoveIterator {
private:
	static constexpr auto max_difference = static_cast<intmax_t>(std::numeric_limits<MoveSize>::max() + 1_bi);
public:
	using value_type = Move const;
	using difference_type = bounded::integer<-max_difference, max_difference>;
	using index_type = MoveIndex;
	using iterator_category = std::random_access_iterator_tag;

	auto operator*() const -> value_type {
		return (m_regular != m_regular_end) ? *m_regular : *m_shared;
	}
	auto operator[](index_type const index) const -> value_type {
		return *(*this + index);
	}

	friend auto operator+(MoveIterator const lhs, difference_type const rhs) -> MoveIterator;
	friend auto operator-(MoveIterator const lhs, MoveIterator const rhs) -> difference_type;

	friend auto operator==(MoveIterator const lhs, MoveIterator const rhs) noexcept -> bool;
	friend auto operator<(MoveIterator const lhs, MoveIterator const rhs) noexcept -> bool;

private:
	friend struct MoveContainer;
	MoveIterator(RegularMoveContainer::const_iterator regular, RegularMoveContainer::const_iterator regular_end, SharedMovesIterator shared) noexcept:
		m_regular(std::move(regular)),
		m_regular_end(std::move(regular_end)),
		m_shared(std::move(shared)) {
	}
	
	RegularMoveContainer::const_iterator m_regular;
	RegularMoveContainer::const_iterator m_regular_end;
	SharedMovesIterator m_shared;
};


inline auto operator!=(MoveIterator const lhs, MoveIterator const rhs) noexcept {
	return !(lhs == rhs);
}

inline auto operator>(MoveIterator const lhs, MoveIterator const rhs) noexcept {
	return rhs < lhs;
}
inline auto operator<=(MoveIterator const lhs, MoveIterator const rhs) noexcept {
	return !(lhs > rhs);
}
inline auto operator>=(MoveIterator const lhs, MoveIterator const rhs) noexcept {
	return !(lhs < rhs);
}


inline auto operator+(MoveIterator::difference_type const lhs, MoveIterator const rhs) -> MoveIterator {
	return rhs + lhs;
}
inline auto operator-(MoveIterator const lhs, MoveIterator::difference_type const rhs) -> MoveIterator {
	return lhs + -rhs;
}


inline auto operator+=(MoveIterator & it, MoveIterator::difference_type const offset) -> MoveIterator & {
	return it = it + offset;
}
inline auto operator-=(MoveIterator & it, MoveIterator::difference_type const offset) -> MoveIterator & {
	return it += -offset;
}


inline auto operator++(MoveIterator & it) -> MoveIterator & {
	return it += 1_bi;
}
inline auto operator++(MoveIterator & it, int) -> MoveIterator {
	auto const original = it;
	++it;
	return original;
}
inline auto operator--(MoveIterator & it) -> MoveIterator & {
	return it -= 1_bi;
}
inline auto operator--(MoveIterator & it, int) -> MoveIterator {
	auto const original = it;
	--it;
	return original;
}


struct MoveContainer {
	using value_type = Move;
	using size_type = MoveSize;
	using index_type = MoveIndex;
	using const_iterator = MoveIterator;
	using const_regular_iterator = RegularMoveContainer::const_iterator;
	using regular_iterator = RegularMoveContainer::iterator;
	
	explicit MoveContainer(TeamSize my_team_size);
	
	const_iterator begin() const {
		return const_iterator(m_regular.begin(), m_regular.end(), m_shared.begin());
	}
	const_iterator end() const {
		return const_iterator(m_regular.end(), m_regular.end(), m_shared.end());
	}

	// Skips Struggle and switches
	auto regular_begin() const {
		return m_regular.begin();
	}
	auto regular_begin() {
		return m_regular.begin();
	}
	auto regular_end() const {
		return m_regular.end();
	}
	auto regular_end() {
		return m_regular.end();
	}
	
	auto operator[](index_type index) const -> Move;
	static constexpr auto empty() -> bool {
		// A move container is never empty, it always contains at least Struggle
		return false;
	}
	template<typename... Args>
	auto emplace_back(Args&&... args) -> void {
		assert(m_regular.size() < max_moves_per_pokemon);
		// The only moves that are ever added are regular moves. Shared
		// moves are just a reference to a collection at the Team level.
		m_regular.emplace_back(std::forward<Args>(args)...);
	}

	auto size() const -> size_type;
	auto number_of_regular_moves() const -> RegularMoveSize;
	auto remove_switch() -> void;
	
	friend auto operator==(MoveContainer const & lhs, MoveContainer const & rhs) -> bool;
private:
	auto unchecked_regular_move(RegularMoveIndex index) const -> Move;
	auto unchecked_regular_move(RegularMoveIndex index) -> Move &;
	RegularMoveContainer m_regular;
	SharedMoves m_shared;
};

}	// namespace technicalmachine
