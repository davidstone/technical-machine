// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/arithmetic/common_arithmetic.hpp>

export module tm.evaluate.score;

import std_module;

namespace technicalmachine {

BOUNDED_COMMON_ARITHMETIC(export)

export struct Score {
	constexpr explicit Score(double const value):
		m_value(value)
	{
	}

	friend constexpr auto operator<=>(Score, Score) = default;

	friend constexpr auto operator+(Score const lhs, Score const rhs) -> Score {
		return Score(lhs.m_value + rhs.m_value);
	}

	friend constexpr auto operator-(Score const score) -> Score {
		return Score(-score.m_value);
	}

	friend constexpr auto operator*(Score const lhs, double const rhs) -> Score {
		return Score(lhs.m_value * rhs);
	}
	friend constexpr auto operator*(double const lhs, Score const rhs) -> Score {
		return Score(lhs * rhs.m_value);
	}

	friend constexpr auto operator/(Score const lhs, double const rhs) -> Score {
		return Score(lhs.m_value / rhs);
	}
private:
	double m_value;
};

} // namespace technicalmachine
