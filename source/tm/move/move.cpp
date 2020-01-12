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

#include <tm/move/move.hpp>

#include <cstdint>

#include <tm/move/base_power.hpp>
#include <tm/move/is_switch.hpp>
#include <tm/move/moves.hpp>

namespace technicalmachine {

auto is_phaze(Moves const move) -> bool {
	switch (move) {
		case Moves::Circle_Throw:
		case Moves::Dragon_Tail:
		case Moves::Roar:
		case Moves::Whirlwind:
			return true;
		default:
			return false;
	}
}

auto is_usable_while_frozen(Moves const move) -> bool {
	switch (move) {
		case Moves::Flame_Wheel:
		case Moves::Sacred_Fire:
		case Moves::Scald:
			return true;
		default:
			return false;
	}
}

auto is_regular(Moves const move) -> bool {
	switch (move) {
		case Moves::Pass:
		case Moves::Hit_Self:
		case Moves::Struggle:
			return false;
		default:
			return !is_switch(move);
	}
}

}	// namespace technicalmachine
