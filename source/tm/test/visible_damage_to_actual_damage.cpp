// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.test.visible_damage_to_actual_damage;

import tm.move.actual_damage;
import tm.move.move_result;

import tm.pokemon.substitute;

import tm.stat.hp;

import tm.visible_damage_to_actual_damage;
import tm.visible_hp;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto exact_hp = true;
constexpr auto inexact_hp = false;

static_assert(
	visible_damage_to_actual_damage(
		Damage(VisibleHP(CurrentVisibleHP(95_bi), MaxVisibleHP(100_bi))),
		exact_hp,
		HP(100_bi),
		Substitute()
	) == ActualDamage::Known(5_bi)
);

static_assert(
	visible_damage_to_actual_damage(
		Damage(VisibleHP(CurrentVisibleHP(90_bi), MaxVisibleHP(100_bi))),
		inexact_hp,
		HP(293_bi),
		Substitute()
	) == ActualDamage::Known(30_bi)
);

} // namespace
} // namespace technicalmachine
