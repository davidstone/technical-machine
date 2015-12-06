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

#include "../operators.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {

struct Level {
	static constexpr int min = 1;
	static constexpr int max = 100;
	explicit Level(bounded::checked_integer<min, max> level);
	bounded::integer<min, max> operator() () const;
private:
	bounded::checked_integer<min, max> m_value;
};

bool operator== (Level lhs, Level rhs);

}	// namespace technicalmachine
