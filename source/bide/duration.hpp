// Handles when Bide activates
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

#include "../operators.hpp"

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <cassert>

namespace technicalmachine {

struct BideDuration {
	constexpr auto activate() {
		m_turns_active = 0_bi;
	}
	constexpr explicit operator bool() const {
		return static_cast<bool>(m_turns_active);
	}
	// returns whether Bide releases damage
	constexpr auto decrement() {
		assert(this->operator bool());
		if (*m_turns_active == max) {
			m_turns_active = bounded::none;
			return true;
		} else {
			++*m_turns_active;
			return false;
		}
	}

	friend constexpr auto compare(BideDuration const lhs, BideDuration const rhs) {
		return bounded::compare(lhs.m_turns_active, rhs.m_turns_active);
	}

private:
	static constexpr auto max = 1;
	bounded::optional<bounded::integer<0, max>> m_turns_active = bounded::none;
};

}	// namespace technicalmachine
