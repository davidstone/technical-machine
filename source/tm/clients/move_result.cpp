// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.move_result;

import tm.move.move_name;

import tm.pokemon.max_pokemon_per_team;

import tm.status.status_name;

import tm.contact_ability_effect;
import tm.visible_hp;

import tv;

namespace technicalmachine {

export struct NoDamage {};
export struct SubstituteDamaged {};
export struct SubstituteBroke {};
export using Damage = tv::variant<NoDamage, VisibleHP, SubstituteDamaged, SubstituteBroke>;

export struct InitialMoveResult {};
export struct Awakening {};
export struct Flinched {};
export struct FrozenSolid {};
export struct FullyParalyzed {};
export struct Recharging {};
export struct Used {
	MoveName selected;
	MoveName executed = selected;
	Damage damage = Damage(NoDamage());
	tv::optional<StatusName> status = tv::none;
	tv::optional<TeamIndex> phaze_index = tv::none;
	ContactAbilityEffect contact_ability_effect = ContactAbilityEffect::nothing;
	bool critical_hit = false;
	bool miss = false;
	bool confuse = false;
	bool recoil = false;
};
export using MoveResult = tv::variant<
	InitialMoveResult,
	Awakening,
	Flinched,
	FrozenSolid,
	FullyParalyzed,
	Recharging,
	Used
>;

} // namespace technicalmachine
