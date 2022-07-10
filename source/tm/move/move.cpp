// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/move/move.hpp>

#include <tm/move/is_switch.hpp>
#include <tm/move/move_name.hpp>

namespace technicalmachine {

auto is_delayed_switch(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Flip_Turn:
		case MoveName::U_turn:
			return true;
		default:
			return false;
	}
}

auto is_regular(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Pass:
		case MoveName::Hit_Self:
		case MoveName::Struggle:
			return false;
		default:
			return !is_switch(move);
	}
}

auto is_phaze(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Circle_Throw:
		case MoveName::Dragon_Tail:
		case MoveName::Roar:
		case MoveName::Whirlwind:
			return true;
		default:
			return false;
	}
}

auto thaws_user(MoveName const move) -> bool {
	switch (move) {
		case MoveName::Flame_Wheel:
		case MoveName::Pyro_Ball:
		case MoveName::Sacred_Fire:
		case MoveName::Scald:
		case MoveName::Scorching_Sands:
			return true;
		default:
			return false;
	}
}

} // namespace technicalmachine
