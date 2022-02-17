// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/operators.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Confusion {
	constexpr auto is_active() const {
		return static_cast<bool>(m_turns_spent_confused);
	}

	constexpr auto activate() {
		if (is_active()) {
			return;
		}
		m_turns_spent_confused = 0_bi;
	}

	constexpr auto do_turn() {
		// min_duration = 2;
		// max_duration = 5;
		if (!is_active()) {
			return;
		}
		if (*m_turns_spent_confused == max_duration) {
			m_turns_spent_confused = bounded::none;
		} else {
			++*m_turns_spent_confused;
		}
	}

	friend auto operator==(Confusion, Confusion) -> bool = default;
	friend constexpr auto compress(Confusion const value) {
		return compress(value.m_turns_spent_confused);
	}

private:
	static constexpr auto max_duration = 4;
	bounded::optional<bounded::integer<0, max_duration>> m_turns_spent_confused = bounded::none;
};

} // namespace technicalmachine
