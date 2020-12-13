// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/happiness.hpp>

namespace technicalmachine {

Happiness::Happiness(value_type const value):
	m_value(value) {
}

auto Happiness::operator()() const -> value_type {
	return m_value;
}

bounded::integer<0, 102> return_power(Happiness const happiness) {
	return happiness.m_value * 2_bi / 5_bi;
}
bounded::integer<0, 102> frustration_power(Happiness const happiness) {
	return 102_bi - return_power(happiness);
}


}	// namespace technicalmachine
