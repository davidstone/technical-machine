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
