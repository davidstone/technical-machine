// Class to abstract UI of getting each move for the team builder
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

#include "pokemon_inputs.hpp"
#include "../../pokemon/species_forward.hpp"
#include "../../string_conversions/invalid_string_conversion.hpp"

namespace technicalmachine {

PokemonInputs::PokemonInputs(int & button_number):
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

std::vector<Moves> PokemonInputs::moves() const {
	std::vector<Moves> result;
	auto const all = {
		&m_moves.input0, &m_moves.input1,
		&m_moves.input2, &m_moves.input3
	};
	for (auto const & move : all) {
		try {
			result.emplace_back(move->value());
		}
		catch (InvalidFromStringConversion const &) {
			// Ignore invalid moves
		}
	}
	return result;
}

}	// namespace technicalmachine
