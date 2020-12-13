// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>
#include <tm/weather.hpp>

#include <cstdint>

namespace technicalmachine {

template<Generation generation>
struct Team;

struct EndOfTurnFlags {
	constexpr EndOfTurnFlags(bool const shed_skin_, bool const lock_in_ends_):
		shed_skin(shed_skin_),
		lock_in_ends(lock_in_ends_)
	{
	}
	bool shed_skin;
	bool lock_in_ends;
};

template<Generation generation>
void end_of_attack(Team<generation> &, Team<generation> &, Weather) {
}

void end_of_attack(Team<Generation::two> & user, Team<Generation::two> & other, Weather);

template<Generation generation>
void end_of_turn(Team<generation> & first, EndOfTurnFlags first_flags, Team<generation> & last, EndOfTurnFlags last_flags, Weather &);

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template void end_of_turn<generation>(Team<generation> & first, EndOfTurnFlags first_flags, Team<generation> & last, EndOfTurnFlags last_flags, Weather &)

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

}	// namespace technicalmachine
