// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
	user_and_all_allies,
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
		case Target::user_and_all_allies:
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

} // namespace technicalmachine
