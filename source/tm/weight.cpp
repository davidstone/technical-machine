// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.weight;

import tm.probability;

import bounded;
import std_module;

namespace technicalmachine {

export template<std::floating_point T = double>
struct Weight {
	constexpr Weight(T const value, bounded::unchecked_t):
		m_value(value)
	{
	}
	constexpr explicit Weight(T const value):
		m_value(value)
	{
		if (value < T(0.0) or !std::isfinite(value)) {
			throw std::runtime_error("Invalid weight");
		}
	}
	template<typename U>
	constexpr explicit Weight(Weight<U> const other):
		m_value(T(other))
	{
	}
	constexpr explicit Weight(Probability const value):
		m_value(T(value))
	{
	}

	template<std::floating_point U>
	constexpr explicit operator U() const {
		return U(m_value);
	}

	friend constexpr auto operator<=>(Weight, Weight) = default;

	friend constexpr auto operator+(Weight const lhs, Weight const rhs) -> Weight {
		return Weight(lhs.m_value + rhs.m_value);
	}
	friend constexpr auto operator*(Weight const lhs, Weight const rhs) -> Weight {
		return Weight(lhs.m_value * rhs.m_value);
	}
	friend constexpr auto operator/(Weight const lhs, Weight const rhs) -> T {
		return lhs.m_value / rhs.m_value;
	}
private:
	T m_value;
};

} // namespace technicalmachine
