// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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

	friend auto operator==(Confusion const &, Confusion const &) -> bool = default;
	friend constexpr auto compress(Confusion const value) {
		return compress(value.m_turns_spent_confused);
	}

private:
	friend struct Evaluate;
	static constexpr auto max_duration = 4;
	bounded::optional<bounded::integer<0, max_duration>> m_turns_spent_confused = bounded::none;
};

}	// namespace technicalmachine
