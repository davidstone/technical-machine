// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.clients.visible_damage_to_actual_damage;

import tm.clients.move_result;
import tm.clients.visible_damage_to_actual_damage;

import tm.move.actual_damage;

import tm.pokemon.substitute;

import tm.stat.hp;

import tm.visible_hp;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

static_assert(
	visible_damage_to_actual_damage(
		Damage(VisibleHP(CurrentVisibleHP(95_bi), MaxVisibleHP(100_bi))),
		true,
		HP(100_bi),
		Substitute()
	) == FlaggedActualDamage(ActualDamage::Known(5_bi), true)
);

} // namespace
} // namespace technicalmachine
