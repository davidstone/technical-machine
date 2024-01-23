// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.switch_;

import tm.pokemon.max_pokemon_per_team;

import bounded;

namespace technicalmachine {

export struct Switch {
	constexpr explicit Switch(TeamIndex const index):
		m_index(index)
	{
	}
	constexpr auto value() const -> TeamIndex {
		return m_index;
	}
	friend constexpr auto operator==(Switch, Switch) -> bool = default;
private:
	TeamIndex m_index;

	constexpr explicit Switch(bounded::tombstone_tag, auto const make):
		m_index(make())
	{
	}
	friend bounded::tombstone_traits<Switch>;
	friend bounded::tombstone_traits_composer<&Switch::m_index>;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone_traits<technicalmachine::Switch> : bounded::tombstone_traits_composer<&technicalmachine::Switch::m_index> {
};
