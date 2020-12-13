// Exception if unable to find a Pokemon that should exist
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/species_forward.hpp>

#include <stdexcept>

namespace technicalmachine {

struct PokemonNotFound : std::logic_error {
	explicit PokemonNotFound(Species const species);
};
}	// namespace technicalmachine
