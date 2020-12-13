// Handles when Bide activates
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/operators.hpp>

#include <utility>

namespace technicalmachine {

struct BideDuration {
	// returns whether Bide releases damage
	[[nodiscard]] constexpr auto advance_one_turn() {
		if (m_charged_up) {
			return true;
		} else {
			m_charged_up = true;
			return false;
		}
	}

	friend auto operator==(BideDuration const &, BideDuration const &) -> bool = default;
	friend constexpr auto compress(BideDuration const value) {
		return compress(value.m_charged_up);
	}
private:
	bool m_charged_up = false;
};

}	// namespace technicalmachine
