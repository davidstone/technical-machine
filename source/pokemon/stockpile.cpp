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

#include "stockpile.hpp"
#include "../rational.hpp"

namespace technicalmachine {

auto Stockpile::increment() -> bool {
	auto const initial = m_level;
	++m_level;
	return m_level == initial;
}

auto Stockpile::release() -> bounded::integer<0, max> {
	auto const temp = m_level;
	*this = {};
	return temp;
}

auto Stockpile::spit_up_power() const -> bounded::integer<0, max * 100> {
	return m_level * 100_bi;
}


auto swallow_healing(bounded::checked_integer<1, Stockpile::max> const stockpiles) -> SwallowHealing{
	switch (stockpiles.value()) {
		case 1:
			return {1_bi, 4_bi};
		case 2:
			return {1_bi, 2_bi};
		default:	// case 3:
			return {1_bi, 1_bi};
	}
}

auto operator==(Stockpile lhs, Stockpile rhs) -> bool {
	return lhs.m_level == rhs.m_level;
}

auto operator!=(Stockpile lhs, Stockpile rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
