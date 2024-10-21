// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.selector;

import tm.generation;
import tm.evaluate.state;

namespace technicalmachine {

template<typename T>
struct SelectionPair {
	T & selection;
	T & other;
};

export struct Selector {
	constexpr explicit Selector(bool const select_ai):
		m_select_ai(select_ai)
	{
	}

	template<typename T>
	constexpr auto operator()(
		[[clang::lifetimebound]] T & lhs,
		[[clang::lifetimebound]] T & rhs
	) const -> SelectionPair<T> {
		return m_select_ai ?
			SelectionPair(lhs, rhs) :
			SelectionPair(rhs, lhs);
	}

	template<Generation generation>
	constexpr auto operator()([[clang::lifetimebound]] State<generation> const & state) const {
		return operator()(state.ai, state.foe);
	}
	template<Generation generation>
	constexpr auto operator()([[clang::lifetimebound]] State<generation> & state) const {
		return operator()(state.ai, state.foe);
	}

	constexpr auto invert() const -> Selector {
		return Selector(!m_select_ai);
	}
private:
	bool m_select_ai;
};

} // namespace technicalmachine
