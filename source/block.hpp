// Declaration of functions that block selection / execution
// Copyright (C) 2015 David Stone
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

#include "move/max_moves_per_pokemon.hpp"
#include "move/moves.hpp"
#include "pokemon/species_forward.hpp"

#include <containers/static_vector/static_vector.hpp>

#include <cstddef>

namespace technicalmachine {

struct ActivePokemon;
struct MutableActivePokemon;
struct Team;
struct Weather;

using LegalSelectionsBase = containers::static_vector<Moves, static_cast<std::intmax_t>(std::numeric_limits<MoveSize>::max())>;

struct LegalSelections : private LegalSelectionsBase {
	using LegalSelectionsBase::value_type;
	using LegalSelectionsBase::size_type;
	using LegalSelectionsBase::const_iterator;

	LegalSelections(Team const & user, ActivePokemon other, Weather weather);
	auto species() const {
		return m_species;
	}

	using LegalSelectionsBase::begin;
	using LegalSelectionsBase::end;
	using LegalSelectionsBase::operator[];
private:
	Species m_species;
};

bool can_execute_move(MutableActivePokemon user, ActivePokemon other, Weather weather);

}	// namespace technicalmachine
