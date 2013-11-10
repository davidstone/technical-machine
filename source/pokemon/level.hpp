// Level data structure
// Copyright (C) 2013 David Stone
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

#ifndef POKEMON__LEVEL_HPP_
#define POKEMON__LEVEL_HPP_

#include <ranged_integer/ranged_integer.hpp>

namespace technicalmachine {

class Level {
public:
	static constexpr int min = 1;
	static constexpr int max = 100;
	explicit Level(checked_integer<min, max> level);
	native_integer<min, max> operator() () const;
private:
	checked_integer<min, max> m_value;
};

bool operator== (Level lhs, Level rhs);
bool operator!= (Level lhs, Level rhs);

}	// namespace technicalmachine
#endif	// POKEMON__POKEMON_HPP_
