// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>
#include <iosfwd>

namespace technicalmachine {
using namespace bounded::literal;

struct Happiness {
	using value_type = bounded::integer<0, 255>;
	constexpr explicit Happiness(value_type const value = 255_bi):
		m_value(value)
	{
	}

	constexpr auto operator()() const -> value_type {
		return m_value;
	}

	friend auto operator==(Happiness, Happiness) -> bool = default;
private:
	value_type m_value;	
};

constexpr auto return_power(Happiness const happiness) -> bounded::integer<0, 102> {
	return happiness() * 2_bi / 5_bi;
}
constexpr auto frustration_power(Happiness const happiness) -> bounded::integer<0, 102> {
	return 102_bi - return_power(happiness);
}

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits> & operator<<(std::basic_ostream<CharT, Traits> & out, Happiness const happiness) {
	return out << happiness();
}

}	// namespace technicalmachine
