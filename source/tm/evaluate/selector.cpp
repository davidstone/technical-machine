// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.selector;

import tm.generation;
import tm.evaluate.state;
import tm.team;

namespace technicalmachine {

template<typename TeamType>
struct SelectedTeams {
	TeamType & team;
	TeamType & other;
};

export struct Selector {
	constexpr explicit Selector(bool const select_ai):
		m_select_ai(select_ai)
	{
	}
	template<Generation generation>
	constexpr auto operator()(State<generation> const & state) const -> SelectedTeams<Team<generation> const> {
		return m_select_ai ?
			SelectedTeams<Team<generation> const>(state.ai, state.foe) :
			SelectedTeams<Team<generation> const>(state.foe, state.ai);
	}
	template<Generation generation>
	constexpr auto operator()(State<generation> & state) const -> SelectedTeams<Team<generation>> {
		return m_select_ai ?
			SelectedTeams<Team<generation>>(state.ai, state.foe) :
			SelectedTeams<Team<generation>>(state.foe, state.ai);
	}

	constexpr auto invert() const -> Selector {
		return Selector(!m_select_ai);
	}
private:
	bool m_select_ai;
};

} // namespace technicalmachine
