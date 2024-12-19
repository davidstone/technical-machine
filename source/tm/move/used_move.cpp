// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.used_move;

import tm.move.move_name;
import tm.move.side_effect_function;

import tm.any_team;
import tm.contact_ability_effect;

namespace technicalmachine {

export template<any_team UserTeam>
struct UsedMove {
	constexpr UsedMove(
		MoveName const selected_,
		MoveName const executed_,
		bool const critical_hit_,
		bool const miss_,
		bool const action_ends_,
		ContactAbilityEffect const contact_ability_effect_,
		SideEffectFunction<UserTeam> const side_effect_
	):
		side_effect(side_effect_),
		selected(selected_),
		executed(executed_),
		critical_hit(critical_hit_),
		miss(miss_),
		action_ends(action_ends_),
		contact_ability_effect(contact_ability_effect_)
	{
	}

	constexpr UsedMove(MoveName const selected_, SideEffectFunction<UserTeam> const side_effect_):
		side_effect(side_effect_),
		selected(selected_),
		executed(selected),
		critical_hit(false),
		miss(false),
		action_ends(false),
		contact_ability_effect(ContactAbilityEffect::nothing)
	{
	}

	SideEffectFunction<UserTeam> side_effect;
	MoveName selected;
	MoveName executed;
	bool critical_hit;
	bool miss;
	bool action_ends;
	ContactAbilityEffect contact_ability_effect;
};

} // namespace technicalmachine
