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
#include "../rational.hpp"

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Stockpile {
	static constexpr auto max = 3;
	// Returns whether Stockpile was able to increment (true) or if it is
	// already maxed out (false)
	auto increment() -> bool;
	// Returns the amount of Stockpile lost. This allows correct subtraction
	// for stat boosts.
	auto release() -> bounded::integer<0, max>;
	auto spit_up_power() const -> bounded::integer<0, max * 100>;
	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_level);
	}
	friend auto operator==(Stockpile lhs, Stockpile rhs) -> bool;
private:
	friend struct Evaluate;
	bounded::clamped_integer<0, max> m_level = 0_bi;
};

using SwallowHealing = bounded_rational<bounded::integer<1, 1>, bounded::integer<1, 4>>;
auto swallow_healing(bounded::checked_integer<1, Stockpile::max> stockpiles) -> SwallowHealing;

constexpr auto hash(Stockpile const stockpile) noexcept {
	return stockpile.hash();
}


}	// namespace technicalmachine
