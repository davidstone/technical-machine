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
#include <tm/move/regular_moves.hpp>
#include <tm/move/shared.hpp>

#include <tm/compress.hpp>
#include <tm/operators.hpp>

#include <containers/algorithms/concatenate_view.hpp>
#include <containers/algorithms/transform.hpp>

namespace technicalmachine {

constexpr auto move_container_transform(auto const & range) {
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
		Transformed<RegularMoves>,
		Transformed<SharedMoves>
	>;
	using iterator = const_iterator;
	
	explicit MoveContainer(Generation const generation, RegularMoves const & regular, TeamSize const my_team_size):
		m_regular(regular),
		m_shared(generation, my_team_size)
	{
	}

	auto const & regular() const {
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

	auto remove_switch() {
		m_shared.remove_switch();
	}

private:
	RegularMoves const & m_regular;
	SharedMoves m_shared;
};

}	// namespace technicalmachine
