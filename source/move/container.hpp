// Moves specific to one Pokemon and shared moves (Struggle and switches)
// Copyright (C) 2014 David Stone
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

#ifndef MOVE__CONTAINER_HPP_
#define MOVE__CONTAINER_HPP_

#include "max_moves_per_pokemon.hpp"
#include "move.hpp"
#include "shared.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/integer_range.hpp>

#include <vector>

namespace technicalmachine {

class MoveIterator {
private:
	static constexpr auto max_size = static_cast<intmax_t>(std::numeric_limits<MoveSize>::max());
public:
	using value_type = Move const;
	using difference_type = bounded::integer<-max_size, max_size>;
	using index_type = MoveIndex;
	using pointer = value_type *;
	using reference = value_type &;
	using iterator_category = std::random_access_iterator_tag;

	auto operator*() const -> reference {
		return (m_regular != m_regular_end) ? *m_regular : *m_shared;
	}
	auto operator->() const -> pointer {
		return & operator*();
	}
	auto operator[](index_type const index) const -> reference {
		return *(*this + index);
	}

	friend auto operator+(MoveIterator const lhs, difference_type const rhs) -> MoveIterator;
	friend auto operator-(MoveIterator const lhs, MoveIterator const rhs) -> difference_type;

	friend auto operator==(MoveIterator const lhs, MoveIterator const rhs) noexcept -> bool;
	friend auto operator<(MoveIterator const lhs, MoveIterator const rhs) noexcept -> bool;

private:
	friend class MoveContainer;
	MoveIterator(std::vector<Move>::const_iterator regular, std::vector<Move>::const_iterator regular_end, SharedMovesIterator shared) noexcept:
		m_regular(std::move(regular)),
		m_regular_end(std::move(regular_end)),
		m_shared(std::move(shared)) {
	}
	
	std::vector<Move>::const_iterator m_regular;
	std::vector<Move>::const_iterator m_regular_end;
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


class MoveContainer {
public:
	using value_type = Move;
	using size_type = MoveSize;
	using index_type = MoveIndex;
	using const_iterator = MoveIterator;
	using const_regular_iterator = std::vector<Move>::const_iterator;
	using regular_iterator = std::vector<Move>::iterator;
	
	explicit MoveContainer(TeamSize my_team_size);
	
	const_iterator begin() const {
		return const_iterator(m_regular.begin(), m_regular.end(), m_shared.begin());
	}
	const_iterator end() const {
		return const_iterator(m_regular.end(), m_regular.end(), m_shared.end());
	}
	const_regular_iterator regular_begin() const {
		return m_regular.begin();
	}
	regular_iterator regular_begin() {
		return m_regular.begin();
	}
	const_regular_iterator regular_end() const {
		return m_regular.end();
	}
	regular_iterator regular_end() {
		return m_regular.end();
	}
	
	auto operator[](index_type index) const -> Move const &;
	// Skips Struggle and switches
	auto regular_move(RegularMoveIndex index) const -> Move const &;
	auto regular_move(RegularMoveIndex index) -> Move &;
	static constexpr auto empty() -> bool {
		// A move container is never empty, it always contains at least Struggle
		return false;
	}
	template<class... Args>
	auto emplace_back(Args&&... args) -> void {
		// The only moves that are ever added are regular moves. Shared
		// moves are just a reference to a collection at the Team level.
		m_regular.emplace_back(std::forward<Args>(args)...);
	}

	auto size() const -> size_type;
	auto number_of_regular_moves() const -> RegularMoveSize;
	auto remove_switch() -> void;
	
	using hash_type = uint64_t;
	auto hash() const -> hash_type;
	auto max_hash() const -> hash_type;
	
	friend auto operator==(MoveContainer const & lhs, MoveContainer const & rhs) -> bool;
private:
	auto unchecked_regular_move(RegularMoveIndex index) const -> Move const &;
	auto unchecked_regular_move(RegularMoveIndex index) -> Move &;
	std::vector<Move> m_regular;
	SharedMoves m_shared;
};

}	// namespace technicalmachine
#endif	// MOVE__CONTAINER_HPP_
