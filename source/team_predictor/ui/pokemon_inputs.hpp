// Class to abstract UI of getting each Pokemon for the team builder
// Copyright (C) 2014 David Stone
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

#include <vector>
#include "ev_inputs.hpp"
#include "move_inputs.hpp"
#include "nature_input.hpp"
#include "species_input.hpp"
#include "../../move/moves.hpp"
#include "../../pokemon/species_forward.hpp"
#include "../../stat/ev.hpp"

namespace technicalmachine {

class PokemonInputs {
public:
	explicit PokemonInputs(int & button_number);
	Species species() const;
	bool is_valid() const;
	Nature nature() const;
	EV hp() const;
	EV atk() const;
	EV def() const;
	EV spa() const;
	EV spd() const;
	EV spe() const;
	std::vector<Moves> moves() const;
private:
	SpeciesInput m_species;
	NatureInput m_nature;
	EVInputs m_evs;
	MoveInputs m_moves;
};

}	// namespace technicalmachine
