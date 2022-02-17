// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/hp.hpp>

#include <compare>

namespace technicalmachine {

struct CurrentVisibleHP {
	constexpr explicit CurrentVisibleHP(HP::current_type const value_):
		m_value(value_)
	{
	}

	constexpr auto value() const {
		return m_value;
	}

	friend constexpr auto operator<=>(CurrentVisibleHP, CurrentVisibleHP) = default;

private:
	HP::current_type m_value;
};

struct MaxVisibleHP {
	constexpr explicit MaxVisibleHP(HP::max_type const value_):
		m_value(value_)
	{
	}

	constexpr auto value() const {
		return m_value;
	}

	friend constexpr auto operator<=>(MaxVisibleHP, MaxVisibleHP) = default;

private:
	HP::max_type m_value;
};

constexpr auto operator<=>(CurrentVisibleHP const lhs, MaxVisibleHP const rhs) {
	return lhs.value() <=> rhs.value();
}
constexpr auto operator==(CurrentVisibleHP const lhs, MaxVisibleHP const rhs) {
	return lhs.value() == rhs.value();
}

// For the AI's Pokemon, this is exact. For the foe's Pokemon, this is a rounded
// fraction out of 42 or 100.
struct VisibleHP {
	CurrentVisibleHP current;
	MaxVisibleHP max;
};

struct AllowedHP {
	constexpr AllowedHP(HP::current_type const min_, HP::current_type const value_, HP::current_type const max_):
		min(min_),
		value(value_),
		max(max_)
	{
	}
	constexpr explicit AllowedHP(HP::current_type const value_):
		AllowedHP(value_, value_, value_)
	{
	}
	HP::current_type min;
	HP::current_type value;
	HP::current_type max;
};

auto to_real_hp(HP::max_type, VisibleHP) -> AllowedHP;

} // namespace technicalmachine
