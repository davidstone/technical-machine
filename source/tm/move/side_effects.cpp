// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.move.side_effects;

import tm.move.move_name;
import tm.move.side_effect_function;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;
import tm.pokemon.known_pokemon;
import tm.pokemon.pokemon;
import tm.pokemon.seen_pokemon;

import tm.any_team;
import tm.associated_team;
import tm.environment;
import tm.other_team;
import tm.probability;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

template<any_team UserTeam>
struct SideEffect {
	Probability probability;
	SideEffectFunction<UserTeam> function;
};

} // namespace technicalmachine

template<technicalmachine::any_team UserTeam>
struct bounded::tombstone<technicalmachine::SideEffect<UserTeam>> {
private:
	using T = technicalmachine::SideEffect<UserTeam>;
	using base = tombstone_traits<technicalmachine::SideEffectFunction<UserTeam>>;
public:
	static constexpr auto make(bounded::constant_t<0>) noexcept {
		return T(
			technicalmachine::Probability(0.0),
			base::make(0_bi)
		);
	}
	static constexpr auto index(T const & value) noexcept {
		return base::index(value.function);
	}
};

namespace technicalmachine {

template<any_team UserTeam>
using SideEffects = containers::static_vector<SideEffect<UserTeam>, 15_bi>;

export template<any_active_pokemon UserPokemon>
auto possible_side_effects(
	MoveName const move,
	UserPokemon const original_user,
	OtherTeam<AssociatedTeam<UserPokemon>> const & original_other,
	Environment const original_environment
) -> SideEffects<AssociatedTeam<UserPokemon>>;

#define EXTERN_INSTANTIATION_ONE(UserPokemon) \
	extern template auto possible_side_effects( \
		MoveName const move, \
		UserPokemon const original_user, \
		OtherTeam<AssociatedTeam<UserPokemon>> const & original_other, \
		Environment const original_environment \
	) -> SideEffects<AssociatedTeam<UserPokemon>>

#define EXTERN_INSTANTIATION_ALL(generation) \
	EXTERN_INSTANTIATION_ONE(AnyActivePokemon<Pokemon<generation>>); \
	EXTERN_INSTANTIATION_ONE(AnyActivePokemon<KnownPokemon<generation>>); \
	EXTERN_INSTANTIATION_ONE(AnyActivePokemon<SeenPokemon<generation>>)

TM_FOR_EACH_GENERATION(EXTERN_INSTANTIATION_ALL);

} // namespace technicalmachine
