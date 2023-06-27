// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.ev;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export struct EV {
	static constexpr auto max = 255_bi;
	static constexpr auto useful_max = 252_bi;
	using value_type = bounded::integer<0, bounded::normalize<max>>;

	// So it can be stored in a constexpr static_vector
	EV() = default;

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

} // namespace technicalmachine
