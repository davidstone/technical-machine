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

#pragma once

#include "../hash.hpp"
#include "../rational.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct Toxic {
	auto increment() -> void;
	constexpr auto ratio_drained() const {
		return make_rational(-m_counter, 16_bi);
	}
	friend auto operator==(Toxic lhs, Toxic rhs) -> bool;
	constexpr auto hash() const noexcept {
		return technicalmachine::hash(m_counter);
	}
private:
	friend struct Evaluate;
	// Number of turns this Pokemon has already taken Toxic damage (or
	// would have if Magic Guard / Poison Heal weren't in play)
	bounded::clamped_integer<0, 15> m_counter = 0_bi;
};
auto operator!=(Toxic lhs, Toxic rhs) -> bool;

constexpr auto hash(Toxic const toxic) noexcept {
	return toxic.hash();
}

}	// namespace technicalmachine
