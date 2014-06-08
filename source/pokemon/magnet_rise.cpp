// Class that handles Magnet Rise
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

#include "magnet_rise.hpp"

namespace technicalmachine {
using namespace bounded::literal;

auto MagnetRise::is_active() const -> bool {
	return turns_remaining() != 0_bi;
}

auto MagnetRise::activate() -> void {
	if (!is_active()) {
		m_turns_remaining = std::numeric_limits<duration_type>::max();
	}
}

auto MagnetRise::decrement() -> void {
	--m_turns_remaining;
}

auto MagnetRise::turns_remaining() const -> duration_type {
	return m_turns_remaining;
}

auto operator== (MagnetRise const lhs, MagnetRise const rhs) -> bool {
	return lhs.turns_remaining() == rhs.turns_remaining();
}

auto operator!= (MagnetRise const lhs, MagnetRise const rhs) -> bool{
	return !(lhs == rhs);
}

}	// namespace technicalmachine
