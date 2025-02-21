// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <ostream>

export module tm.pokemon.happiness;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct Happiness {
	using value_type = bounded::integer<0, 255>;
	constexpr explicit Happiness(value_type const value = 255_bi):
		m_value(value)
	{
	}

	constexpr auto operator()() const -> value_type {
		return m_value;
	}

	friend auto operator==(Happiness, Happiness) -> bool = default;
	template<typename CharT, typename Traits>
	friend std::basic_ostream<CharT, Traits> & operator<<(std::basic_ostream<CharT, Traits> & out, Happiness const happiness) {
		return out << happiness();
	}

private:
	value_type m_value;
};

export constexpr auto return_power(Happiness const happiness) -> bounded::integer<0, 102> {
	return happiness() * 2_bi / 5_bi;
}
export constexpr auto frustration_power(Happiness const happiness) -> bounded::integer<0, 102> {
	return 102_bi - return_power(happiness);
}

} // namespace technicalmachine
