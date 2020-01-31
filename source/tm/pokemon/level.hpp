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

#include <bounded/integer.hpp>

namespace technicalmachine {

struct Level {
	using value_type = bounded::checked_integer<1, 100>;
	constexpr explicit Level(value_type const level) :
		m_value(level)
	{
	}
	constexpr auto operator()() const -> bounded::change_policy<value_type, bounded::integer> {
		return m_value;
	}
private:
	value_type m_value;
};

constexpr auto operator==(Level const lhs, Level const rhs) -> bool {
	return lhs() == rhs();
}

}	// namespace technicalmachine
