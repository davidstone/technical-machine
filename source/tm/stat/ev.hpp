// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/generic_stats.hpp>

#include <tm/generation.hpp>

#include <bounded/detail/conditional.hpp>
#include <bounded/detail/construct_destroy.hpp>
#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct EV {
	static constexpr auto max = 255_bi;
	static constexpr auto useful_max = 252_bi;
	using value_type = bounded::integer<0, static_cast<int>(max)>;

	constexpr explicit EV(value_type evs):
		m_value(evs) {
	}
	constexpr auto value() const -> value_type {
		return m_value;
	}

	friend auto operator<=>(EV, EV) = default;
	friend auto operator==(EV, EV) -> bool = default;
	friend constexpr auto operator<=>(EV const lhs, EV::value_type const rhs) {
		return lhs.value() <=> rhs;
	}
	friend constexpr auto operator==(EV const lhs, EV::value_type const rhs) -> bool {
		return lhs.value() == rhs;
	}

private:
	value_type m_value;
};

using EVs = GenericStats<EV>;

constexpr auto max_total_evs(Generation const generation) {
	return
		BOUNDED_CONDITIONAL(generation <= Generation::two, 252_bi * 6_bi,
		508_bi
	);
}

constexpr auto ev_sum(EVs const evs) {
	return evs.hp().value() + evs.atk().value() + evs.def().value() + evs.spa().value() + evs.spd().value() + evs.spe().value();
}

} // namespace technicalmachine
