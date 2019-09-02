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

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/shared.hpp>

#include <tm/string_conversions/move.hpp>

#include <tm/operators.hpp>

#include <bounded/assert.hpp>
#include <bounded/integer.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/concatenate_view.hpp>
#include <containers/algorithms/transform.hpp>
#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
using namespace bounded::literal;

using RegularMoveContainer = containers::static_vector<Move, max_moves_per_pokemon.value()>;

template<typename Range>
constexpr auto move_container_transform(Range const & range) {
	auto const transformed = containers::transform(range, [](auto value) { return value; });
	return containers::range_view(transformed);
}

struct MoveContainer {
private:
	template<typename Range>
	using Transformed = decltype(move_container_transform(std::declval<Range>()));
public:
	using value_type = Move;
	using size_type = MoveSize;
	using const_iterator = containers::concatenate_view_iterator<
		Transformed<RegularMoveContainer>,
		Transformed<SharedMoves>
	>;
	using iterator = const_iterator;
	using const_regular_iterator = RegularMoveContainer::const_iterator;
	using regular_iterator = RegularMoveContainer::iterator;
	
	explicit MoveContainer(TeamSize const my_team_size):
		m_shared(my_team_size)
	{
	}

	auto number_of_regular_moves() const {
		return static_cast<RegularMoveSize>(size(m_regular));
	}

	// Skips moves for which is_regular(move) is false
	auto const & regular() const {
		return m_regular;
	}
	auto & regular() {
		return m_regular;
	}
	
	friend auto begin(MoveContainer const & container) {
		return const_iterator(
			move_container_transform(container.m_regular),
			move_container_transform(container.m_shared)
		);
	}
	friend auto end(MoveContainer) {
		return containers::concatenate_view_sentinel();
	}

	auto & emplace_back(Move const move) {
		BOUNDED_ASSERT(containers::none_equal(m_regular, move.name()));
		return containers::emplace_back(m_regular, move);
	}

	template<typename... MaybePP>
	auto & emplace_back(Generation const generation, Moves const move, MaybePP... maybe_pp) {
		return emplace_back(Move(generation, move, maybe_pp...));
	}

	auto remove_switch() {
		m_shared.remove_switch();
	}
	
private:
	RegularMoveContainer m_regular;
	SharedMoves m_shared;
};

using ::containers::detail::common::compare;
using ::containers::detail::common::operator==;

template<typename... MaybePP>
auto add_seen_move(MoveContainer & container, Generation const generation, Moves const move, MaybePP... pp) {
	if (move == Moves::Pass or move == Moves::Struggle or move == Moves::Hit_Self) {
		return;
	}
	if (containers::any_equal(container, move)) {
		return;
	}
	if (size(container.regular()) == max_moves_per_pokemon) {
		auto message = std::string("Tried to add too many moves. Already have: ");
		for (auto const existing_move : container.regular()) {
			message += to_string(existing_move.name());
			message += ", ";
		}
		message += "-- Tried to add ";
		message += to_string(move);
		throw std::runtime_error(message);
	}
	containers::emplace_back(container, generation, move, pp...);
}

}	// namespace technicalmachine
