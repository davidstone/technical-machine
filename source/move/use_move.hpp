// Use moves
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

#ifndef MOVE__USE_MOVE_HPP_
#define MOVE__USE_MOVE_HPP_

#include "../damage.hpp"

namespace technicalmachine {

class Team;
class Variable;
class Weather;

auto call_move(Team & user, Team & target, Weather & weather, Variable const & variable, bool damage_is_known = false) -> damage_type;

}	// namespace technicalmachine
#endif	// MOVE__USE_MOVE_HPP_
