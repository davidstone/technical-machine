// Class that handles which party I am
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

#include "../operators.hpp"

#include <bounded/integer.hpp>

namespace technicalmachine {

struct Party {
	using value_type = bounded::checked_integer<0, 1>;
	constexpr explicit Party(value_type const initial):
		m_party(initial)
	{
	}

	constexpr auto value() const {
		return m_party;
	}

private:
	value_type m_party;
};

constexpr auto operator==(Party const lhs, Party const rhs) {
	return lhs.value() == rhs.value();
}

constexpr auto other(Party const party) -> Party {
	using namespace bounded::literal;
	return Party(1_bi - party.value());
}

}	// namespace technicalmachine
