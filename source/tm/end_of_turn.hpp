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

#include <cstdint>

namespace technicalmachine {

enum class Generation : std::uint8_t;
struct Team;
struct Weather;

struct EndOfTurnFlags {
	constexpr EndOfTurnFlags(bool const shed_skin_, bool const lock_in_ends_):
		shed_skin(shed_skin_),
		lock_in_ends(lock_in_ends_)
	{
	}
	bool shed_skin;
	bool lock_in_ends;
};

void end_of_attack(Generation, Team & user, Team & other, Weather);

void end_of_turn(Generation, Team & first, EndOfTurnFlags first_flags, Team & last, EndOfTurnFlags last_flags, Weather &);

}	// namespace technicalmachine