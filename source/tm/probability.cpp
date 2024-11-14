// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.probability;

import bounded;
import std_module;

namespace technicalmachine {

export struct Probability {
	constexpr Probability(double const value, bounded::unchecked_t):
		m_value(value)
	{
	}
	constexpr explicit Probability(double const value):
		m_value(value)
	{
		auto const valid = 0.0 <= value and value <= 1.0;
		if (!valid) {
			throw std::runtime_error("Invalid probability");
		}
	}

	template<std::floating_point U>
	constexpr explicit operator U() const {
		return U(m_value);
	}

	friend constexpr auto operator<=>(Probability, Probability) = default;

	friend constexpr auto operator+(Probability const lhs, Probability const rhs) -> Probability {
		return Probability(lhs.m_value + rhs.m_value);
	}
	friend constexpr auto operator-(Probability const lhs, Probability const rhs) -> Probability {
		return Probability(lhs.m_value - rhs.m_value);
	}
	friend constexpr auto operator*(Probability const lhs, Probability const rhs) -> Probability {
		return Probability(lhs.m_value * rhs.m_value, bounded::unchecked);
	}

	friend auto operator<<(std::ostream & stream, Probability const p) -> std::ostream & {
		return stream << p.m_value * 100.0 << '%';
	}
private:
	double m_value;
};

} // namespace technicalmachine
