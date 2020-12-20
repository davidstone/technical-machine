// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/operators.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

struct Level {
	using value_type = bounded::integer<1, 100>;
	constexpr explicit Level(value_type const level) :
		m_value(level)
	{
	}
	constexpr auto operator()() const -> value_type {
		return m_value;
	}

	friend auto operator==(Level, Level) -> bool = default;
	friend constexpr auto compress(Level const value) {
		return compress(value.m_value);
	}
private:
	value_type m_value;
};

}	// namespace technicalmachine
