// Moves specific to one Pokemon and shared moves (Struggle and switches)
// Copyright (C) 2018 David Stone
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

#include "../operators.hpp"
#include "../range.hpp"

#include <bounded/integer.hpp>
#include <bounded/integer_range.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/static_vector/static_vector.hpp>

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

using RegularMoveContainer = containers::static_vector<Move, max_moves_per_pokemon.value()>;

struct MoveIterator {
private:
	static constexpr auto max_difference = static_cast<intmax_t>(std::numeric_limits<MoveSize>::max() + 1_bi);
public:
	using value_type = Move const;
	using difference_type = bounded::integer<-max_difference, max_difference>;
	using pointer = value_type *;
	using reference = value_type &;
	using iterator_category = std::random_access_iterator_tag;

	constexpr auto operator*() const -> value_type {
		return (m_regular.begin() != m_regular.end()) ? *m_regular.begin() : *m_shared;
	}
	CONTAINERS_OPERATOR_BRACKET_DEFINITIONS(MoveIterator)

	friend auto operator+(MoveIterator const lhs, difference_type const rhs) -> MoveIterator;
	friend auto operator-(MoveIterator const lhs, MoveIterator const rhs) -> difference_type;

	friend constexpr auto compare(MoveIterator const lhs, MoveIterator const rhs) noexcept {
		auto as_tuple = [](auto const value) {
			return containers::make_tuple(value.m_regular.begin(), value.m_shared);
		};
		return compare(as_tuple(lhs), as_tuple(rhs));
	}

private:
	using range_t = Range<RegularMoveContainer::const_iterator>;
	friend struct MoveContainer;
	constexpr MoveIterator(range_t regular, SharedMovesIterator shared) noexcept:
		m_regular(std::move(regular)),
		m_shared(std::move(shared)) {
	}
	
	range_t m_regular;
	SharedMovesIterator m_shared;
};


struct MoveContainer {
	using value_type = Move;
	using size_type = MoveSize;
	using const_iterator = MoveIterator;
	using const_regular_iterator = RegularMoveContainer::const_iterator;
	using regular_iterator = RegularMoveContainer::iterator;
	
	explicit constexpr MoveContainer(TeamSize const my_team_size):
		m_shared(my_team_size)
	{
	}

	constexpr auto number_of_regular_moves() const {
		return static_cast<RegularMoveSize>(size(m_regular));
	}

	// Skips Struggle and switches
	constexpr auto regular() const {
		return make_range(begin(m_regular), end(m_regular));
	}
	constexpr auto regular() {
		return make_range(begin(m_regular), end(m_regular));
	}
	
	friend constexpr const_iterator begin(MoveContainer const & container) {
		return const_iterator(container.regular(), begin(container.m_shared));
	}
	friend constexpr const_iterator end(MoveContainer const & container) {
		return const_iterator(MoveIterator::range_t(container.regular().end(), container.regular().end()), end(container.m_shared));
	}

	CONTAINERS_OPERATOR_BRACKET_DEFINITIONS(MoveContainer)

	template<typename M, typename... MaybePP>
	constexpr auto & emplace_back(M const move, MaybePP... maybe_pp) {
		assert(containers::none_equal(begin(m_regular), end(m_regular), move));
		return m_regular.emplace_back(move, maybe_pp...);
	}

	constexpr auto remove_switch() {
		m_shared.remove_switch();
	}
	
private:
	RegularMoveContainer m_regular;
	SharedMoves m_shared;
};

}	// namespace technicalmachine
