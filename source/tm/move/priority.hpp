// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/operators.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
enum class MoveName : std::uint16_t;

struct Priority {
	explicit Priority(Generation generation, MoveName move);
	friend auto operator<=>(Priority, Priority) = default;
private:
	bounded::integer<-6, 6> priority;
};

}	// namespace technicalmachine
