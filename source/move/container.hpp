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

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/concatenate_view.hpp>
#include <containers/static_vector/static_vector.hpp>

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

using RegularMoveContainer = containers::static_vector<Move, max_moves_per_pokemon.value()>;

struct MoveContainer {
	using value_type = Move;
	using size_type = MoveSize;
	using const_iterator = containers::concatenate_view_iterator<
		RegularMoveContainer::const_iterator,
		RegularMoveContainer::const_iterator,
		SharedMoves::const_iterator
	>;
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
		return range_view(begin(m_regular), end(m_regular));
	}
	constexpr auto regular() {
		return range_view(begin(m_regular), end(m_regular));
	}
	
	friend constexpr auto begin(MoveContainer const & container) {
		return const_iterator(begin(container.m_regular), end(container.m_regular), begin(container.m_shared));
	}
	friend constexpr auto end(MoveContainer const & container) {
		return containers::concatenate_view_sentinel(end(container.m_shared));
	}

	template<typename M, typename... MaybePP>
	constexpr auto & emplace_back(M const move, MaybePP... maybe_pp) {
		assert(containers::none_equal(m_regular, move));
		return m_regular.emplace_back(move, maybe_pp...);
	}

	constexpr auto remove_switch() {
		m_shared.remove_switch();
	}
	
private:
	RegularMoveContainer m_regular;
	SharedMoves m_shared;
};

using ::containers::detail::common::compare;

template<typename... MaybePP>
auto & add_seen_move(MoveContainer & container, Moves const move, MaybePP... pp) {
	auto const regular = container.regular();
	auto const it = containers::find(begin(regular), end(regular), move);
	if (it != end(regular)) {
		return *it;
	}
	if (size(container.regular()) == max_moves_per_pokemon) {
		throw std::runtime_error("Tried to add too many moves");
	}
	return container.emplace_back(move, pp...);
}

}	// namespace technicalmachine
