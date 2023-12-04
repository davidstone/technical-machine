// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.visible_hp;

import tm.stat.current_hp;
import tm.stat.max_hp;

namespace technicalmachine {

export struct CurrentVisibleHP {
	constexpr explicit CurrentVisibleHP(CurrentHP const value_):
		m_value(value_)
	{
	}

	constexpr auto value() const {
		return m_value;
	}

	friend constexpr auto operator<=>(CurrentVisibleHP, CurrentVisibleHP) = default;

private:
	CurrentHP m_value;
};

export struct MaxVisibleHP {
	constexpr explicit MaxVisibleHP(MaxHP const value_):
		m_value(value_)
	{
	}

	constexpr auto value() const {
		return m_value;
	}

	friend constexpr auto operator<=>(MaxVisibleHP, MaxVisibleHP) = default;

private:
	MaxHP m_value;
};

export constexpr auto operator<=>(CurrentVisibleHP const lhs, MaxVisibleHP const rhs) {
	return lhs.value() <=> rhs.value();
}
export constexpr auto operator==(CurrentVisibleHP const lhs, MaxVisibleHP const rhs) {
	return lhs.value() == rhs.value();
}

// For the AI's Pokemon, this is exact. For the foe's Pokemon, this is a rounded
// fraction out of 42 or 100.
export struct VisibleHP {
	CurrentVisibleHP current;
	MaxVisibleHP max;

	friend auto operator==(VisibleHP, VisibleHP) -> bool = default;
};

} // namespace technicalmachine
