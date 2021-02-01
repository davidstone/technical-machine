// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/move.hpp>

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

auto thaws_user(Moves const move) -> bool {
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
