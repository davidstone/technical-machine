// Exception if unable to find a Pokemon that should exist
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/pokemon_not_found.hpp>
#include <tm/string_conversions/species.hpp>

#include <string>

namespace technicalmachine {

PokemonNotFound::PokemonNotFound(Species const species):
	std::logic_error("Unable to find " + std::string(to_string(species))) {
}

}	// namespace technicalmachine
