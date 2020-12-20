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
	explicit Happiness(value_type value = 255_bi);
	// Needed to send out to simulators
	value_type operator()() const;
	friend bounded::integer<0, 102> return_power(Happiness happiness);
	friend auto operator==(Happiness, Happiness) -> bool = default;
private:
	value_type m_value;	
};

bounded::integer<0, 102> frustration_power(Happiness happiness);

template<typename CharT, typename Traits>
std::basic_ostream<CharT, Traits> & operator<<(std::basic_ostream<CharT, Traits> & out, Happiness const happiness) {
	return out << happiness();
}

}	// namespace technicalmachine
