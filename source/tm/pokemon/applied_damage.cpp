// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.applied_damage;

import tm.move.damage_type;
import tm.move.move_name;

import tm.pokemon.any_pokemon;
import tm.pokemon.can_be_koed;

import tm.stat.current_hp;

import tm.environment;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

constexpr bool cannot_ko(MoveName const move) {
	return move == MoveName::False_Swipe;
}

struct AppliedDamage {
	CurrentHP damage;
	bool consume_item;
};
export constexpr auto applied_damage(
	any_active_pokemon auto pokemon,
	MoveName const move,
	damage_type const damage,
	Environment const environment
) -> AppliedDamage {
	auto const hp = pokemon.hp();
	auto const actual = bounded::min(damage, hp.max());
	if (hp.current() > damage) {
		return AppliedDamage(actual, false);
	}
	auto const capped = bounded::assume_in_range<CurrentHP>(hp.current() - 1_bi);
	if (cannot_ko(move)) {
		return AppliedDamage(capped, false);
	}
	switch (can_be_koed(pokemon, environment)) {
		case CanBeKOed::yes: return AppliedDamage(actual, false);
		case CanBeKOed::no: return AppliedDamage(capped, false);
		case CanBeKOed::consume_item: return AppliedDamage(capped, true);
	}
}

} // namespace technicalmachine
