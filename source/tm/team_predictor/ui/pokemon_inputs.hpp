// Class to abstract UI of getting each Pokemon for the team builder
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

#include <tm/team_predictor/ui/ev_inputs.hpp>
#include <tm/team_predictor/ui/move_inputs.hpp>
#include <tm/team_predictor/ui/nature_input.hpp>
#include <tm/team_predictor/ui/species_input.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>
#include <tm/stat/ev.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {
enum class Moves : std::uint16_t;

struct PokemonInputs {
	struct construct_t{};
	static constexpr auto construct = construct_t{};
	// FLTK does not support move semantics. We do not want a single-argument
	// implicit constructor, but we need an implicit constructor to put this
	// class in an array.
	PokemonInputs(int & button_number, construct_t);
	Species species() const;
	bool is_valid() const;
	Nature nature() const;
	EV hp() const;
	EV atk() const;
	EV def() const;
	EV spa() const;
	EV spd() const;
	EV spe() const;
	auto moves() const {
		containers::static_vector<Moves, max_moves_per_pokemon.value()> result;
		for (auto const & move : m_moves.value) {
			try {
				result.emplace_back(move.value());
			} catch (InvalidFromStringConversion const &) {
				// Ignore invalid moves
			}
		}
		return result;
	}

private:
	SpeciesInput m_species;
	NatureInput m_nature;
	EVInputs m_evs;
	MoveInputs m_moves;
};

}	// namespace technicalmachine
