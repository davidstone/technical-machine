// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.container;

import tm.move.regular_moves;
import tm.move.shared;

import tm.pokemon.max_pokemon_per_team;

import tm.compress;
import tm.generation;
export import tm.operators;

import bounded;
import containers;

namespace technicalmachine {

constexpr auto move_container_transform(auto const & range) {
	return containers::range_view(containers::transform(range, bounded::copy));
}

export template<Generation generation>
struct MoveContainer {
private:
	template<typename Range>
	using Transformed = decltype(move_container_transform(bounded::declval<Range>()));

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
