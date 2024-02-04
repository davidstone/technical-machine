// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.hit_self;

import tm.move.damage_type;
import tm.move.move_name;

import tm.pokemon.applied_damage;

import tm.any_team;
import tm.environment;

import bounded;

namespace technicalmachine {

using namespace bounded::literal;

export template<any_team UserTeam>
constexpr auto hit_self(
	UserTeam & user,
	damage_type const damage,
	Environment const environment
) -> void {
	auto const user_pokemon = user.pokemon();
	user_pokemon.advance_confusion();
	// TODO: ???
	constexpr auto irrelevant_move = MoveName::Struggle;
	auto const applied = applied_damage(user_pokemon.as_const(), irrelevant_move, damage, environment);
	user_pokemon.indirect_damage(environment, applied.damage);
	user_pokemon.hit_self();
}

} // namespace technicalmachine
