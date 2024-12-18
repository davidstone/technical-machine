// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.level;

import tm.compress;

import bounded;

namespace technicalmachine {

export struct Level {
	using value_type = bounded::integer<1, 100>;
	constexpr explicit Level(value_type const level) :
		m_value(level)
	{
	}
	constexpr auto operator()() const -> value_type {
		return m_value;
	}

	friend auto operator==(Level, Level) -> bool = default;
	friend constexpr auto compress(Level const value) {
		return compress(value.m_value);
	}

private:
	value_type m_value;

	constexpr explicit Level(bounded::tombstone_tag, auto const make):
		m_value(make())
	{
	}
	friend bounded::tombstone<Level>;
	friend bounded::tombstone_member<&Level::m_value>;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone<technicalmachine::Level> : bounded::tombstone_member<&technicalmachine::Level::m_value> {
};
