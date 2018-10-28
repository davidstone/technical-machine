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

#include <tm/operators.hpp>

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {
struct Pokemon;
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

	auto do_turn(Pokemon & pokemon) -> void;
	constexpr auto end_of_turn_reset() {
		m_is_hitting_self = false;
	}
	constexpr auto hit_self() {
		m_is_hitting_self = true;
	}

	friend constexpr auto operator==(Confusion const lhs, Confusion const rhs) {
		return lhs.m_turns_spent_confused == rhs.m_turns_spent_confused;
	}

private:
	auto increment() -> void;
	friend struct Evaluate;
	static constexpr auto max_duration = 4;
	bounded::optional<bounded::integer<0, max_duration>> m_turns_spent_confused = bounded::none;
	bool m_is_hitting_self = false;
};

}	// namespace technicalmachine
