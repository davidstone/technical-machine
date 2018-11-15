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

	friend constexpr auto operator==(BideDuration const lhs, BideDuration const rhs) {
		return lhs.m_charged_up == rhs.m_charged_up;
	}

private:
	bool m_charged_up = false;
};

}	// namespace technicalmachine
