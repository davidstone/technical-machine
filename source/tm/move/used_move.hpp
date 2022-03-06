// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/moves.hpp>
#include <tm/move/side_effects.hpp>

#include <tm/any_team.hpp>
#include <tm/contact_ability_effect.hpp>

namespace technicalmachine {

template<any_team UserTeam>
struct UsedMove {
	constexpr UsedMove(
		Moves const selected_,
		Moves const executed_,
		bool const critical_hit_,
		bool const miss_,
		ContactAbilityEffect const contact_ability_effect_,
		typename SideEffect<UserTeam>::Function const side_effect_
	):
		side_effect(side_effect_),
		selected(selected_),
		executed(executed_),
		critical_hit(critical_hit_),
		miss(miss_),
		contact_ability_effect(contact_ability_effect_)
	{
	}

	constexpr UsedMove(Moves const selected_, typename SideEffect<UserTeam>::Function const side_effect_):
		side_effect(side_effect_),
		selected(selected_),
		executed(selected),
		critical_hit(false),
		miss(false),
		contact_ability_effect(ContactAbilityEffect::nothing)
	{
	}

	typename SideEffect<UserTeam>::Function side_effect;
	Moves selected;
	Moves executed;
	bool critical_hit;
	bool miss;
	ContactAbilityEffect contact_ability_effect;
};

} // namespace technicalmachine
