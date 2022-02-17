// Moves specific to one Pokemon and shared moves (Struggle and switches)
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>
#include <tm/move/regular_moves.hpp>
#include <tm/move/shared.hpp>

#include <tm/compress.hpp>
#include <tm/operators.hpp>

#include <bounded/copy.hpp>

#include <containers/algorithms/concatenate_view.hpp>
#include <containers/algorithms/transform.hpp>

namespace technicalmachine {

constexpr auto move_container_transform(auto const & range) {
	return containers::range_view(containers::transform(range, bounded::copy));
}

template<Generation generation>
struct MoveContainer {
private:
	template<typename Range>
	using Transformed = decltype(move_container_transform(std::declval<Range>()));

public:
	MoveContainer(RegularMoves regular, TeamSize const my_team_size):
		m_regular(regular),
		m_shared(my_team_size)
	{
	}

	auto const & regular() const {
		return m_regular;
	}

	auto begin() const {
		using iterator = containers::concatenate_view_iterator<
			Transformed<RegularMoves>,
			Transformed<SharedMoves<generation>>
		>;
		return iterator(
			move_container_transform(m_regular),
			move_container_transform(m_shared)
		);
	}
	static constexpr auto end() {
		return containers::concatenate_view_sentinel();
	}

	auto remove_switch() {
		m_shared.remove_switch();
	}

private:
	RegularMoves m_regular;
	SharedMoves<generation> m_shared;
};

} // namespace technicalmachine
