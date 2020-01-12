// Copyright (C) 2019 David Stone
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

#include <tm/generation.hpp>

#include <cstdint>

namespace technicalmachine {
enum class Moves : std::uint16_t;

enum class Target {
	user,
	adjacent_ally,
	user_or_adjacent_ally,
	all_allies,
	user_team,
	user_field,
	adjacent_foe,
	all_adjacent_foes,
	foe_field,
	adjacent,
	any,
	all_adjacent,
	all,
	field,
};

auto move_target(Generation generation, Moves move) -> Target;

inline auto move_targets_foe(Generation const generation, Moves const move) {
	switch (move_target(generation, move)) {
		case Target::user:
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::all_allies:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
		case Target::field:
			return false;
		case Target::adjacent_foe:
		case Target::all_adjacent_foes:
		case Target::adjacent:
		case Target::any:
		case Target::all_adjacent:
		case Target::all:
			return true;
	}
}

}	// namespace technicalmachine
