// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.to_used_move;

import tm.move.move_result;

import tm.move.side_effects;
import tm.move.used_move;

import tm.pokemon.any_pokemon;
import tm.pokemon.max_pokemon_per_team;

import tm.any_team;
import tm.associated_team;
import tm.environment;
import tm.other_team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<any_active_pokemon UserPokemon>
constexpr auto get_side_effect(VisibleMove const move, UserPokemon const user, OtherTeam<AssociatedTeam<UserPokemon>> const & other, Environment const environment) {
	auto const side_effects = possible_side_effects(move.executed, user, other, environment);

	if (containers::size(side_effects) == 1_bi) {
		return containers::front(side_effects).function;
	}

	if (move.phaze_index) {
		if (move.confuse or move.status) {
			throw std::runtime_error("Tried to phaze and do other side effects.");
		}
		auto const index = *move.phaze_index;
		if (index == other.all_pokemon().index()) {
			throw std::runtime_error("Tried to phaze in current Pokemon");
		}
		using PhazeIndex = bounded::integer<0, bounded::normalize<max_pokemon_per_team - 2_bi>>;
		auto const effect_index = (index < other.all_pokemon().index()) ?
			bounded::assume_in_range<PhazeIndex>(index) :
			bounded::assume_in_range<PhazeIndex>(index - 1_bi);
		return side_effects[effect_index].function;
	}

	// TODO: Handle multi-effect situations
	if (move.confuse or move.status) {
		return side_effects[1_bi].function;
	}

	return containers::front(side_effects).function;
}

export template<any_team UserTeam>
constexpr auto to_used_move(VisibleMove const move, UserTeam const & user, OtherTeam<UserTeam> const & other, Environment const environment) -> UsedMove<UserTeam> {
	return UsedMove<UserTeam>(
		move.selected,
		move.executed,
		move.critical_hit,
		move.miss,
		move.contact_ability_effect,
		get_side_effect(move, user.pokemon(), other, environment)
	);
}

} // namespace technicalmachine
