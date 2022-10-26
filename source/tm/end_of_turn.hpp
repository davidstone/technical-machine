// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/any_team.hpp>
#include <tm/for_each_generation.hpp>
#include <tm/generation.hpp>
#include <tm/heal.hpp>
#include <tm/other_team.hpp>
#include <tm/rational.hpp>
#include <tm/weather.hpp>

#include <cstdint>

namespace technicalmachine {

struct EndOfTurnFlags {
	constexpr EndOfTurnFlags(bool const shed_skin_, bool const lock_in_ends_, bool const thaws_):
		shed_skin(shed_skin_),
		lock_in_ends(lock_in_ends_),
		thaws(thaws_)
	{
	}
	bool shed_skin;
	bool lock_in_ends;
	bool thaws;
};

constexpr auto handle_curse(any_mutable_active_pokemon auto const pokemon, Weather const weather) -> void {
	if (!pokemon.is_cursed()) {
		return;
	}
	heal(pokemon, weather, rational(-1_bi, 4_bi));
}

template<any_team TeamType>
void end_of_turn(TeamType & first, EndOfTurnFlags first_flags, OtherTeam<TeamType> & last, EndOfTurnFlags last_flags, Weather &);

#define TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(TeamType) \
	extern template void end_of_turn(TeamType & first, EndOfTurnFlags first_flags, OtherTeam<TeamType> & last, EndOfTurnFlags last_flags, Weather &)

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(Team<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(KnownTeam<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(SeenTeam<generation>)

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXTERN_INSTANTIATION);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION
#undef TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL

} // namespace technicalmachine
