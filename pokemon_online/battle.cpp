// Pokemon Online battle logic
// Copyright (C) 2011 David Stone
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

#include "battle.h"
#include <string>
#include <vector>
#include "../pokemon.h"
#include "../team.h"

namespace technicalmachine {
namespace po {

Battle::Battle (std::string const & opponent, int battle_depth):
	GenericBattle::GenericBattle (opponent, battle_depth) {
}

} // namespace po
} // namespace technicalmachine
