// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.move_is_unsuccessful;

import tm.move.move_name;
import tm.move.other_action;
import tm.move.target;

import tm.stat.current_hp;

import tm.ability;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto move_fails(MoveName const move, bool const user_damaged, Ability const other_ability, OtherAction const other_action) -> bool {
	switch (move) {
		case MoveName::Boomburst:
		case MoveName::Bug_Buzz:
		case MoveName::Chatter:
		case MoveName::Clanging_Scales:
		case MoveName::Clangorous_Soulblaze:
		case MoveName::Confide:
		case MoveName::Disarming_Voice:
		case MoveName::Echoed_Voice:
		case MoveName::Grass_Whistle:
		case MoveName::Growl:
		case MoveName::Hyper_Voice:
		case MoveName::Metal_Sound:
		case MoveName::Noble_Roar:
		case MoveName::Parting_Shot:
		case MoveName::Relic_Song:
		case MoveName::Roar:
		case MoveName::Round:
		case MoveName::Screech:
		case MoveName::Sing:
		case MoveName::Snarl:
		case MoveName::Snore:
		case MoveName::Sparkling_Aria:
		case MoveName::Supersonic:
		case MoveName::Uproar:
			return blocks_sound_moves(other_ability);
		case MoveName::Explosion:
		case MoveName::Mind_Blown:
		case MoveName::Self_Destruct:
			return other_ability == Ability::Damp;
		case MoveName::Focus_Punch:
			return user_damaged;
		case MoveName::Sucker_Punch:
			return !other_action.future_action_is_damaging();
		default:
			return false;
	}
}

constexpr auto blocked_by_protect(Target const target, MoveName const move) -> bool {
	switch (target) {
		case Target::user:
		case Target::all_allies:
		case Target::user_and_all_allies:
		case Target::all:
		case Target::field:
		case Target::user_team:
		case Target::user_field:
		case Target::foe_field:
			return false;
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::adjacent_foe:
		case Target::all_adjacent_foes:
		case Target::any:
		case Target::all_adjacent:
			return true;
		case Target::adjacent:
			switch (move) {
				case MoveName::Feint:
				case MoveName::Hyperspace_Fury:
				case MoveName::Hyperspace_Hole:
				case MoveName::Phantom_Force:
				case MoveName::Shadow_Force:
					return false;
				default:
					return true;
			}
	}
}

constexpr auto fails_against_fainted(Target const target) -> bool {
	switch (target) {
		case Target::user:
		case Target::all_allies:
		case Target::user_and_all_allies:
		case Target::all:
		case Target::field:
		case Target::user_team:
		case Target::user_field:
		case Target::all_adjacent_foes:
		case Target::foe_field:
		case Target::all_adjacent:
			return false;
		case Target::adjacent_ally:
		case Target::user_or_adjacent_ally:
		case Target::adjacent_foe:
		case Target::adjacent:
		case Target::any:
			return true;
	}
}

export constexpr auto move_is_unsuccessful(Target const target, MoveName const move, bool const user_damaged, CurrentHP const other_hp, Ability const other_ability, OtherAction const other_action, bool const is_protecting) -> bool {
	return
		move_fails(move, user_damaged, other_ability, other_action) or
		(other_hp == 0_bi and fails_against_fainted(target)) or
		(is_protecting and blocked_by_protect(target, move));
}

} // namespace technicalmachine
