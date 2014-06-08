// Class that handles Stockpile
// Copyright (C) 2014 David Stone
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

#ifndef STOCKPILE_HPP_
#define STOCKPILE_HPP_

#include "../hash.hpp"
#include "../rational.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
class Rational;
using namespace bounded::literal;

class Stockpile {
public:
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
	constexpr auto max_hash() const noexcept {
		return technicalmachine::max_hash(m_level);
	}
	friend auto operator==(Stockpile lhs, Stockpile rhs) -> bool;
private:
	friend class Evaluate;
	bounded::clamped_integer<0, max> m_level = 0_bi;
};
auto operator!=(Stockpile lhs, Stockpile rhs) -> bool;

using SwallowHealing = bounded_rational<bounded::integer<1, 1>, bounded::integer<1, 4>>;
auto swallow_healing(bounded::checked_integer<1, Stockpile::max> stockpiles) -> SwallowHealing;

constexpr auto hash(Stockpile const stockpile) noexcept {
	return stockpile.hash();
}
constexpr auto max_hash(Stockpile const stockpile) noexcept {
	return stockpile.max_hash();
}


}	// namespace technicalmachine
#endif	// STOCKPILE_HPP_
