// Class to abstract UI of getting each Pokemon for the team builder
// Copyright (C) 2012 David Stone
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

#ifndef TEAM_PREDICTOR__UI__POKEMON_INPUTS_HPP_
#define TEAM_PREDICTOR__UI__POKEMON_INPUTS_HPP_

#include <vector>
#include "ev_inputs.hpp"
#include "move_inputs.hpp"
#include "nature_input.hpp"
#include "species_input.hpp"
#include "../../move/moves_forward.hpp"
#include "../../pokemon/species_forward.hpp"

namespace technicalmachine {

class PokemonInputs {
public:
	explicit PokemonInputs(int & button_number);
	Species species() const;
	bool is_valid() const;
	Nature::Natures nature() const;
	unsigned hp() const;
	unsigned atk() const;
	unsigned def() const;
	unsigned spa() const;
	unsigned spd() const;
	unsigned spe() const;
	std::vector<Moves> moves() const;
private:
	SpeciesInput m_species;
	NatureInput m_nature;
	EVInputs m_evs;
	MoveInputs m_moves;
};

}	// namespace technicalmachine
#endif	// TEAM_PREDICTOR__UI__POKEMON_INPUTS_HPP_
