// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.visible_hp;

import tm.stat.current_hp;
import tm.stat.max_hp;

import bounded;

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
	friend constexpr auto operator==(CurrentVisibleHP, CurrentVisibleHP) -> bool = default;
	friend constexpr auto operator<=>(CurrentVisibleHP const lhs, bounded::constant_t<0> const rhs) {
		return lhs.m_value <=> rhs;
	}
	friend constexpr auto operator==(CurrentVisibleHP const lhs, bounded::constant_t<0> const rhs) -> bool {
		return lhs.m_value == rhs;
	}

private:
	constexpr explicit CurrentVisibleHP(bounded::tombstone_tag, auto const make) noexcept:
		m_value(make())
	{
	}

	CurrentHP m_value;

	friend bounded::tombstone_traits<CurrentVisibleHP>;
	friend bounded::tombstone_traits_composer<&CurrentVisibleHP::m_value>;
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
	constexpr explicit MaxVisibleHP(bounded::tombstone_tag, auto const make) noexcept:
		m_value(make())
	{
	}

	MaxHP m_value;

	friend bounded::tombstone_traits<MaxVisibleHP>;
	friend bounded::tombstone_traits_composer<&MaxVisibleHP::m_value>;
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

template<>
struct bounded::tombstone_traits<technicalmachine::CurrentVisibleHP> : bounded::tombstone_traits_composer<&technicalmachine::CurrentVisibleHP::m_value> {
};

template<>
struct bounded::tombstone_traits<technicalmachine::MaxVisibleHP> : bounded::tombstone_traits_composer<&technicalmachine::MaxVisibleHP::m_value> {
};

template<>
struct bounded::tombstone_traits<technicalmachine::VisibleHP> {
private:
	using base = tombstone_traits<technicalmachine::MaxVisibleHP>;
public:
	static constexpr auto spare_representations = base::spare_representations;

	static constexpr auto make(auto const index) noexcept -> technicalmachine::VisibleHP {
		return technicalmachine::VisibleHP{
			.current = tombstone_traits<technicalmachine::CurrentVisibleHP>::make(0_bi),
			.max = base::make(index)
		};
	}
	static constexpr auto index(technicalmachine::VisibleHP const & value) noexcept {
		return base::index(value.max);
	}
};
