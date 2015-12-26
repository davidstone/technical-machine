// Read "User" message from PO
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

#include "conversion.hpp"

#include "../../pokemon/max_pokemon_per_team.hpp"

#include <containers/static_vector/static_vector.hpp>

#include <cstdint>
#include <string>
#include <utility>

namespace technicalmachine {
namespace po {

struct InMessage;

using PokemonStaticVector = containers::static_vector<SpeciesIDs, max_pokemon_per_team.value()>;

struct User {
	explicit User(InMessage & msg);

	uint32_t const id;
	std::string const name;
	std::string const info;
	int8_t const authority;
	uint8_t const flags;
	bool const logged_in;
	bool const battling;
	bool const away;
	int16_t const rating;
	PokemonStaticVector const team;
	uint16_t const avatar;
	std::string const tier;
	uint8_t const color_spec;
	uint16_t const alpha;
	uint16_t const red;
	uint16_t const green;
	uint16_t const blue;
	uint16_t const padding;
	uint8_t const gen;
};

}	// namespace po
}	// namespace technicalmachine
