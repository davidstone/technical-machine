// Handles when Bide activates
// Copyright (C) 2015 David Stone
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

#include "../hash.hpp"
#include "../operators.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/optional.hpp>

namespace technicalmachine {

struct BideDuration {
	auto activate() -> void;
	explicit operator bool() const;
	// returns whether Bide releases damage
	auto decrement() -> bool;
	friend auto operator== (BideDuration const & lhs, BideDuration const & rhs) -> bool;

	// Declare as member functions to work around gcc 4.9.0 bug
	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_turns_active);
	}
private:
	static constexpr auto max = 1;
	bounded::optional<bounded::integer<0, max>> m_turns_active = bounded::none;
};

constexpr auto hash(BideDuration const duration) noexcept {
	return duration.hash();
}

}	// namespace technicalmachine
