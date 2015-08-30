// EVs
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

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct EV {
	static constexpr auto max = 252_bi;
	static constexpr auto max_total = 508_bi;
	using value_type = bounded::checked_integer<0, max.value()>;
	using total_type = bounded::checked_integer<0, max_total.value()>;

	constexpr explicit EV(value_type evs):
		m_value(evs) {
	}
	constexpr auto value() const -> bounded::integer<0, max.value()> {
		return m_value;
	}
private:
	bounded::clamped_integer<0, max.value()> m_value;
};

}	// namespace technicalmachine
