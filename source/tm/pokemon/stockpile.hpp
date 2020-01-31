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
#include <tm/rational.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Stockpile {
	static constexpr auto max = 3;
	// Returns whether Stockpile was able to increment (true) or if it is
	// already maxed out (false)
	constexpr auto increment() {
		auto const initial = m_level;
		++m_level;
		return m_level == initial;
	}

	// Returns the amount of Stockpile lost. This allows correct subtraction
	// for stat boosts.
	constexpr auto release() -> bounded::integer<0, max> {
		auto const temp = m_level;
		*this = {};
		return temp;
	}

	constexpr auto spit_up_power() const {
		return m_level * 100_bi;
	}

	friend auto operator==(Stockpile const &, Stockpile const &) -> bool = default;

private:
	friend struct Evaluate;
	bounded::clamped_integer<0, max> m_level = 0_bi;
};

constexpr auto swallow_healing(bounded::checked_integer<1, Stockpile::max> const stockpiles) {
	using result = rational<bounded::integer<1, 1>, bounded::integer<1, 4>>;
	switch (stockpiles.value()) {
		case 1:
			return result{1_bi, 4_bi};
		case 2:
			return result{1_bi, 2_bi};
		default:	// case 3:
			return result{1_bi, 1_bi};
	}
}

}	// namespace technicalmachine
