// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.do_switch;

import tm.move.actual_damage;
import tm.move.end_of_attack;
import tm.move.is_switch;
import tm.move.move_name;
import tm.move.used_move;

import tm.any_team;
import tm.environment;
import tm.other_team;

import bounded;

namespace technicalmachine {

using namespace bounded::literal;

export template<any_team UserTeam>
constexpr auto do_switch(
	UserTeam & user,
	MoveName const switch_,
	OtherTeam<UserTeam> & other,
	Environment & environment
) -> void {
	auto const replacing_fainted = user.pokemon().hp().current() == 0_bi;
	auto const other_pokemon = other.pokemon();
	user.switch_pokemon(other_pokemon, environment, to_replacement(switch_));
	if (!replacing_fainted) {
		end_of_attack(user.pokemon(), other_pokemon, environment);
	}
}

} // namespace technicalmachine
