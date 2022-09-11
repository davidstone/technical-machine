// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/move_name.hpp>

#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <tm/contact_ability_effect.hpp>
#include <tm/status_name.hpp>
#include <tm/visible_hp.hpp>

#include <bounded/detail/variant/variant.hpp>
#include <bounded/optional.hpp>

#include <stdexcept>

namespace technicalmachine {

struct NoDamage {};
struct SubstituteDamaged {};
struct SubstituteBroke {};
using Damage = bounded::variant<NoDamage, VisibleHP, SubstituteDamaged, SubstituteBroke>;

struct InitialMoveResult {};
struct Awakening {};
struct Flinched {};
struct FrozenSolid {};
struct FullyParalyzed {};
struct Recharging {};
struct Used {
	MoveName selected;
	MoveName executed = selected;
	Damage damage = Damage(NoDamage());
	bounded::optional<StatusName> status = bounded::none;
	bounded::optional<TeamIndex> phaze_index = bounded::none;
	ContactAbilityEffect contact_ability_effect = ContactAbilityEffect::nothing;
	bool critical_hit = false;
	bool miss = false;
	bool confuse = false;
	bool recoil = false;
};
using MoveResult = bounded::variant<
	InitialMoveResult,
	Awakening,
	Flinched,
	FrozenSolid,
	FullyParalyzed,
	Recharging,
	Used
>;

} // namespace technicalmachine
