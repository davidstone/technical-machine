// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/moves.hpp>

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/stat/hp.hpp>

#include <tm/any_team.hpp>
#include <tm/generation.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <containers/static_vector.hpp>
#include <containers/trivial_inplace_function.hpp>

namespace technicalmachine {

template<typename UserTeam>
struct SideEffect {
	using Function = containers::trivial_inplace_function<void(UserTeam & user, UserTeam & other, Weather &, HP::current_type) const, 0>;
	double probability;
	Function function;
};

inline constexpr auto no_effect_function = [](auto &, auto &, auto &, auto) {};

constexpr auto reflected_status(Generation const generation, Statuses const status) -> bounded::optional<Statuses> {
	switch (status) {
		case Statuses::burn:
		case Statuses::paralysis:
		case Statuses::poison:
			return status;
		case Statuses::toxic:
			return generation <= Generation::four ? Statuses::poison : Statuses::toxic;
		case Statuses::freeze:
		case Statuses::sleep:
			return bounded::none;
		case Statuses::clear:
		case Statuses::rest:
			bounded::unreachable();
	}
}

template<any_mutable_active_pokemon MutableActivePokemonType>
auto apply_status(Statuses const status, MutableActivePokemonType const user, MutableActivePokemonType const target, Weather const weather) {
	target.set_status(status, weather);
	auto const reflected = reflected_status(generation_from<MutableActivePokemonType>, status);
	if (reflected and reflects_status(target.ability())) {
		user.set_status(*reflected, weather);
	}
}

template<typename UserTeam>
using SideEffects = containers::static_vector<SideEffect<UserTeam>, 15>;

template<Generation generation>
auto possible_side_effects(Moves, ActivePokemon<generation> user, Team<generation> const & other, Weather) -> SideEffects<Team<generation>>;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto possible_side_effects(Moves, ActivePokemon<generation>, Team<generation> const &, Weather) -> SideEffects<Team<generation>>

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

} // namespace technicalmachine
