// Class to abstract UI of getting each move for the team builder
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

#include <tm/team_predictor/ui/pokemon_inputs.hpp>
#include <tm/pokemon/species_forward.hpp>

namespace technicalmachine {

PokemonInputs::PokemonInputs(int & button_number, construct_t):
	m_species(button_number++),
	m_nature(button_number++),
	m_evs(button_number++),
	m_moves(button_number++)
	{
}
Species PokemonInputs::species() const {
	return m_species.value();
}
bool PokemonInputs::is_valid() const {
	try {
		m_species.value();
		return true;
	}
	catch (InvalidFromStringConversion const &) {
		return false;
	}
}

Nature PokemonInputs::nature() const {
	return m_nature.value();
}

EV PokemonInputs::hp() const {
	return m_evs.hp.value();
}
EV PokemonInputs::atk() const {
	return m_evs.atk.value();
}
EV PokemonInputs::def() const {
	return m_evs.def.value();
}
EV PokemonInputs::spa() const {
	return m_evs.spa.value();
}
EV PokemonInputs::spd() const {
	return m_evs.spd.value();
}
EV PokemonInputs::spe() const {
	return m_evs.spe.value();
}

}	// namespace technicalmachine
