// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/move/moves.hpp>

#include <tm/stat/hp.hpp>

#include <tm/generation.hpp>
#include <tm/status.hpp>
#include <tm/weather.hpp>

#include <containers/static_vector/static_vector.hpp>
#include <containers/trivial_inplace_function.hpp>

namespace technicalmachine {

template<Generation>
struct ActivePokemon;
template<Generation>
struct MutableActivePokemon;
template<Generation>
struct Team;

template<Generation generation>
struct SideEffect {
	using Function = containers::trivial_inplace_function<void(Team<generation> & user, Team<generation> & other, Weather &, HP::current_type) const, 0>;
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

template<Generation generation>
auto apply_status(Statuses const status, MutableActivePokemon<generation> const user, MutableActivePokemon<generation> const target, Weather const weather) {
	target.set_status(status, weather);
	auto const reflected = reflected_status(generation, status);
	if (reflected and reflects_status(target.ability())) {
		user.set_status(*reflected, weather);
	}
}

template<Generation generation>
using SideEffects = containers::static_vector<SideEffect<generation>, 15>;

template<Generation generation>
auto possible_side_effects(Moves, ActivePokemon<generation> user, Team<generation> const & other, Weather) -> SideEffects<generation>;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto possible_side_effects<generation>(Moves, ActivePokemon<generation>, Team<generation> const &, Weather) -> SideEffects<generation>

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
