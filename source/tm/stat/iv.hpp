// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/generic_stats.hpp>

#include <tm/generation.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

using namespace bounded::literal;

struct IV {
	static constexpr auto max = 31;
	using value_type = bounded::integer<0, max>;
	constexpr explicit IV(value_type const iv):
		m_value(iv) {
	}
	constexpr auto value() const -> value_type {
		return m_value;
	}

	friend auto operator==(IV, IV) -> bool = default;
private:
	value_type m_value;
};

constexpr auto default_iv(Generation const generation) {
	return generation <= Generation::two ? IV(30_bi) : IV(31_bi);
}

using IVs = GenericStats<IV>;

} // namespace technicalmachine
