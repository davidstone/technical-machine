// Declaration of functions that block selection / execution
// Copyright (C) 2016 David Stone
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

#include <cstddef>

namespace technicalmachine {

struct ActivePokemon;
struct MutableActivePokemon;
struct Team;
struct Weather;

struct LegalSelections : private StaticVectorMoves {
	using StaticVectorMoves::value_type;
	using StaticVectorMoves::size_type;
	using StaticVectorMoves::const_iterator;

	LegalSelections(Team const & user, ActivePokemon other, Weather weather);
	auto species() const {
		return m_species;
	}

	friend constexpr auto begin(LegalSelections const & container) {
		return begin(static_cast<StaticVectorMoves const &>(container));
	}
	friend constexpr auto end(LegalSelections const & container) {
		return end(static_cast<StaticVectorMoves const &>(container));
	}
	using StaticVectorMoves::operator[];
private:
	Species m_species;
};

bool can_execute_move(MutableActivePokemon user, ActivePokemon other, Weather weather, bool awakens);

}	// namespace technicalmachine
